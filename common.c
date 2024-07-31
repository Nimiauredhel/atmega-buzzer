#include <avr/io.h>

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))

typedef struct variableLengthAddress
{
    uint8_t addressLength;
    union address
    {
        volatile uint8_t *eight;
        volatile uint16_t *sixteen;
    } address;
} variableLengthAddress;

uint16_t VLA_Read(variableLengthAddress *target)
{
    switch (target->addressLength)
    {
        case 8:
            return *target->address.eight;
            break;
        case 16:
            return *target->address.sixteen;
            break;
        default:
            break;
    }
}

void VLA_Write(uint16_t value, variableLengthAddress *target)
{
    switch (target->addressLength)
    {
        case 8:
            *target->address.eight = value;
            break;
        case 16:
            *target->address.sixteen = value;
            break;
        default:
            break;
    }
}

typedef struct device
{
    variableLengthAddress pitch;
    variableLengthAddress width;

} device;

typedef struct channel
{
    device *device;
    // the number of pitches represented by this channel
    uint8_t currentPitchCount;
    // array of pitches represented by this channel
    uint8_t currentPitches[4];
    // index of next pitch to sound on this channel
    uint8_t nextPitchIndex;
    // current "tone" (voltage?) set on this channel
    uint8_t currentTone;
    uint8_t polyCycleThreshold;
    uint8_t polyCycleCounter;
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
    // the current position of the command sequence
    uint16_t sPosition;
    // the last position from which a jump was triggered
    uint16_t jPosition;
    // remaining track sleep time in microseconds
    uint64_t remainingSleepTime;
} track;

typedef void(*instrument)(channel *channel);
