#include <avr/io.h>
#include "notes.c"

// sequence encoding:
// first element: music event type (also implying length)
// 0: sleep, 1-4: select 1-4 pitches, 5: tone, 
// 6: instrument index 7: rhythm unit (tempo)
// following elements are the function parameters

#define SLEEP(x) 0, x,
#define TONE(x) 5, x,
#define SILENCE(x) TONE(0) SLEEP(x)
#define PITCH(x) 1, x,
#define PITCH2(x, y) 2, x, y,
#define PITCH3(x, y, z) 3, x, y, z,
#define PITCH4(x, y, z, w) 4, x, y, z, w,
#define INSTRUMENT(x) 6, x,
#define TEMPO(x) 7, x,
#define JUMPBACK(x) 8, x,

#define QUIET 0
#define SQUARE 1
#define SINE 2

uint32_t sequence[] =
{
    TEMPO(256)
                    INSTRUMENT(SQUARE)

            TONE(42)
    PITCH4(C4, F4, Ab4, C5)
                            SLEEP(8)
                            SILENCE(4)
            TONE(42)
    PITCH4(C4, F4, Ab4, C5)
                            SLEEP(8)
                            SILENCE(10)
            TONE(42)
    PITCH4(C4, F4, Ab4, C5)
                            SLEEP(8)
                            SILENCE(4)
            TONE(42)
    PITCH4(C4, F4, Ab4, C5)
                            SLEEP(8)
                            SILENCE(10)

            TONE(32)
    PITCH4(C4, F4, Ab4, C5)
                            SLEEP(18)
    PITCH4(D4, G4, Bb5, D5)
                            SLEEP(6)
            JUMPBACK(14)
    PITCH4(Eb4, Ab4, C5, Eb5)
                            SLEEP(18)
    PITCH4(D4, G4, Bb5, D5)
                            SLEEP(6)
    PITCH4(C4, F4, Ab4, C5)
                            SLEEP(24)

//
    PITCH4(F3, C4, Ab4, Ab4)
                            SLEEP(18)
    PITCH4(F3, Ab3, C4, F4)
                            SLEEP(6)
    PITCH4(G3, D4, G4, Bb4)
                            SLEEP(18)
    PITCH4(Ab3, C4, Eb4, Ab4)
                            SLEEP(6)
                            //
    PITCH4(Ab3, C4, Eb4, C5)
                            SLEEP(6)
    PITCH4(Ab3, C4, Eb4, Db5)
                            SLEEP(6)
    PITCH4(G3, Bb3, D4, Bb4)
                            SLEEP(9)
    PITCH4(G3, Bb3, D4, Ab4)
                            SLEEP(3)
    PITCH4(C3, F3, Ab3, C5)
                            SLEEP(24)
                            //
    PITCH4(Eb4, Ab4, B4, Eb5)
                            SLEEP(18)
    PITCH4(F4, Bb4, Db5, F5)
                            SLEEP(6)
            JUMPBACK(14)
    PITCH4(Gb4, B4, Eb5, Gb5)
                            SLEEP(18)
    PITCH4(Gb4, B4, Eb5, Ab5)
                            SLEEP(6)
    PITCH4(D4, F4, Bb4, Eb5)
                            SLEEP(12)
    PITCH4(D4, F4, Bb4, D5)
                            SLEEP(12)
                            //
    PITCH4(Eb4, Ab4, B4, Eb5)
                            SLEEP(18)
    PITCH(Ab4) 
    SLEEP(2) 
    PITCH(B4) 
    SLEEP(2) 
    PITCH(Eb5)
    SLEEP(2) 
    PITCH4(Db4, Ab4, Db5, Gb5)
                            SLEEP(6)
    PITCH4(Db4, Ab4, Db5, F5)
                            SLEEP(6)
    PITCH(Db5)
    SLEEP(6) 
    PITCH(Bb4) 
    SLEEP(6) 
    PITCH4(Eb4, Gb4, Bb5, Eb5)
                            SLEEP(24)
    PITCH4(Eb3, Gb4, Bb5, Eb5)
                            SLEEP(24)
};
