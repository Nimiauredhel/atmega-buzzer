// sequence encoding:
// first element: music event type (also implying length)
// 0: sleep, 1-4: select 1-4 pitches, 5: tone, 
// 6: instrument index 7: rhythm unit (tempo)
// 8: jump x cells back
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
