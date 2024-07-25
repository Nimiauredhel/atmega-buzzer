#include <avr/io.h>
#include <util/delay.h>

#define GET_BIT(REG, BIT) (REG & (1 << BIT))
#define SET_BIT(REG, BIT) (REG |= (1 << BIT))
#define UNSET_BIT(REG, BIT) (REG &= ~(1 << BIT))

#define RHYTHM_UNIT 48
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

#define PLAY_NOTE(tone, pitch, duration, gap) {\
    OCR0B = tone;\
    OCR0A = pitch;\
    SET_BIT(PORTB, 5);\
    for (int i = 0; i < duration; i++)\
        RHYTHM_DELAY\
    UNSET_BIT(PORTB, 5);\
    for (int i = 0; i < gap; i++)\
        RHYTHM_DELAY\
}

#define PLAY_NOTE_FLUID(tone, pitch, duration, gap, toneDir, pitchDir) {\
    OCR0B = tone;\
    OCR0A = pitch;\
    SET_BIT(PORTB, 5);\
    for (int i = 0; i < duration; i++)\
    {\
        RHYTHM_DELAY\
        OCR0B = tone + (toneDir*i);\
        OCR0A = pitch + (pitchDir*i);\
    }\
    UNSET_BIT(PORTB, 5);\
    for (int i = 0; i < gap; i++)\
        RHYTHM_DELAY\
}

#define PLAY_CHORD(tone, pitch1, pitch2, pitch3, pitch4, duration, gap) {\
    OCR0B = tone;\
    OCR0A = pitch1;\
    SET_BIT(PORTB, 5);\
    for (int i = 0; i < duration; i++)\
    {\
        OCR0A = pitch1;\
        RHYTHM_DELAY\
        OCR0A = pitch2;\
        RHYTHM_DELAY\
        OCR0A = pitch3;\
        RHYTHM_DELAY\
        OCR0A = pitch4;\
        RHYTHM_DELAY\
    }\
    UNSET_BIT(PORTB, 5);\
    for (int i = 0; i < gap; i++)\
    {\
        RHYTHM_DELAY\
        RHYTHM_DELAY\
        RHYTHM_DELAY\
        RHYTHM_DELAY\
    }\
}

#define PLAY_CHORD_FLUID(tone, pitch1, pitch2, pitch3, pitch4, duration, gap, toneDir, pitchDir) {\
    unsigned char pitchMod = 0;\
    OCR0B = tone;\
    OCR0A = pitch1;\
    SET_BIT(PORTB, 5);\
    for (int i = 0; i < duration; i++)\
    {\
        OCR0A = pitch1+pitchMod;\
        RHYTHM_DELAY\
        OCR0A = pitch2+pitchMod;\
        RHYTHM_DELAY\
        OCR0A = pitch3+pitchMod;\
        RHYTHM_DELAY\
        OCR0A = pitch4+pitchMod;\
        RHYTHM_DELAY\
        OCR0B = tone+(toneDir*i);\
        pitchMod += pitchDir*i;\
    }\
    UNSET_BIT(PORTB, 5);\
    for (int i = 0; i < gap; i++)\
        RHYTHM_DELAY\
}

int main(void)
{
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B= (1 << CS02) | (1 << WGM02); 

    SET_BIT(DDRD, DDD5);
    SET_BIT(DDRB, DDB5);

    SET_BIT(PORTB, 5);

    // opening sfx
    PLAY_NOTE_FLUID(4, C3, 14, 2, 1, -4);

    // lost elf b section
    for (int i = 0; i < 2; i++)
    {
        PLAY_CHORD_FLUID(2, C3, F3, Ab3, C4,   12, 0, 1, 0);
        PLAY_CHORD_FLUID(16, D3, G3, Bb3, D4,   4, 0, -1, 0);
    }

    PLAY_CHORD_FLUID(2, Eb3, Ab3, C4, Eb4, 12, 0, 1, 0);
    PLAY_CHORD_FLUID(16, D3, G3, Bb3, D4,   4, 0, -1, 0);
    PLAY_CHORD_FLUID(4, C3, F3, Ab3, C4,   8, 0, 1, 0);
    PLAY_CHORD_FLUID(16, C3, F3, Ab3, C4,   8, 0, -1, 0);

    PLAY_CHORD_FLUID(2, F3, C4, F4, Ab4,   12, 0, 1, 0);
    PLAY_CHORD_FLUID(16, F3, Ab3, C4, F4,   4, 0, 1, 0);
    PLAY_CHORD_FLUID(2, G3, D4, G4, Bb4,   12, 0, 1, 0);
    PLAY_CHORD_FLUID(16, Ab3, C4, Eb4, Ab4,   4, 0, -1, 0);
    PLAY_CHORD_FLUID(8, Ab3, C4, Eb4, C5, 4, 0, 0, 0);
    PLAY_CHORD_FLUID(8, Ab3, C4, Eb4, Db5, 4, 0, 0, 0);
    PLAY_CHORD_FLUID(8, G3, Bb3, D4, Bb4, 6, 0, -1, 0);
    PLAY_CHORD_FLUID(2, G3, Bb3, D4, Ab4, 2, 0, 1, 0);
    PLAY_CHORD_FLUID(4, C3, F3, Ab3, C5,   16, 0, 1, 0);

    PLAY_CHORD_FLUID(2, Eb3, Ab3, B3, Eb4,   12, 0, 1, 0);
    PLAY_CHORD_FLUID(16,Eb3, Bb3, Db4, F4,   4, 0, -1, 0);
    PLAY_CHORD_FLUID(2, Eb3, Ab3, B3, Eb4,   12, 0, 1, 0);
    PLAY_CHORD_FLUID(16,Eb3, Bb3, Db4, F4,   4, 0, -1, 0);
    PLAY_CHORD_FLUID(2, Gb3, B3, Eb4, Gb4, 12, 0, 1, 0);
    PLAY_CHORD_FLUID(16,Gb3, B3, Eb4, Ab4,   4, 0, -1, 0);
    PLAY_CHORD_FLUID(4, D3, F3, Bb3, Eb4,   8, 0, 1, 0);
    PLAY_CHORD_FLUID(16,D3, F3, Bb3, D4,   8, 0, -1, 0);

    PLAY_CHORD_FLUID(2, Eb3, Ab3, B3, Eb4,   12, 0, 1, 0);
    PLAY_NOTE_FLUID(2, Ab3, 4, 0, 1, 0);
    PLAY_NOTE_FLUID(2, B3, 4, 0, 1, 0);
    PLAY_NOTE_FLUID(2, Eb3, 2, 2, 1, 1);
    PLAY_CHORD_FLUID(4, Db3, Ab3, Db4, Gb4, 4, 0, 2, 0);
    PLAY_CHORD_FLUID(16, Db3, Ab3, Db4, F4,  4, 0, -2, 0);
    PLAY_NOTE_FLUID(4, Db4, 16, 0, 0, 0);
    PLAY_NOTE_FLUID(16, Bb3, 16, 0, 0 ,0);
    PLAY_CHORD_FLUID(4, Eb3, Gb3, Bb3, Eb4,  16, 0, 4, 0);
    PLAY_CHORD_FLUID(48, Eb3, Gb3, Bb3, Eb4,  32, 0, 0, 0);

    // cybat intro

    for (int i = 0; i < 2; i++)
    {
        PLAY_NOTE_FLUID(50, C3, 3, 9,  -2, -5);
        PLAY_NOTE_FLUID(50, C3, 3, 1,  -2, -5);
        PLAY_NOTE_FLUID(50, C3, 3, 5,  -2, -5);
        PLAY_NOTE_FLUID(50, C3, 3, 9,  -2, -5);
        PLAY_NOTE_FLUID(50, C3, 3, 1,  -2, -5);
        PLAY_NOTE_FLUID(50, C3, 3, 5,  -2, -5);
        PLAY_NOTE_FLUID(50, C3, 3, 13, -2, -5);
    }

    for(;;)
    {

        // cybat
        PLAY_NOTE(4, A3, 6, 2);
        PLAY_NOTE(2, B3, 4, 0);
        PLAY_NOTE_FLUID(3, Ab3, 4, 8, 8, 0);
        PLAY_NOTE(2, Ab3-2, 4, 4);
        PLAY_NOTE(2, G3, 2, 2);
        PLAY_NOTE(3, G3, 2, 2);
        PLAY_NOTE(4, G3, 6, 2);
        PLAY_NOTE(3, Gb3, 8, 0);
        PLAY_NOTE_FLUID(2, Gb3, 8, 0, 0, -4);

        PLAY_NOTE(4, G3, 6, 2);
        PLAY_NOTE(2, A3, 4, 0);
        PLAY_NOTE_FLUID(3, Gb3, 4, 8, 8, 0);
        PLAY_NOTE(2, Gb3-2, 4, 4);
        PLAY_NOTE(2, F3, 2, 2);
        PLAY_NOTE(3, F3, 2, 2);
        PLAY_NOTE(4, F3, 6, 2);
        PLAY_NOTE(3, E3, 8, 0);
        PLAY_NOTE_FLUID(2, E3, 8, 0, 0, 8);

        PLAY_NOTE(4, A4, 6, 2);
        PLAY_NOTE(2, B4, 4, 0);
        PLAY_NOTE_FLUID(3, Ab4, 4, 8, 8, 0);
        PLAY_NOTE(2, Ab4-2, 4, 4);
        PLAY_NOTE(2, G4, 2, 2);
        PLAY_NOTE(3, G4, 2, 2);
        PLAY_NOTE(4, G4, 6, 2);
        PLAY_NOTE(3, Gb4, 8, 0);
        PLAY_NOTE_FLUID(2, Gb4, 8, 0, 0, -4);

        PLAY_NOTE(4, G4, 6, 2);
        PLAY_NOTE(2, A4, 4, 0);
        PLAY_NOTE_FLUID(3, Gb4, 4, 8, 8, 0);
        PLAY_NOTE(2, Gb4-2, 4, 4);
        PLAY_NOTE(2, F4, 2, 2);
        PLAY_NOTE(3, F4, 2, 2);
        PLAY_NOTE(4, F4, 6, 2);
        PLAY_NOTE(3, E4, 4, 0);
        PLAY_NOTE_FLUID(2, E4, 12, 0, 0, 4);
    }
}
