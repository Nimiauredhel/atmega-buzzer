#include <avr/io.h>
#include "../notes.c"

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

uint32_t sequenceTreble[] =
{
    TEMPO(256)
                    INSTRUMENT(SQUARE)
                    //
            VOLUME(32)
    PITCH3(F4, Ab4, C5)
                            SLEEP(18)
    PITCH3(G4, Bb5, D5)
                            SLEEP(6)
            JUMPBACK(14)
    PITCH3(Ab4, C5, Eb5)
                        SLEEP(18)
    PITCH3(G4, Bb5, D5)
                        SLEEP(6)
    PITCH3(F4, Ab4, C5)
                            SLEEP(24)

//
    PITCH3(C4, Ab4, Ab4)
                        SLEEP(18)
    PITCH3(Ab3, C4, F4)
                        SLEEP(6)
    PITCH3(D4, G4, Bb4)
                        SLEEP(18)
    PITCH3(C4, Eb4, Ab4)
                            SLEEP(6)
                            //
    PITCH3(C4, Eb4, C5)
                        SLEEP(6)
    PITCH3(C4, Eb4, Db5)
                        SLEEP(6)
    PITCH3(Bb3, D4, Bb4)
                        SLEEP(9)
    PITCH3(Bb3, D4, Ab4)
                        SLEEP(3)
    PITCH3(F3, Ab3, C5)
                            SLEEP(24)
                            //
    PITCH3(Ab4, B4, Eb5)
                        SLEEP(18)
    PITCH3(Bb4, Db5, F5)
                            SLEEP(6)
            JUMPBACK(12)
    PITCH3(B4, Eb5, Gb5)
                       SLEEP(18)
    PITCH3(B4, Eb5, Ab5)
                        SLEEP(6)
    PITCH3(F4, Bb4, Eb5)
                        SLEEP(12)
    PITCH3(F4, Bb4, D5)
                            SLEEP(12)
                            //
    PITCH3(Ab4, B4, Eb5)
                            SLEEP(18)
    PITCH(Ab4) 
    SLEEP(2) 
    PITCH(B4) 
    SLEEP(2) 
    PITCH(Eb5)
    SLEEP(2) 
    PITCH(Gb5)
             SLEEP(6)
    PITCH(F5)
            SLEEP(6)
    PITCH(Db5)
            SLEEP(6) 
    PITCH(Bb4) 
            SLEEP(6) 
    PITCH3(Gb4, Bb5, Eb5)
                            SLEEP(48)
};
uint32_t sequenceBass[] =
{
    TEMPO(256)
                    INSTRUMENT(SQUARE)
//
            VOLUME(1)
    PITCH(C3)
                            SLEEP(18)
    PITCH(D3)
                            SLEEP(6)
            JUMPBACK(8)
    PITCH(Eb3)
                            SLEEP(18)
    PITCH(D3)
                            SLEEP(6)
    PITCH(C3)
                            SLEEP(24)
//
    PITCH(F3)
                            SLEEP(24)
    PITCH(G3)
                            SLEEP(18)
    PITCH(Ab3)
                            SLEEP(6)
                            //
    PITCH(Ab3)
                            SLEEP(12)
    PITCH(G3)
                            SLEEP(12)
    PITCH(C3)
                            SLEEP(24)
                            //
    PITCH(Eb3)
                            SLEEP(18)
    PITCH(F3)
                            SLEEP(6)
            JUMPBACK(8)
    PITCH(Gb3)
                            SLEEP(24)
    PITCH(D3)
                            SLEEP(24)
                            //
    PITCH(Eb3)
                            SLEEP(18)
    PITCH(Ab3) 
    SLEEP(2) 
    PITCH(Ab4) 
    SLEEP(2) 
    PITCH(B4)
    SLEEP(2) 
    PITCH3(Db4, Ab4, Db5)
                            SLEEP(24)
    PITCH(Eb4)
                            SLEEP(24)
    PITCH(Eb3)
                            SLEEP(24)
};
