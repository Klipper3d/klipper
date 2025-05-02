#ifndef __LGT_PINDEIFNE_H
#define __LGT_PINDEIFNE_H

#define PMCR    _SFR_MEM8(0xF2)
#define PINE    _SFR_IO8(0x0C)
#define RCMCAL  (*((volatile unsigned char *)0x66))
#endif
