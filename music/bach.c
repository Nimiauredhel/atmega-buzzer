#include "../notes.c"
#include <avr/io.h>

// sequence encoding:
// first element: music event type (also implying length)
// 0: sleep, 1-4: select 1-4 pitches, 5: tone, 
// 6: instrument index 7: rhythm unit (tempo)
// following elements are the function parameters

#define SLEEP(x) 0, x,
#define VOLUME(x) 5, x,
#define SILENCE(x) VOLUME(0) SLEEP(x)
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
TEMPO(6400)
VOLUME(1)
    INSTRUMENT(SINE)
SLEEP(2)
VOLUME(2)
SLEEP(2)
VOLUME(3)
SLEEP(2)
VOLUME(4)
SLEEP(2)
VOLUME(5)
    PITCH(Gb5)
SLEEP(1)
VOLUME(10)
SLEEP(1)
    INSTRUMENT(SQUARE)
VOLUME(15)
SLEEP(1)
VOLUME(20)
SLEEP(1)

TEMPO(4800)

VOLUME(21)
    PITCH(B4) SLEEP(4)
VOLUME(22)
    PITCH(D5) SLEEP(4)
VOLUME(23)
    PITCH(Gb5) SLEEP(4)
VOLUME(24)
    PITCH(B5) SLEEP(4)

VOLUME(25)
    PITCH(G5) SLEEP(4)
    PITCH(E5) SLEEP(4)
    PITCH(Db5) SLEEP(4)
    PITCH(E5) SLEEP(4)

    PITCH(Bb4) SLEEP(4)
    PITCH(G5) SLEEP(4)
    PITCH(Gb5) SLEEP(4)
    PITCH(E5) SLEEP(4)

    PITCH(D5) SLEEP(4)
    PITCH(B4) SLEEP(4)
    PITCH(Gb4) SLEEP(4)
    PITCH(A4) SLEEP(4)

    PITCH(G4) SLEEP(4)
    PITCH(Gb5) SLEEP(4)
    PITCH(E5) SLEEP(4)
    PITCH(D5) SLEEP(4)
    
    PITCH(Db5) SLEEP(4)
    PITCH(E5) SLEEP(4)
    PITCH(G5) SLEEP(4)
    PITCH(B4) SLEEP(4)

TEMPO(5600)

    PITCH(Gb4) SLEEP(4)
    PITCH(Db5) SLEEP(4)
    PITCH(Gb4) SLEEP(4)
    PITCH(E4) SLEEP(4)

TEMPO(4800)
VOLUME(10)
    PITCH(D4) SLEEP(4)
VOLUME(15)
    PITCH(B4) SLEEP(4)
VOLUME(20)
    PITCH(Db4) SLEEP(4)
VOLUME(25)
    PITCH(Db5) SLEEP(4)
 //   PITCH2(D4, B4) SLEEP(4)
 //   PITCH2(Db4, Db5) SLEEP(4)

    // line 2

VOLUME(30)
    PITCH(D5) SLEEP(4)
    PITCH(Gb4) SLEEP(4)
VOLUME(1)
    PITCH(B3) SLEEP(4)
VOLUME(2)
    PITCH(Db4) SLEEP(4)
             
VOLUME(3)
    PITCH(D4) SLEEP(4)
VOLUME(4)
    PITCH(E4) SLEEP(4)
VOLUME(5)
    PITCH(Gb4) SLEEP(4)
    PITCH(G4) SLEEP(4)
VOLUME(10)
    PITCH(A4) SLEEP(4)
    PITCH(D4) SLEEP(4)
    PITCH(Gb4) SLEEP(4)
    PITCH(A4) SLEEP(4)
             
    PITCH(D5) SLEEP(4)
    PITCH(E5) SLEEP(4)
    PITCH(Gb5) SLEEP(4)
    PITCH(A4) SLEEP(4)

    PITCH(B4) SLEEP(4)
    PITCH(G4) SLEEP(4)
    PITCH(B4) SLEEP(4)
    PITCH(D5) SLEEP(4)
             
    PITCH(G5) SLEEP(4)
    PITCH(B4) SLEEP(4)
    PITCH(G4) SLEEP(4)
    PITCH(E4) SLEEP(4)
             
TEMPO(5600)
VOLUME(20)
    PITCH(Db4) SLEEP(4)
    PITCH(E4) SLEEP(4)
    PITCH(A4) SLEEP(4)
    PITCH(G5) SLEEP(4)
             
TEMPO(6400)
    PITCH(Gb5) SLEEP(4)
    PITCH(D4) SLEEP(4)
    PITCH(G4) SLEEP(4)
    PITCH(E5) SLEEP(4)
};
