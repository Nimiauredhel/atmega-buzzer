#include "notes.c"
#include <avr/io.h>

#define RHYTHM_UNIT 42000

// sequence encoding:
// first element: music event type (also implying length)
// 0: sleep, 1-4: select 1-4 pitches, 5: tone, 6: instrument index
// following elements are the function parameters
uint32_t sequence[] =
{
    6, 1, // set instrument to 1 (regular)

    //start playing

    1, Gb4, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*2, // note duration

    5, 0, // set tone 0 - silence
    0, RHYTHM_UNIT*2, // silent gap

    1, B3, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, D3, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, Gb3, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, B4, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, G4, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, E4, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, Db4, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    1, E4, // set pitch
    5, 10, // set tone
    0, RHYTHM_UNIT*4,

    0, 10000000, // initial sleep
/*
    // line 1

    0, 3, 4, 0, B3,
    0, 1, 3, 1, D3,
    0, 1, 4, 0, Gb4,
    0, 1, 3, 1, B4,

    0, 1, 4, 0, G4, 
    0, 1, 3, 1, E4, 
    0, 1, 4, 0, Db4,
    0, 1, 3, 1, E4, 

    0, 1, 4, 0, Bb3,
    0, 1, 3, 1, G4, 
    0, 1, 4, 0, Gb4,
    0, 1, 3, 1, E4, 
              
    0, 1, 4, 0, D4, 
    0, 1, 3, 1, B3, 
    0, 1, 4, 0, Gb3,
    0, 1, 3, 1, A3, 

    0, 2, 4, 0, G3, 
    0, 1, 3, 1, Gb4,
    0, 1, 4, 0, E4, 
    0, 1, 3, 1, D4, 
                    
    0, 1, 4, 0, Db4,
    0, 1, 3, 1, E4, 
    0, 1, 4, 0, G4, 
    0, 1, 3, 1, B3, 
                    
    0, 3, 4, 0, Gb3,
    0, 1, 3, 1, Db4,
    0, 2, 4, 0, Gb3,
    0, 1, 3, 1, E3, 
                    
    0, 4, 4, 0, D3, 
    0, 1, 3, 1, B3, 
    0, 3, 4, 0, Db3,
    0, 1, 3, 1, Db4,

    // line 2
    
    0, 5, 4, 0, D4, 
    0, 1, 3, 1, Gb3,
    0, 2, 4, 0, B3, 
    0, 3, 3, 1, Db3,
                    
    0, 2, 4, 0, D3, 
    0, 1, 3, 1, E3, 
    0, 1, 4, 0, Gb3,
    0, 1, 3, 1, G3, 
                    
    0, 3, 4, 0, A3, 
    0, 1, 3, 1, D3, 
    0, 1, 4, 0, Gb3,
    0, 1, 3, 1, A3, 
                    
    0, 2, 4, 0, D4, 
    0, 1, 3, 1, E4, 
    0, 1, 4, 0, Gb4,
    0, 1, 3, 1, A3, 
                    
    0, 3, 4, 0, B3, 
    0, 1, 3, 1, G3, 
    0, 1, 4, 0, B3, 
    0, 1, 3, 1, D4, 
                    
    0, 2, 4, 0, G4, 
    0, 1, 3, 1, B3, 
    0, 1, 4, 0, G3, 
    0, 1, 3, 1, E3, 
                    
    0, 3, 4, 0, Db3,
    0, 1, 3, 1, E3, 
    0, 1, 4, 0, A3, 
    0, 1, 3, 1, G4, 
                    
    0, 2, 4, 0, Gb4,
    0, 1, 3, 1, D3, 
    0, 1, 4, 0, G3, 
    0, 1, 3, 1, E4, 
    */
};
