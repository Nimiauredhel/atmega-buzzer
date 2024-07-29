#include "notes.c"
#include <avr/io.h>

// sequence encoding:
// first element: music event type (also implying length)
// 0: sleep, 1-4: select 1-4 pitches, 5: tone, 6: instrument index
// following elements are the function parameters

#define SLEEP(x) 0, x,
#define TONE(x) 5, x,
#define SILENCE(x) TONE(0) SLEEP(x)
#define PITCH(x) 1, x,
#define PITCH2(x, y) 2, x, y,
#define PITCH3(x, y, z) 3, x, y, z,
#define PITCH4(x, y, z, w) 4, x, y, z, w,
#define INSTRUMENT(x) 6, x,

#define QUIET 0
#define SQUARE 1
#define SINE 2

uint8_t sequence[] =
{
    INSTRUMENT(SINE)

    PITCH(Gb5) TONE(10) SLEEP(2) SILENCE(2)

    PITCH(B4) TONE(10) SLEEP(4)
    PITCH(D5) TONE(10) SLEEP(4)
    PITCH(Gb5) TONE(10) SLEEP(4)
    PITCH(B5) TONE(10) SLEEP(4)

    PITCH(G5) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)
    PITCH(Db5) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)

    PITCH(Bb4) TONE(10) SLEEP(4)
    PITCH(G5) TONE(10) SLEEP(4)
    PITCH(Gb5) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)
             
    PITCH(D5) TONE(10) SLEEP(4)
    PITCH(B4) TONE(10) SLEEP(4)
    PITCH(Gb4) TONE(10) SLEEP(4)
    PITCH(A4) TONE(10) SLEEP(4)

    PITCH(G4) TONE(10) SLEEP(4)
    PITCH(Gb5) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)
    PITCH(D5) TONE(10) SLEEP(4)
             
    PITCH(Db5) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)
    PITCH(G5) TONE(10) SLEEP(4)
    PITCH(B4) TONE(10) SLEEP(4)

    PITCH(Gb4) TONE(10) SLEEP(4)
    PITCH(Db5) TONE(10) SLEEP(4)
    PITCH(Gb4) TONE(10) SLEEP(4)
    PITCH(E4) TONE(10) SLEEP(4)
             
    PITCH(D4) TONE(10) SLEEP(4)
    PITCH(B4) TONE(10) SLEEP(4)
    PITCH(Db4) TONE(10) SLEEP(4)
    PITCH(Db5) TONE(10) SLEEP(4)

    // line 2

    PITCH(D5) TONE(10) SLEEP(4)
    PITCH(Gb4) TONE(10) SLEEP(4)
    PITCH(B3) TONE(10) SLEEP(4)
    PITCH(Db4) TONE(10) SLEEP(4)
             
    PITCH(D4) TONE(10) SLEEP(4)
    PITCH(E4) TONE(10) SLEEP(4)
    PITCH(Gb4) TONE(10) SLEEP(4)
    PITCH(G4) TONE(10) SLEEP(4)
             
    PITCH(A4) TONE(10) SLEEP(4)
    PITCH(D4) TONE(10) SLEEP(4)
    PITCH(Gb4) TONE(10) SLEEP(4)
    PITCH(A4) TONE(10) SLEEP(4)
             
    PITCH(D5) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)
    PITCH(Gb5) TONE(10) SLEEP(4)
    PITCH(A4) TONE(10) SLEEP(4)
             
    PITCH(B4) TONE(10) SLEEP(4)
    PITCH(G4) TONE(10) SLEEP(4)
    PITCH(B4) TONE(10) SLEEP(4)
    PITCH(D5) TONE(10) SLEEP(4)
             
    PITCH(G5) TONE(10) SLEEP(4)
    PITCH(B4) TONE(10) SLEEP(4)
    PITCH(G4) TONE(10) SLEEP(4)
    PITCH(E4) TONE(10) SLEEP(4)
             
    PITCH(Db4) TONE(10) SLEEP(4)
    PITCH(E4) TONE(10) SLEEP(4)
    PITCH(A4) TONE(10) SLEEP(4)
    PITCH(G5) TONE(10) SLEEP(4)
             
    PITCH(Gb5) TONE(10) SLEEP(4)
    PITCH(D4) TONE(10) SLEEP(4)
    PITCH(G4) TONE(10) SLEEP(4)
    PITCH(E5) TONE(10) SLEEP(4)
};
