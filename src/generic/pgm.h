#ifndef __GENERIC_PGM_H
#define __GENERIC_PGM_H
// This header provides wrappers for the AVR specific "PROGMEM"
// declarations on non-avr platforms.

#define NEED_PROGMEM 0
#define PROGMEM
#define PSTR(S) S
#define READP(VAR) VAR
#define vsnprintf_P(D, S, F, A) vsnprintf(D, S, F, A)
#define strcasecmp_P(S1, S2) strcasecmp(S1, S2)
#define memcpy_P(DST, SRC, SIZE) memcpy((DST), (SRC), (SIZE))

#endif // pgm.h
