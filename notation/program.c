#include <avr/io.h>
#include <util/delay.h>

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))

#define RHYTHM_UNIT 54
#define RHYTHM_DELAY _delay_ms(RHYTHM_UNIT);

#define  C3 255
#define Db3 240
#define  D3 232
#define Eb3 218
#define  E3 205
#define  F3 192
#define Gb3 182
#define  G3 172
#define Ab3 163
#define  A3 154
#define Bb3 145
#define  B3 136
#define  C4 127
#define Db4 120
#define  D4 116
#define Eb4 109
#define  E4 102
#define  F4 96
#define Gb4 91
#define  G4 86
#define Ab4 81
#define  A4 77
#define Bb4 72
#define  B4 68
#define  C5 63
#define Db5 60
#define  D5 58
#define Eb5 54
#define  E5 51
#define  F5 48
#define Gb5 45
#define  G5 43
#define Ab5 40
#define  A5 38
#define Bb5 36
#define  B5 34
#define  C6 31

// sequence encoding:
// first element: music event type (also implying length)
// 0: playNote, 1: playNoteFluid, 2: playChord, 3: playChordFluid
// following elements are the function parameters
unsigned int position = 0;
unsigned char sequence[] =
{
    0, 0, 2, 2, Gb5,

    // line 1

    0, 3, 4, 0, B4,
    0, 1, 3, 1, D5,
    0, 1, 4, 0, Gb5,
    0, 0, 3, 1, B5,

    0, 3, 4, 0, G5, 
    0, 1, 3, 1, E5, 
    0, 1, 4, 0, Db5,
    0, 0, 3, 1, E5, 

    0, 3, 4, 0, Bb4,
    0, 1, 3, 1, G5, 
    0, 1, 4, 0, Gb5,
    0, 0, 3, 1, E5, 

    0, 3, 4, 0, D5, 
    0, 1, 3, 1, B4, 
    0, 1, 4, 0, Gb4,
    0, 0, 3, 1, A4, 

    0, 3, 4, 0, G4, 
    0, 1, 3, 1, Gb5,
    0, 1, 4, 0, E5, 
    0, 0, 3, 1, D5, 
                    
    0, 3, 4, 0, Db5,
    0, 1, 3, 1, E5, 
    0, 1, 4, 0, G5, 
    0, 0, 3, 1, B4, 
                    
    0, 3, 4, 0, Gb4,
    0, 1, 3, 1, Db5,
    0, 1, 4, 0, Gb4,
    0, 0, 3, 1, E4, 
                    
    0, 3, 4, 0, D4, 
    0, 1, 3, 1, B4, 
    0, 1, 4, 0, Db4,
    0, 0, 3, 1, Db5,

    // line 2
    
    0, 3, 4, 0, D5, 
    0, 1, 3, 1, Gb4,
    0, 1, 4, 0, B3, 
    0, 0, 3, 1, Db4,
                    
    0, 3, 4, 0, D4, 
    0, 1, 4, 0, E4, 
    0, 1, 4, 0, Gb4,
    0, 0, 3, 1, G4, 
                    
    0, 3, 4, 0, A4, 
    0, 1, 3, 1, D4, 
    0, 1, 4, 0, Gb4,
    0, 0, 3, 1, A4, 
                    
    0, 3, 4, 0, D5, 
    0, 1, 3, 1, E5, 
    0, 1, 4, 0, Gb5,
    0, 0, 3, 1, A4, 
                    
    0, 3, 4, 0, B4, 
    0, 1, 3, 1, G4, 
    0, 1, 4, 0, B4, 
    0, 0, 3, 1, D5, 
                    
    0, 3, 4, 0, G5, 
    0, 1, 3, 1, B4, 
    0, 1, 4, 0, G4, 
    0, 0, 3, 1, E4, 
                    
    0, 3, 4, 0, Db4,
    0, 1, 3, 1, E4, 
    0, 1, 4, 0, A4, 
    0, 0, 3, 1, G5, 
                    
    0, 3, 4, 0, Gb5,
    0, 1, 3, 1, D4, 
    0, 1, 4, 0, G4, 
    0, 0, 3, 1, E5, 
};

static void playNote(unsigned char tone, unsigned char pitch, unsigned char duration, unsigned char gap)
{
    OCR0B = tone;
    OCR0A = pitch;
    SET_BIT(PORTB, 5);

    for (int i = 0; i < duration; i++)
    {
        RHYTHM_DELAY
    }

    UNSET_BIT(PORTB, 5);

    for (int i = 0; i < gap; i++)
    {
        RHYTHM_DELAY
    }
}

static void playNoteFluid(unsigned char tone, unsigned char pitch, unsigned char duration, unsigned char gap, char toneDir, char pitchDir)
{
    OCR0B = tone;
    OCR0A = pitch;
    SET_BIT(PORTB, 5);

    for (int i = 0; i < duration; i++)
    {
        RHYTHM_DELAY
        OCR0B = tone + (toneDir*i);
        OCR0A = pitch + (pitchDir*i);
    }

    UNSET_BIT(PORTB, 5);

    for (int i = 0; i < gap; i++)
    {
        RHYTHM_DELAY
    }
}

static void playChord(unsigned char tone,  unsigned char pitch1,  unsigned char pitch2,  unsigned char pitch3, unsigned char pitch4, unsigned char duration, unsigned char gap)
{
    OCR0B = tone;
    OCR0A = pitch1;
    SET_BIT(PORTB, 5);

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

    UNSET_BIT(PORTB, 5);

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
    OCR0B = tone;
    OCR0A = pitch1;
    SET_BIT(PORTB, 5);

    for (int i = 0; i < duration; i++)
    {
        OCR0A = pitch1+pitchMod;
        RHYTHM_DELAY
        OCR0A = pitch2+pitchMod;
        RHYTHM_DELAY
        OCR0A = pitch3+pitchMod;
        RHYTHM_DELAY
        OCR0A = pitch4+pitchMod;
        RHYTHM_DELAY
        OCR0B = tone+(toneDir*i);
        pitchMod += pitchDir*i;
    }

    UNSET_BIT(PORTB, 5);

    for (int i = 0; i < gap; i++)
    {
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
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B= (1 << CS00) | (1 << CS02) | (1 << WGM02); 

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

