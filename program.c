#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "common.c"
#include "notes.c"
//#include "music/bach.c"
//#include "music/lostelf.c"
//#include "music/testnotation.c"
#include "music/viricorda.c"

static uint64_t sleepUnit = 512;
static uint8_t rhythmUnit = 255; // also set dynamically by tracks

#define SLEEP_DELAY _delay_us(sleepUnit);

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

const instrument instruments[] =
{
    instSilence, instRegular
};

static void readTrack(track *target)
{
    if (target->remainingSleepTime > 0)
    {
        target->remainingSleepTime-=sleepUnit;
        return;
    }

    if (target->sPosition >= target->sLength-1)
    {
        target->sPosition = 0;
    }

    uint16_t code = target->sequence[target->sPosition];
    uint16_t position = target->sPosition;
    uint8_t *tSequence = target->sequence;
    channel *tChannel = target->channel;

    switch (code) 
    {
        case 0:
            // sleep for duration
            target->remainingSleepTime =
                tSequence[position+1] * sleepUnit * rhythmUnit;
            target->sPosition = position+2;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            // Set pitches
            // pitches + sleep combo to save space
            target->remainingSleepTime =
                tSequence[position+code+1] * sleepUnit * rhythmUnit;
            tChannel->currentPitchCount = code;
            target->sPosition = position + code + 2;
            for (int i = 0; i < code; i++)
            {
                tChannel->currentPitches[i] = tSequence[position+i+1];
            }
            tChannel->nextPitchIndex = 0;
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
            if (target->jPosition == position)
            {
                target->sPosition = position+2;
                target->jPosition = 0;
            }
            else
            {
                target->jPosition = position;
                target->sPosition = position-(tSequence[position+1]);
            }
            break;
        default:
            /*for (int i = 0; i < code; i++)
            {
                SET_BIT(PORTB, 5);
                _delay_ms(500);
                UNSET_BIT(PORTB, 5);
                _delay_ms(500);
            }*/
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

static void initializeDevice8(device *device, volatile uint8_t *pitch, volatile uint8_t *width)
{
    device->pitch.addressLength = 8;
    device->width.addressLength = 8;
    device->pitch.address.eight = pitch;
    device->width.address.eight = width;
}

static void initializeDevice16(device *device, volatile uint16_t *pitch, volatile uint16_t *width)
{
    device->pitch.addressLength = 16;
    device->width.addressLength = 16;
    device->pitch.address.sixteen = pitch;
    device->width.address.sixteen = width;
}

static device* initializeDevices(void)
{
    device *devices = malloc(sizeof(device)*2);

    // timer counter 0
    initializeDevice8(&devices[0], &OCR0A, &OCR0B);
    // timer counter 1
    initializeDevice16(&devices[1], &OCR1A, &OCR1B);

    return devices;
}

static void initializeChannel(channel *channel, device *device)
{
    channel->device = device;
    channel->currentTone = 0;
    for(int i = 0; i < 4; i++)
    {
        channel->currentPitches[i] = 255;
    }
    channel->currentPitchCount = 0;
    channel->nextPitchIndex = 0;
    channel->polyCycleThreshold = 96;
    channel->polyCycleCounter = 0;
    channel->instrument = instruments[0];
}

static channel* initializeChannels(device *devices)
{
    channel *channels = malloc(sizeof(channel)*2);
    initializeChannel(&channels[0], &devices[0]);
    initializeChannel(&channels[1], &devices[1]);
    return channels;
}

static void initializeTrack(track *track, channel *channel, uint8_t *sequence, uint16_t sequenceLength)
{
    track->channel = channel;
    track->sequence = sequence;
    track->sLength = sequenceLength;
    track->sPosition = 0;
    track->remainingSleepTime = 0;
    track->jPosition = 0;
}
static track* initializeTracks(channel* channels)
{
    track *tracks = malloc(sizeof(track)*2);
    initializeTrack(&tracks[0], &channels[0], sequenceBass, sequenceBassLength);
    initializeTrack(&tracks[1], &channels[1], sequenceTreble, sequenceTrebleLength);
    return tracks;
}

int main(void)
{
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

    for(;;)
    {
        for (int i = 0; i < 2; i++)
        {
            readTrack(&tracks[i]);
        }
        for (int i = 0; i < 2; i++)
        {
            channels[i].instrument(&channels[i]);
        }
        SLEEP_DELAY
    }
}
