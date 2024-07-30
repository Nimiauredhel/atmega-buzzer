#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "common.c"
#include "notes.c"
//#include "bach.c"
#include "lostelf.c"
//#include "testnotation.c"

#define SIN_LENGTH 640

static uint64_t sleepUnit = 512;
static uint64_t rhythmUnit = 256;
static const uint16_t sinLength = SIN_LENGTH;
int8_t sinArr[SIN_LENGTH] = {0};

#define SLEEP_DELAY _delay_us(sleepUnit);

static void initializeSinArray()
{
    double value = 0.5;

    for (int i = 0; i < sinLength; i++)
    {
        sinArr[i] = (int8_t)(sin(value) * 10);
        value += 1.0 / sinLength;
    }
}

static void instSilence(channel *channel)
{

}

static void instRegular(channel *channel)
{
    if (channel->currentPitchCount == 0) return;

    if (channel->nextPitchIndex >= channel->currentPitchCount)
    {
        channel->nextPitchIndex = 0;
    }

    *channel->toneReg =
        channel->currentTone;
    *channel->pitchReg =
        channel->currentPitches[channel->nextPitchIndex];

    channel->polyCycleCounter++;

    if (channel->polyCycleCounter >= channel->polyCycleThreshold)
    {
        channel->polyCycleCounter = 0;
        if (channel->nextPitchIndex < channel->currentPitchCount-1)
        {
            channel->nextPitchIndex++;
        }
        else if (channel->nextPitchIndex == channel->currentPitchCount-1)
        {
            channel->nextPitchIndex = 0;
        }
    }
}

static void instSine(channel *channel)
{
    static const int8_t waveThreshold = 4;
    static int8_t waveCounter = 0;
    static uint16_t wavePosition = 0;
    static int8_t waveDirection = 1;

    *channel->pitchReg =
        (channel->currentPitches[(channel->nextPitchIndex)]
                *sinArr[wavePosition]) / 10;
    *channel->toneReg =
        channel->currentTone;

    waveCounter++;

    if (waveCounter >= waveThreshold)
    {
        waveCounter = 0;
        wavePosition += waveDirection;

        if (wavePosition >= sinLength || wavePosition <= 0)
        {
            waveDirection *= -1;
        }
    }

    channel->polyCycleCounter++;

    if (channel->polyCycleCounter >= channel->polyCycleThreshold)
    {
        channel->polyCycleCounter = 0;
        if (channel->nextPitchIndex < channel->currentPitchCount-1)
        {
            channel->nextPitchIndex++;
        }
        else if (channel->nextPitchIndex == channel->currentPitchCount-1)
        {
            channel->nextPitchIndex = 0;
        }
    }
}

const instrument instruments[] =
{
    instSilence, instRegular, instSine
};


static void pwmStart()
{
    //SET_BIT(TCCR0B, CS00);
    //SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS02);
}

static void pwmStop()
{
    UNSET_BIT(TCCR0B, CS00);
    UNSET_BIT(TCCR0B, CS01);
    UNSET_BIT(TCCR0B, CS02);
}

static void readTrack(track *target)
{
    static uint16_t jumpedFrom = 0;

    if (target->remainingSleepTime > 0)
    {
        target->remainingSleepTime-=sleepUnit;
        return;
    }

    if (target->sPosition >= target->sLength-1)
    {
        target->sPosition = 0;
    }

    uint32_t code = target->sequence[target->sPosition];
    uint16_t position = target->sPosition;
    uint32_t *tSequence = target->sequence;
    channel *tChannel = target->channel;

    switch (code) 
    {
        case 0:
            // sleep for duration
            target->remainingSleepTime =
                tSequence[position+1]*rhythmUnit*sleepUnit;
            target->sPosition = position+2;
            break;
        case 1:
            // Set 1 pitch
            tChannel->currentPitchCount=1;
            tChannel->currentPitches[0] =
                tSequence[position+1];
            tChannel->nextPitchIndex = 0;
            target->sPosition = position+2;
            break;
        case 2:
            // Set 2 pitches
            tChannel->currentPitchCount=2;
            tChannel->currentPitches[0] =
                tSequence[position+1];
            tChannel->currentPitches[1] =
                tSequence[position+2];
            tChannel->nextPitchIndex = 0;
            target->sPosition = position+3;
            break;
        case 3:
            // Set 3 pitches
            tChannel->currentPitchCount=3;
            tChannel->currentPitches[0] =
                tSequence[position+1];
            tChannel->currentPitches[1] =
                tSequence[position+2];
            tChannel->currentPitches[2] =
                target->sequence[position+3];
            tChannel->nextPitchIndex = 0;
            target->sPosition = position+4;
            break;
        case 4:
            // Set 4 pitches
            tChannel->currentPitchCount=4;
            tChannel->currentPitches[0] =
                tSequence[position+1];
            tChannel->currentPitches[1] =
                tSequence[position+2];
            tChannel->currentPitches[2] =
                tSequence[position+3];
            tChannel->currentPitches[3] =
                tSequence[position+4];
            tChannel->nextPitchIndex = 0;
            target->sPosition = position+5;
            break;
        case 5:
            // Set "tone" (voltage)
            tChannel->currentTone =
                tSequence[position+1];
            target->sPosition = position+2;
            break;
        case 6:
            // Set instrument function
            tChannel->instrument =
                instruments[tSequence[position+1]];
            target->sPosition = position+2;
            break;
        case 7:
            // Set rhythm unit (tempo)
            rhythmUnit =
                tSequence[position+1];
            target->sPosition = position+2;
            break;
        case 8:
            // Jump back (repeat)
            if (jumpedFrom == position)
            {
                target->sPosition = position+2;
                jumpedFrom = 0;
            }
            else
            {
                jumpedFrom = position;
                target->sPosition = position-(tSequence[position+1]);
            }
            break;
        default:
            for (int i = 0; i < code; i++)
            {
                SET_BIT(PORTB, 5);
                _delay_ms(500);
                UNSET_BIT(PORTB, 5);
                _delay_ms(500);
            }
        break;
    }
}

int main(void)
{
    initializeSinArray();

    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << WGM02); 

    SET_BIT(DDRD, DDD5);
    SET_BIT(DDRB, DDB5);

    UNSET_BIT(PORTB, 5);
    pwmStart();

    SLEEP_DELAY
    SLEEP_DELAY

    channel channels[] =
    { 
        {
            &OCR0A,
            &OCR0B,
            0,
            { 255, 255, 255, 255 },
            0, 0,
            96, 0,
            instruments[0]
        }
    };

    track tracks[] =
    {
        {
            &channels[0],
            sequence,
            sizeof(sequence) / sizeof(sequence[0]),
            0, 0
        }
    };

    SLEEP_DELAY
    SLEEP_DELAY

    for(;;)
    {
        readTrack(&tracks[0]);
        channels[0].instrument(&channels[0]);
        SLEEP_DELAY
    }
}
