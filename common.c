#include <avr/io.h>

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))

typedef struct channel
{

    // write to this register to set the channel frequency
    volatile uint8_t *pitchReg;
    // write to this register to set the channel.. voltage?
    volatile uint8_t *toneReg;

    // the number of pitches represented by this channel
    uint8_t currentPitchCount;
    // array of pitches represented by this channel
    uint8_t currentPitches[4];
    // index of next pitch to sound on this channel
    uint8_t nextPitchIndex;
    // current "tone" (voltage?) set on this channel
    uint8_t currentTone;
    // the "instrument" function assigned to this channel, controlling the waveform etc.
    void (*instrument)(struct channel *ch);
} channel;

typedef struct track
{
    // target channel associated with this track
    channel *channel;
    // the sequence of commands associated with this track
    uint32_t *sequence;
    // the length of the command sequence
    uint16_t sLength;
    // te current position of the command sequence
    uint16_t sPosition;
    // remaining track sleep time in microseconds
    uint64_t remainingSleepTime;
} track;

typedef void(*instrument)(channel *channel);
