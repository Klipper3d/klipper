#include <string.h>     // memmove
#include "autoconf.h"   // CONFIG_SERIAL_BAUD
#include "board/gpio.h" // gpio_peripheral
#include "board/io.h"   // readl
#include "board/irq.h"  // irq_save
#include "board/misc.h" // console_sendf
#include "command.h"    // DECL_CONSTANT
#include "sched.h"      // DECL_INIT

//#include "pins_MKS.h"

#include <LPC17xx.h>
#include <lpc17xx_clkpwr.h>
#include <lpc17xx_uart.h>
#include <cmsis_nvic.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// UART0
// P0_02 = TXD0, FUNC_1
// P0_03 = RXD0, FUNC_1
// UART1
// P0_15 = TXD1, FUNC_1
// P0_16 = RXD1, FUNC_1
// UART2
// P0_10 = TXD2, FUNC_1
// P0_11 = RXD2, FUNC_1
// UART3
// P0_00 = TXD3, FUNC_2
// P0_01 = RXD3, FUNC_2
// ==== Pins config ====
_gpio_peripheral_t TXD = {0, 2, PINSEL_FUNC_1};
_gpio_peripheral_t RXD = {0, 3, PINSEL_FUNC_1};

static uint32_t initdone = 0;


//#define NO_DRIVER 0
#define NO_DRIVER 2

void calc_baudrate(uint32_t baud)
{
    uint32_t pclk, reg;

    /** Baud Rate Calculation :
        PCLKSELx registers contains the PCLK info for all the clock dependent peripherals.
        Bit6,Bit7 contains the Uart Clock(ie.UART_PCLK) information.
        The UART_PCLK and the actual Peripheral Clock(PCLK) is calculated as below.
        (Refer data sheet for more info)

        UART_PCLK    PCLK
        0x00       SystemFreq/4
        0x01       SystemFreq
        0x02       SystemFreq/2
        0x03       SystemFreq/8
    **/

    LPC_UART0->LCR |= UART_LCR_DLAB_EN;

    //pclk = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
    pclk = CLKPWR_GetPCLKSEL(CLKPWR_PCLKSEL_UART0);
    switch( pclk )
    {
        case 0x00:
            pclk = SystemCoreClock/4;
            break;
        case 0x01:
            pclk = SystemCoreClock;
            break;
        case 0x02:
            pclk = SystemCoreClock/2;
            break;
        case 0x03:
            pclk = SystemCoreClock/8;
            break;
    }

    reg = ( pclk / (16 * baud ));

    LPC_UART0->DLL  = (reg & 0xFF);
    LPC_UART0->DLM  = ((reg >> 8) & 0xFF);
    LPC_UART0->LCR &= ~(UART_LCR_DLAB_EN);
}


void serial_uart_init(void) {
    if (initdone) return;

    // Init pins
    gpio_peripheral(&TXD, 1);
    gpio_peripheral(&RXD, 1);

#if (NO_DRIVER == 1)
    CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCUART0, ENABLE);
    CLKPWR_SetPCLKDiv(CLKPWR_PCLKSEL_UART0, CLKPWR_PCLKSEL_CCLK_DIV_1);
    LPC_UART0->FCR = UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS;
    LPC_UART0->LCR = UART_LCR_WLEN8;
    calc_baudrate(CONFIG_SERIAL_BAUD);
    // enable TX
    LPC_UART0->->TER |= UART_TER_TXEN;

#else
    UART_CFG_Type initStruct;
    /* Initialize UART Configuration parameter structure to default state:
     * Baudrate = 9600bps
     * 8 data bit
     * 1 Stop bit
     * None parity
     */
    UART_ConfigStructInit(&initStruct);
    initStruct.Baud_rate = CONFIG_SERIAL_BAUD; // Set configured baudrate
    UART_Init(LPC_UART0, &initStruct);

    // Enable UART Transmit
    UART_TxCmd(LPC_UART0, ENABLE);

#endif

    initdone = 1;
}

void serial_uart_put(char const c) {
    if (initdone) {
#if (NO_DRIVER == 0)
        UART_SendByte(LPC_UART0, (uint8_t)c);
#else
        while( !(LPC_UART0->LSR & UART_LSR_THRE) );
        LPC_UART0->THR = (uint8_t)c;
#endif
    }
}

void serial_uart_puts(char const * const str) {
    if (initdone) {
#if (NO_DRIVER == 0)
        UART_Send(LPC_UART0, (uint8_t*)str, strlen(str), /*NONE_*/BLOCKING);
#else
        char * tmp = (char*)str;
        while(*tmp) serial_uart_put(*tmp++);
#endif
    }
}

uint8_t serial_uart_get(void) {
    if (initdone) {
        while( !(LPC_UART0->LSR & UART_LSR_RDR));
        return (uint8_t)LPC_UART0->RBR;
    }
    return 0;
}


void serial_uart_put_num(uint32_t n, uint8_t const base) {
    if (n) {
#if 1
        static char Representation[]= "0123456789ABCDEF";
        static char buffer[50]; // Enough space for base 2
        char *ptr = &buffer[49];
        *ptr = '\0';
        do
        {
            *--ptr = Representation[n % base];
            n /= base;
        } while (n != 0);
        serial_uart_puts(ptr);
#else
        unsigned char buf[8 * sizeof(long)]; // Enough space for base 2
        int8_t i = 0;
        while (n) {
            //buf[i++] = n % base;
            n /= base;
        }
        while (i--) {
            //serial_uart_put((char)(buf[i] + (buf[i] < 10 ? '0' : 'A' - 10)));
        }
#endif
    } else {
        serial_uart_put('0');
    }
}

void serial_uart_printf(char const * const format, ...)
{
    char *traverse;
    int i;
    unsigned int u;
    char *s;

    va_list arg;
    va_start(arg, format);

    //for(traverse = format; *traverse != '\0'; traverse++)
    for(traverse = (char*)format; *traverse != '\n'; traverse++)
    {
        while( *traverse != '%' )
        {
            serial_uart_put(*traverse);
            traverse++;
        }

        traverse++;

        switch (*traverse) {
            case 'c' : i = va_arg(arg,int); //Fetch char argument
                serial_uart_put(i);
                break;

            case 'd' : i = va_arg(arg,int); //Fetch Decimal/Integer argument
                if (i < 0)
                {
                    i = -i;
                    serial_uart_put('-');
                }
                serial_uart_put_num(i, 10);
                break;

            case 'u' : u = va_arg(arg,unsigned int); //Fetch Unsigned Integer argument
                serial_uart_put_num(u, 10);
                break;

            case 'o': u = va_arg(arg,unsigned int); //Fetch Octal representation
                serial_uart_put_num(u, 8);
                break;

            case 's': s = va_arg(arg,char *); //Fetch string
                serial_uart_puts(s);
                break;

            case 'X':
            case 'x': u = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                serial_uart_put('0');
                serial_uart_put('x');
                serial_uart_put_num(u, 16);
                break;
        }
    }

    va_end(arg);
    serial_uart_put('\n');
}

/*
char *convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;

    ptr = &buffer[49];
    *ptr = '\0';

    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);

    return(ptr);
}
*/



/*
void UART0_IRQHandler(void) {
    ;
}
*/
