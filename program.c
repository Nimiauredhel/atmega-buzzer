#include <avr/io.h>
#include <util/delay.h>
#include "notes.c"
//#include "bach.c"
#include "lostelf.c"

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))

#define RHYTHM_UNIT 48
#define RHYTHM_DELAY _delay_ms(RHYTHM_UNIT);

unsigned int position = 0;

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

static void playNote(unsigned char tone, unsigned char pitch, unsigned char duration, unsigned char gap)
{
    OCR0B = tone;
    OCR0A = pitch;
    pwmStart();

    for (int i = 0; i < duration; i++)
    {
        RHYTHM_DELAY
    }

    pwmStop();

    for (int i = 0; i < gap; i++)
    {
        RHYTHM_DELAY
    }
}

static void playNoteFluid(unsigned char tone, unsigned char pitch, unsigned char duration, unsigned char gap, char toneDir, char pitchDir)
{
    OCR0B = tone;
    OCR0A = pitch;

    pwmStart();

    for (int i = 0; i < duration; i++)
    {
        RHYTHM_DELAY
        OCR0B = tone + (toneDir*i);
        OCR0A = pitch + (pitchDir*i);
    }

    pwmStop();

    for (int i = 0; i < gap; i++)
    {
        RHYTHM_DELAY
    }
}

static void playChord(unsigned char tone,  unsigned char pitch1,  unsigned char pitch2,  unsigned char pitch3, unsigned char pitch4, unsigned char duration, unsigned char gap)
{
    OCR0B = tone;
    OCR0A = pitch1;

    pwmStart();

    for (int i = 0; i < duration; i++)
    {
        OCR0A = pitch1;
        RHYTHM_DELAY
        OCR0A = pitch2;
        RHYTHM_DELAY
        OCR0A = pitch3;
        RHYTHM_DELAY
        OCR0A = pitch4;
        RHYTHM_DELAY
    }

    pwmStop();

    for (int i = 0; i < gap; i++)
    {
        RHYTHM_DELAY
        RHYTHM_DELAY
        RHYTHM_DELAY
        RHYTHM_DELAY
    }
}

static void playChordFluid(unsigned char tone,  unsigned char pitch1,  unsigned char pitch2,  unsigned char pitch3, unsigned char pitch4, unsigned char duration, unsigned char gap, char toneDir, char pitchDir)
{
    unsigned char pitchMod = 0;
    unsigned char toneMod = 0;
    OCR0B = tone;
    OCR0A = pitch1;

    pwmStart();

    for (int i = 0; i < duration; i++)
    {
        OCR0B = tone+toneMod;
        OCR0A = pitch1+pitchMod;
        RHYTHM_DELAY
        OCR0A = pitch2+pitchMod;
        RHYTHM_DELAY
        OCR0A = pitch3+pitchMod;
        RHYTHM_DELAY
        OCR0A = pitch4+pitchMod;
        RHYTHM_DELAY
        toneMod += toneDir;
        pitchMod += pitchDir;
    }

    pwmStop();

    for (int i = 0; i < gap; i++)
    {
        RHYTHM_DELAY
        RHYTHM_DELAY
        RHYTHM_DELAY
        RHYTHM_DELAY
    }
}

static void play()
{
    switch (sequence[position]) 
    {
        case 0:
            playNote(sequence[position+1], sequence[position+4], sequence[position+2], sequence[position+3]);
            position += 5;
            break;
        case 1:
            playNoteFluid(sequence[position+1], sequence[position+4], sequence[position+2], sequence[position+3], sequence[position+5], sequence[position+6]);
            position += 7;
            break;
        case 2:
            playChord(sequence[position+1], sequence[position+4], sequence[position+5], sequence[position+6], sequence[position+7], sequence[position+2], sequence[position+3]);
            position += 8;
            break;
        case 3:
            playChordFluid(sequence[position+1], sequence[position+4], sequence[position+5], sequence[position+6], sequence[position+7], sequence[position+2], sequence[position+3], sequence[position+8], sequence[position+9]);
            position += 10;
            break;
    }
}

int main(void)
{
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);
    TCCR0B = (1 << WGM02); 

    SET_BIT(DDRD, DDD5);
    SET_BIT(DDRB, DDB5);

    SET_BIT(PORTB, 5);

    unsigned int sequenceLength = sizeof(sequence) / sizeof(sequence[0]);

    for(;;)
    {
        play();
        if (position >= sequenceLength) position = 0;
    }
}
