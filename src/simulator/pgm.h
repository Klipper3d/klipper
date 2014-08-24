#ifndef __SIMU_PGM_H
#define __SIMU_PGM_H
// This header provides wrappers for the AVR specific "PROGMEM"
// declarations.

#define PROGMEM
#define PSTR(S) S
#define READP(VAR) VAR
#define vsnprintf_P(D, S, F, A) vsnprintf(D, S, F, A)
#define strcasecmp_P(S1, S2) strcasecmp(S1, S2)

#endif // pgm.h
