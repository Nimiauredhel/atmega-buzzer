#include "notes.c"
// sequence encoding:
// first element: music event type (also implying length)
// 0: playNote, 1: playNoteFluid, 2: playChord, 3: playChordFluid
// following elements are the function parameters
unsigned char sequence[] =
{
    3, 2, 12, 0, C3, F3, Ab3, C4, 1, 0,
    3, 16, 4, 0, D3, G3, Bb3, D4, -1, 0,
    3, 2, 12, 0, C3, F3, Ab3, C4, 1, 0,
    3, 16, 4, 0, D3, G3, Bb3, D4, -1, 0,
    3, 2, 12, 0, Eb3, Ab3, C4, Eb4, 1, 0,
    3, 16, 4, 0, D3, G3, Bb3, D4, -1, 0,
    3, 4, 8, 0, C3, F3, Ab3, C4, 1, 0,
    3, 16, 8, 0, C3, F3, Ab3, C4, -1, 0,

    3, 2, 12, 0, F3, C4, F4, Ab4, 1, 0,
    3, 16, 4, 0, F3, Ab3, C4, F4, 1, 0,
    3, 2, 12, 0, G3, D4, G4, Bb4, 1, 0,
    3, 16, 4, 0, Ab3, C4, Eb4, Ab4, -1, 0,
    3, 8, 4, 0, Ab3, C4, Eb4, C5, 0, 0,
    3, 8, 4, 0, Ab3, C4, Eb4, Db5, 0, 0,
    3, 8, 6, 0, G3, Bb3, D4, Bb4, -1, 0,
    3, 2, 2, 0, G3, Bb3, D4, Ab4, 1, 0,
    3, 4, 16, 0, C3, F3, Ab3, C5, 1, 0,

    3, 2, 12, 0, Eb3, Ab3, B3, Eb4, 1, 0,
    3, 16, 4, 0, F3, Bb3, Db4, F4, -1, 0,
    3, 2, 12, 0, Eb3, Ab3, B3, Eb4, 1, 0,
    3, 16, 4, 0, F3, Bb3, Db4, F4, -1, 0,
    3, 2, 12, 0, Gb3, B3, Eb4, Eb4, 1, 0,
    3, 16, 4, 0, Gb3, B3, Eb4, D4, -1, 0,
    3, 4, 8, 0, D3, F3, Bb3, Eb4, 1, 0,
    3, 16, 8, 0, D3, F3, Bb3, D4, -1, 0,

    3, 2, 12, 0, Eb3, Ab3, B3, Eb4, 1, 0,
    1, 2, 4, 0, Ab3, 1, 0,
    1, 2, 4, 0, B3, 1, 0,
    1, 2, 2, 2, Eb3, 1, 1,
    3, 4, 4, 0, Db3, Ab3, Db4, Gb4, 1, 0,
    3, 16, 4, 0, Db3, Ab3, Db4, F4, -1, 0,
    1, 4, 16, 0, Db4, 1, 0,
    1, 16, 15, 1, Bb3, -1, 0,
    3, 4, 16, 0, Eb3, Gb3, Bb4, Eb4, 1, 0,
    3, 48, 32, 0, Eb3, Gb3, Bb4, Eb4, -1, 0,
};
