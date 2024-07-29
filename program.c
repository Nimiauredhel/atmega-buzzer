#include <avr/io.h>
#include <util/delay.h>
#include "common.c"
#include "notes.c"
#include "bach.c"
//#include "lostelf.c"

#define SLEEP_UNIT_USECS 1
#define SLEEP_DELAY _delay_us(SLEEP_UNIT_USECS);
#define RHYTHM_UNIT 6400

void instSilence(channel *channel)
{

}

void instRegular(channel *channel)
{
    if (channel->currentPitchCount == 0) return;

    *channel->toneReg =
        channel->currentTone;
    *channel->pitchReg =
        channel->currentPitches[channel->nextPitchIndex];
    if (channel->nextPitchIndex < channel->currentPitchCount-1)
    {
        channel->nextPitchIndex++;
    }
    else if (channel->nextPitchIndex == channel->currentPitchCount-1)
    {
        channel->nextPitchIndex = 0;
    }
}

void instSine(channel *channel)
{
    static double wavePosition = 0.501;
    static double waveDirection = 0.01;

    *channel->pitchReg =
        channel->currentPitches[(int)(channel->nextPitchIndex*sin(wavePosition))];
    *channel->toneReg =
        channel->currentTone;

    wavePosition += waveDirection;

    if (wavePosition >= 1.0 || wavePosition <= 0.5)
    {
        wavePosition *= -1;
    }

    if (channel->nextPitchIndex < channel->currentPitchCount-1)
    {
        channel->nextPitchIndex++;
    }
    else if (channel->nextPitchIndex == channel->currentPitchCount-1)
    {
        channel->nextPitchIndex = 0;
    }
}

instrument instruments[] =
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
    if (target->remainingSleepTime > 0)
    {
        target->remainingSleepTime -= SLEEP_UNIT_USECS;
        return;
    }

    if (target->sPosition >= target->sLength-1)
    {
        target->sPosition = 0;
    }

    uint32_t code = target->sequence[target->sPosition];
    uint16_t position = target->sPosition;
    uint8_t *tSequence = target->sequence;
    channel *tChannel = target->channel;

    switch (code) 
    {
        case 0:
            // sleep for duration
            target->remainingSleepTime =
                tSequence[position+1]*RHYTHM_UNIT;
            target->sPosition = position+2;
            break;
        case 1:
            // Set 1 pitch
            tChannel->currentPitchCount=1;
            tChannel->currentPitches[0] =
                tSequence[position+1];
            target->sPosition = position+2;
            break;
        case 2:
            // Set 2 pitches
            tChannel->currentPitchCount=2;
            tChannel->currentPitches[0] =
                tSequence[position+1];
            tChannel->currentPitches[1] =
                tSequence[position+2];
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
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << WGM02); 

    SET_BIT(DDRD, DDD5);
    SET_BIT(DDRB, DDB5);

    UNSET_BIT(PORTB, 5);
    pwmStart();

    channel channels[] =
    { 
        {
            &OCR0A,
            &OCR0B,
            0,
            { 0, 0, 0, 0 },
            0,
            0,
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

    OCR0B = 10;
    OCR0A = Gb4;

    for(;;)
    {
        readTrack(&tracks[0]);
        channels[0].instrument(&channels[0]);
        _delay_us(SLEEP_UNIT_USECS);
    }
}
