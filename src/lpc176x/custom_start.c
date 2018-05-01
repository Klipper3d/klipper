#include "mempool.h"
#include <mpu.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#define MRI_SEMIHOST_STDIO 0
#define __debugbreak()
#define __mriNewlib_SemihostWrite(x,y,z)   0
#define __mriNewlib_SemihostRead(x,y,z)    0

/****************************************************************
 * startup init functions
 ****************************************************************/

#define STACK_SIZE  3072

POOL * _AHB0 = NULL;
POOL * _AHB1 = NULL;

extern unsigned int __bss_start__;
extern unsigned int __bss_end__;
extern unsigned int __StackTop;
extern unsigned int __end__;

extern uint8_t __AHB0_block_start;
extern uint8_t __AHB0_dyn_start;
extern uint8_t __AHB0_end;
extern uint8_t __AHB1_block_start;
extern uint8_t __AHB1_dyn_start;
extern uint8_t __AHB1_end;

static void fillUnusedRAM(void);
static void configureStackSizeLimit(unsigned int stackSizeLimit);
static unsigned int alignTo32Bytes(unsigned int value);
static void configureMpuToCatchStackOverflowIntoHeap(unsigned int maximumHeapAddress);

unsigned int g_maximumHeapAddress;


void exit (int status) {
    (void)status;
    while(1);
}
void abort(void)
{
    exit(1);
}

int main(void);
void __libc_init_array(void);
void _start(void)
{
    size_t bssSize = (uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__;
    int mainReturnValue;

    memset(&__bss_start__, 0, bssSize);
    fillUnusedRAM();

    if (STACK_SIZE) {
        configureStackSizeLimit(STACK_SIZE);
    }

    // zero the data sections in AHB0 and AHB1
    //memset(&__AHB0_block_start, 0, &__AHB0_dyn_start - &__AHB0_block_start);
    //memset(&__AHB1_block_start, 0, &__AHB1_dyn_start - &__AHB1_block_start);

    // MemoryPool stuff - needs to be initialised before __libc_init_array
    // so static ctors can use them
    //_AHB0 = pool_create(&__AHB0_dyn_start, ((uintptr_t)&__AHB0_end - (uintptr_t)&__AHB0_dyn_start));
    //_AHB1 = pool_create(&__AHB1_dyn_start, ((uintptr_t)&__AHB1_end - (uintptr_t)&__AHB1_dyn_start));

    __libc_init_array();
    mainReturnValue = main();
    exit(mainReturnValue);
    exit(0);
}

static __attribute__((naked)) void fillUnusedRAM(void)
{
    __asm (
        ".syntax unified\n"
        ".thumb\n"
        // Fill 2 words (8 bytes) at a time with 0xdeadbeef.
        " ldr   r2, =__FillStart\n"
        " movw  r0, #0xbeef\n"
        " movt  r0, #0xdead\n"
        " mov   r1, r0\n"
        // Don't fill past current stack pointer value.
        " mov   r3, sp\n"
        " bics  r3, r3, #7\n"
        "1$:\n"
        " strd  r0, r1, [r2], #8\n"
        " cmp   r2, r3\n"
        " blo   1$\n"
        " bx    lr\n"
    );
}

void configureStackSizeLimit(unsigned int stackSizeLimit)
{
    // Note: 32 bytes are reserved to fall between top of heap and top of stack for minimum MPU guard region.
    g_maximumHeapAddress = alignTo32Bytes((unsigned int)&__StackTop - stackSizeLimit - 32);
    configureMpuToCatchStackOverflowIntoHeap(g_maximumHeapAddress);
}

unsigned int alignTo32Bytes(unsigned int value)
{
    return (value + 31) & ~31;
}

void configureMpuToCatchStackOverflowIntoHeap(unsigned int maximumHeapAddress)
{
#define MPU_REGION_SIZE_OF_32_BYTES ((5-1) << MPU_RASR_SIZE_SHIFT)  // 2^5 = 32 bytes.

    prepareToAccessMPURegion(getHighestMPUDataRegionIndex());
    setMPURegionAddress(maximumHeapAddress);
    setMPURegionAttributeAndSize(MPU_REGION_SIZE_OF_32_BYTES | MPU_RASR_ENABLE);
    enableMPUWithDefaultMemoryMap();
}

/* Turn off the errno macro and use actual external global variable instead. */
#undef errno
extern int errno;

int doesHeapCollideWithStack(unsigned int newHeap)
{
    return ((newHeap >= __get_MSP()) ||
            (STACK_SIZE && newHeap >= g_maximumHeapAddress));
}

/* Dynamic memory allocation related syscalls. */
caddr_t _sbrk(int incr)
{
    static unsigned char *heap = (unsigned char *)&__end__;
    unsigned char        *prev_heap = heap;
    unsigned char        *new_heap = heap + incr;

    if (doesHeapCollideWithStack((unsigned int)new_heap)) {
        //errno = ENOMEM;
        return (caddr_t) - 1;
    }

    heap = new_heap;
    return (caddr_t) prev_heap;
}
