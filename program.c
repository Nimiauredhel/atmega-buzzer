#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "common.c"
#include "notes.c"
//#include "bach.c"
//#include "lostelf.c"
#include "testnotation.c"

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

    VLA_Write(channel->currentTone, &channel->device->width);
    VLA_Write(channel->currentPitches[channel->nextPitchIndex], 
            &channel->device->pitch);

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

    VLA_Write(channel->currentTone, &channel->device->width);
    VLA_Write((channel->currentPitches[(channel->nextPitchIndex)]
                *sinArr[wavePosition]) / 10, &channel->device->pitch);

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

// timer counter 0
static  void initializeTimerCounter0(void)
{
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << WGM02); 
}
static void timerCounter0Start(void)
{
    // 1 0 0 - clk/256
    //SET_BIT(TCCR0B, CS00);
    //SET_BIT(TCCR0B, CS01);
    SET_BIT(TCCR0B, CS02);
}
static void timerCounter0Stop(void)
{
    //UNSET_BIT(TCCR0B, CS00);
    //UNSET_BIT(TCCR0B, CS01);
    UNSET_BIT(TCCR0B, CS02);
}
// timer counter 1
static  void initializeTimerCounter1(void)
{
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << WGM13); 
}
static void timerCounter1Start(void)
{
    // 1 0 0 - clk/256
    //SET_BIT(TCCR1B, CS10);
    //SET_BIT(TCCR1B, CS11);
    SET_BIT(TCCR1B, CS12);
}
static void timerCounter1Stop(void)
{
    //UNSET_BIT(TCCR1B, CS10);
    //UNSET_BIT(TCCR1B, CS11);
    UNSET_BIT(TCCR1B, CS12);
}

static void initializePortB(void)
{
    DDRB = 0;
    SET_BIT(DDRB, DDB1);
    SET_BIT(DDRB, DDB2);
    SET_BIT(DDRB, DDB5);
}

static void initializePortD(void)
{
    DDRD = 0;
    SET_BIT(DDRD, DDD5);
    SET_BIT(DDRD, DDD6);
}

static device* initializeDevices(void)
{
    device *devices = malloc(sizeof(device)*2);

    // timer counter 0
    devices[0].pitch.addressLength = 8;
    devices[0].pitch.address.eight = &OCR0A;
    devices[0].width.addressLength = 8;
    devices[0].width.address.eight = &OCR0B;
    // timer counter 1
    devices[1].pitch.addressLength = 16;
    devices[1].pitch.address.sixteen = &OCR1A;
    devices[1].width.addressLength = 16;
    devices[1].width.address.sixteen = &OCR1B;

    return devices;
}

static channel* initializeChannels(device *devices)
{
    channel *channels = malloc(sizeof(channel)*2);

    channels[0].device = &devices[0];
    channels[0].currentTone = 0;
    channels[0].currentPitches[0] = 255;
    channels[0].currentPitches[1] = 255;
    channels[0].currentPitches[2] = 255;
    channels[0].currentPitches[3] = 255;
    channels[0].currentPitchCount = 0;
    channels[0].nextPitchIndex = 0;
    channels[0].polyCycleThreshold = 96;
    channels[0].polyCycleCounter = 0;
    channels[0].instrument = instruments[0];

    channels[1].device = &devices[1];
    channels[1].currentTone = 0;
    channels[1].currentPitches[0] = 255;
    channels[1].currentPitches[1] = 255;
    channels[1].currentPitches[2] = 255;
    channels[1].currentPitches[3] = 255;
    channels[1].currentPitchCount = 0;
    channels[1].nextPitchIndex = 0;
    channels[1].polyCycleThreshold = 96;
    channels[1].polyCycleCounter = 0;
    channels[1].instrument = instruments[0];

    return channels;
}

static track* initializeTracks(channel* channels)
{
    track *tracks = malloc(sizeof(track)*2);

    tracks[0].channel = &channels[0];
    tracks[0].sequence = sequenceBass;
    tracks[0].sLength = sizeof(sequenceBass) / sizeof(sequenceBass[0]);
    tracks[0].sPosition = 0;
    tracks[0].remainingSleepTime = 0;

    tracks[1].channel = &channels[1];
    tracks[1].sequence = sequenceTreble;
    tracks[1].sLength = sizeof(sequenceTreble) / sizeof(sequenceTreble[0]);
    tracks[1].sPosition = 0;
    tracks[1].remainingSleepTime = 0;

    return tracks;
}

int main(void)
{
    initializeSinArray();

    initializePortB();
    initializePortD();

    initializeTimerCounter0();
    initializeTimerCounter1();
    timerCounter0Start();
    timerCounter1Start();

    // turn off built-in led - to use for debugging later
    UNSET_BIT(PORTB, 5);

    // initialize the devices - interfaces to audio emitting hardware
    device *devices = initializeDevices();
    // initialize the channels - device usage & state management
    channel *channels = initializeChannels(devices);
    // initialize the tracks - parallel streams of commands to the channels
    track *tracks = initializeTracks(channels);

    SLEEP_DELAY

    for(;;)
    {
        readTrack(&tracks[0]);
        readTrack(&tracks[1]);
        channels[0].instrument(&channels[0]);
        channels[1].instrument(&channels[1]);
        SLEEP_DELAY
    }
}
