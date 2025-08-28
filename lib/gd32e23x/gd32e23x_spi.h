/*!
    \file    gd32e23x_spi.h
    \brief   definitions for the SPI

    \version 2019-02-19, V1.0.0, firmware for GD32E23x
    \version 2020-12-12, V1.1.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32E23X_SPI_H
#define GD32E23X_SPI_H

#include "gd32e23x.h"

/* SPIx(x=0,1) definitions */
#define SPI0                            (SPI_BASE + 0x0000F800U)
#define SPI1                            SPI_BASE

/* registers definitions */
#define SPI_CTL0(spix)                  REG32((spix) + 0x00000000U)             /*!< SPI control register 0 */
#define SPI_CTL1(spix)                  REG32((spix) + 0x00000004U)             /*!< SPI control register 1*/
#define SPI_STAT(spix)                  REG32((spix) + 0x00000008U)             /*!< SPI status register */
#define SPI_DATA(spix)                  REG32((spix) + 0x0000000CU)             /*!< SPI data register */
#define SPI_CRCPOLY(spix)               REG32((spix) + 0x00000010U)             /*!< SPI CRC polynomial register */
#define SPI_RCRC(spix)                  REG32((spix) + 0x00000014U)             /*!< SPI receive CRC register */
#define SPI_TCRC(spix)                  REG32((spix) + 0x00000018U)             /*!< SPI transmit CRC register */
#define SPI_I2SCTL(spix)                REG32((spix) + 0x0000001CU)             /*!< SPI I2S control register */
#define SPI_I2SPSC(spix)                REG32((spix) + 0x00000020U)             /*!< SPI I2S clock prescaler register */
#define SPI_QCTL(spix)                  REG32((spix) + 0x00000080U)             /*!< SPI quad mode control register(only available in SPI1) */

/* bits definitions */
/* SPI_CTL0 */
#define SPI_CTL0_CKPH                   BIT(0)                                  /*!< clock phase selection */
#define SPI_CTL0_CKPL                   BIT(1)                                  /*!< clock polarity selection */
#define SPI_CTL0_MSTMOD                 BIT(2)                                  /*!< master mode enable */
#define SPI_CTL0_PSC                    BITS(3,5)                               /*!< master clock prescaler selection */
#define SPI_CTL0_SPIEN                  BIT(6)                                  /*!< SPI enable*/
#define SPI_CTL0_LF                     BIT(7)                                  /*!< LSB first mode */
#define SPI_CTL0_SWNSS                  BIT(8)                                  /*!< NSS pin selection in NSS software mode */
#define SPI_CTL0_SWNSSEN                BIT(9)                                  /*!< NSS software mode selection */
#define SPI_CTL0_RO                     BIT(10)                                 /*!< receive only */
/* only available in SPI0*/
#define SPI_CTL0_FF16                   BIT(11)                                 /*!< data frame size */
/* only available in SPI1*/
#define SPI_CTL0_CRCL                   BIT(11)                                 /*!< CRC length */
#define SPI_CTL0_CRCNT                  BIT(12)                                 /*!< CRC next transfer */
#define SPI_CTL0_CRCEN                  BIT(13)                                 /*!< CRC calculation enable */
#define SPI_CTL0_BDOEN                  BIT(14)                                 /*!< bidirectional transmit output enable*/
#define SPI_CTL0_BDEN                   BIT(15)                                 /*!< bidirectional enable */

/* SPI_CTL1 */
#define SPI_CTL1_DMAREN                 BIT(0)                                  /*!< receive buffer DMA enable */
#define SPI_CTL1_DMATEN                 BIT(1)                                  /*!< transmit buffer DMA enable */
#define SPI_CTL1_NSSDRV                 BIT(2)                                  /*!< drive NSS output */
#define SPI_CTL1_NSSP                   BIT(3)                                  /*!< SPI NSS pulse mode enable */
#define SPI_CTL1_TMOD                   BIT(4)                                  /*!< SPI TI mode enable */
#define SPI_CTL1_ERRIE                  BIT(5)                                  /*!< errors interrupt enable */
#define SPI_CTL1_RBNEIE                 BIT(6)                                  /*!< receive buffer not empty interrupt enable */
#define SPI_CTL1_TBEIE                  BIT(7)                                  /*!< transmit buffer empty interrupt enable */
/* only available in SPI1 */
#define SPI_CTL1_DZ                     BITS(8,11)                              /*!< data size */
#define SPI_CTL1_BYTEN                  BIT(12)                                 /*!< byte access to FIFO enable */
#define SPI_CTL1_RXDMA_ODD              BIT(13)                                 /*!< odd bytes in RX DMA channel */
#define SPI_CTL1_TXDMA_ODD              BIT(14)                                 /*!< odd bytes in TX DMA channel */

/* SPI_STAT */
#define SPI_STAT_RBNE                   BIT(0)                                  /*!< receive buffer not empty */
#define SPI_STAT_TBE                    BIT(1)                                  /*!< transmit buffer empty */
#define SPI_STAT_I2SCH                  BIT(2)                                  /*!< I2S channel side */
#define SPI_STAT_TXURERR                BIT(3)                                  /*!< I2S transmission underrun error bit */
#define SPI_STAT_CRCERR                 BIT(4)                                  /*!< SPI CRC error bit */
#define SPI_STAT_CONFERR                BIT(5)                                  /*!< SPI configuration error bit */
#define SPI_STAT_RXORERR                BIT(6)                                  /*!< SPI reception overrun error bit */
#define SPI_STAT_TRANS                  BIT(7)                                  /*!< transmitting on-going bit */
#define SPI_STAT_FERR                   BIT(8)                                  /*!< format error bit */
/* only available in SPI1 */
#define SPI_STAT_RXLVL                  BITS(9,10)                              /*!< RXFIFO level */
#define SPI_STAT_TXLVL                  BITS(11,12)                             /*!< TXFIFO level */

/* SPI_DATA */
#define SPI_DATA_DATA                   BITS(0,15)                              /*!< data transfer register */

/* SPI_CRCPOLY */
#define SPI_CRCPOLY_CRCPOLY             BITS(0,15)                              /*!< CRC polynomial value */

/* SPI_RCRC */
#define SPI_RCRC_RCRC                   BITS(0,15)                              /*!< RX CRC value */

/* SPI_TCRC */
#define SPI_TCRC_TCRC                   BITS(0,15)                              /*!< TX CRC value */

/* SPI_I2SCTL */
#define SPI_I2SCTL_CHLEN                BIT(0)                                  /*!< channel length */
#define SPI_I2SCTL_DTLEN                BITS(1,2)                               /*!< data length */
#define SPI_I2SCTL_CKPL                 BIT(3)                                  /*!< idle state clock polarity */
#define SPI_I2SCTL_I2SSTD               BITS(4,5)                               /*!< I2S standard selection */
#define SPI_I2SCTL_PCMSMOD              BIT(7)                                  /*!< PCM frame synchronization mode */
#define SPI_I2SCTL_I2SOPMOD             BITS(8,9)                               /*!< I2S operation mode */
#define SPI_I2SCTL_I2SEN                BIT(10)                                 /*!< I2S enable */
#define SPI_I2SCTL_I2SSEL               BIT(11)                                 /*!< I2S mode selection */

/* SPI_I2SPSC */
#define SPI_I2SPSC_DIV                  BITS(0,7)                               /*!< dividing factor for the prescaler */
#define SPI_I2SPSC_OF                   BIT(8)                                  /*!< odd factor for the prescaler */
#define SPI_I2SPSC_MCKOEN               BIT(9)                                  /*!< I2S MCK output enable */

/* SPI_QCTL(only available in SPI1) */
#define SPI_QCTL_QMOD                   BIT(0)                                  /*!< quad-SPI mode enable */
#define SPI_QCTL_QRD                    BIT(1)                                  /*!< quad-SPI mode read select */
#define SPI_QCTL_IO23_DRV               BIT(2)                                  /*!< drive SPI_IO2 and SPI_IO3 enable */

/* constants definitions */
/* SPI and I2S parameter struct definitions */
typedef struct {
    uint32_t device_mode;                                                       /*!< SPI master or slave */
    uint32_t trans_mode;                                                        /*!< SPI transfer type */
    uint32_t frame_size;                                                        /*!< SPI frame size */
    uint32_t nss;                                                               /*!< SPI NSS control by handware or software */
    uint32_t endian;                                                            /*!< SPI big endian or little endian */
    uint32_t clock_polarity_phase;                                              /*!< SPI clock phase and polarity */
    uint32_t prescale;                                                          /*!< SPI prescaler factor */
} spi_parameter_struct;

/* SPI mode definitions */
#define SPI_MASTER                      (SPI_CTL0_MSTMOD | SPI_CTL0_SWNSS)      /*!< SPI as master */
#define SPI_SLAVE                       ((uint32_t)0x00000000U)                 /*!< SPI as slave */

/* SPI bidirectional transfer direction */
#define SPI_BIDIRECTIONAL_TRANSMIT      SPI_CTL0_BDOEN                          /*!< SPI work in transmit-only mode */
#define SPI_BIDIRECTIONAL_RECEIVE       (~SPI_CTL0_BDOEN)                       /*!< SPI work in receive-only mode */

/* SPI transmit type */
#define SPI_TRANSMODE_FULLDUPLEX        ((uint32_t)0x00000000U)                 /*!< SPI receive and send data at fullduplex communication */
#define SPI_TRANSMODE_RECEIVEONLY       SPI_CTL0_RO                             /*!< SPI only receive data */
#define SPI_TRANSMODE_BDRECEIVE         SPI_CTL0_BDEN                           /*!< bidirectional receive data */
#define SPI_TRANSMODE_BDTRANSMIT        (SPI_CTL0_BDEN | SPI_CTL0_BDOEN)        /*!< bidirectional transmit data*/

/* SPI NSS control mode */
#define SPI_NSS_SOFT                    SPI_CTL0_SWNSSEN                        /*!< SPI NSS control by sofrware */
#define SPI_NSS_HARD                    ((uint32_t)0x00000000U)                 /*!< SPI NSS control by hardware */

/* SPI transmit way */
#define SPI_ENDIAN_MSB                  ((uint32_t)0x00000000U)                 /*!< SPI transmit way is big endian: transmit MSB first */
#define SPI_ENDIAN_LSB                  SPI_CTL0_LF                             /*!< SPI transmit way is little endian: transmit LSB first */

/* SPI clock phase and polarity */
#define SPI_CK_PL_LOW_PH_1EDGE          ((uint32_t)0x00000000U)                 /*!< SPI clock polarity is low level and phase is first edge */
#define SPI_CK_PL_HIGH_PH_1EDGE         SPI_CTL0_CKPL                           /*!< SPI clock polarity is high level and phase is first edge */
#define SPI_CK_PL_LOW_PH_2EDGE          SPI_CTL0_CKPH                           /*!< SPI clock polarity is low level and phase is second edge */
#define SPI_CK_PL_HIGH_PH_2EDGE         (SPI_CTL0_CKPL | SPI_CTL0_CKPH)         /*!< SPI clock polarity is high level and phase is second edge */

/* SPI clock prescaler factor */
#define CTL0_PSC(regval)                (BITS(3,5) & ((uint32_t)(regval) << 3))
#define SPI_PSC_2                       CTL0_PSC(0)                             /*!< SPI clock prescaler factor is 2 */
#define SPI_PSC_4                       CTL0_PSC(1)                             /*!< SPI clock prescaler factor is 4 */
#define SPI_PSC_8                       CTL0_PSC(2)                             /*!< SPI clock prescaler factor is 8 */
#define SPI_PSC_16                      CTL0_PSC(3)                             /*!< SPI clock prescaler factor is 16 */
#define SPI_PSC_32                      CTL0_PSC(4)                             /*!< SPI clock prescaler factor is 32 */
#define SPI_PSC_64                      CTL0_PSC(5)                             /*!< SPI clock prescaler factor is 64 */
#define SPI_PSC_128                     CTL0_PSC(6)                             /*!< SPI clock prescaler factor is 128 */
#define SPI_PSC_256                     CTL0_PSC(7)                             /*!< SPI clock prescaler factor is 256 */

/* SPIx frame size */
#define CTL1_FRAMESIZE(regval)          (BITS(8,11) & ((uint32_t)(regval) << 8))
#define SPI_FRAMESIZE_4BIT              CTL1_FRAMESIZE(3)                       /*!< SPI frame size is 4 bits */
#define SPI_FRAMESIZE_5BIT              CTL1_FRAMESIZE(4)                       /*!< SPI frame size is 5 bits */
#define SPI_FRAMESIZE_6BIT              CTL1_FRAMESIZE(5)                       /*!< SPI frame size is 6 bits */
#define SPI_FRAMESIZE_7BIT              CTL1_FRAMESIZE(6)                       /*!< SPI frame size is 7 bits */
#define SPI_FRAMESIZE_8BIT              CTL1_FRAMESIZE(7)                       /*!< SPI frame size is 8 bits */
#define SPI_FRAMESIZE_9BIT              CTL1_FRAMESIZE(8)                       /*!< SPI frame size is 9 bits */
#define SPI_FRAMESIZE_10BIT             CTL1_FRAMESIZE(9)                       /*!< SPI frame size is 10 bits */
#define SPI_FRAMESIZE_11BIT             CTL1_FRAMESIZE(10)                      /*!< SPI frame size is 11 bits */
#define SPI_FRAMESIZE_12BIT             CTL1_FRAMESIZE(11)                      /*!< SPI frame size is 12 bits */
#define SPI_FRAMESIZE_13BIT             CTL1_FRAMESIZE(12)                      /*!< SPI frame size is 13 bits */
#define SPI_FRAMESIZE_14BIT             CTL1_FRAMESIZE(13)                      /*!< SPI frame size is 14 bits */
#define SPI_FRAMESIZE_15BIT             CTL1_FRAMESIZE(14)                      /*!< SPI frame size is 15 bits */
#define SPI_FRAMESIZE_16BIT             CTL1_FRAMESIZE(15)                      /*!< SPI frame size is 16 bits */

/* SPIx CRC length(x=1) */
#define SPI_CRC_8BIT                    ((uint32_t)0x00000000U)                 /*!< SPI CRC length is 8 bits */
#define SPI_CRC_16BIT                   SPI_CTL0_CRCL                           /*!< SPI CRC length is 16 bits */

/* SPIx byte access enable(x=1) */
#define SPI_HALFWORD_ACCESS             ((uint32_t)0x00000000U)                 /*!< SPI half-word access to FIFO */
#define SPI_BYTE_ACCESS                 SPI_CTL1_BYTEN                          /*!< SPI byte access to FIFO */

/* SPIx odd bytes in TX DMA channel(x=1) */
#define SPI_TXDMA_EVEN                  ((uint32_t)0x00000000U)                 /*!< SPI number of byte in TX DMA channel is even */
#define SPI_TXDMA_ODD                   SPI_CTL1_TXDMA_ODD                      /*!< SPI number of byte in TX DMA channel is odd */

/* SPIx odd bytes in RX DMA channel(x=1) */
#define SPI_RXDMA_EVEN                  ((uint32_t)0x00000000U)                 /*!< SPI number of byte in RX DMA channel is even */
#define SPI_RXDMA_ODD                   SPI_CTL1_RXDMA_ODD                      /*!< SPI number of byte in RX DMA channel is odd */

/* SPIx TXFIFO level(x=1) */
#define CTL1_TXLVL(regval)              (BITS(11,12) & ((uint32_t)(regval) << 11))
#define SPI_TXLVL_EMPTY                 CTL1_TXLVL(0)                           /*!< SPI TXFIFO is empty */
#define SPI_TXLVL_QUARTER_FULL          CTL1_TXLVL(1)                           /*!< SPI TXFIFO is a quarter of full */
#define SPI_TXLVL_HAlF_FULL             CTL1_TXLVL(2)                           /*!< SPI TXFIFO is a half of full */
#define SPI_TXLVL_FULL                  CTL1_TXLVL(3)                           /*!< SPI TXFIFO is full */

/* SPIx RXFIFO level(x=1) */
#define CTL1_RXLVL(regval)              (BITS(9,10) & ((uint32_t)(regval) << 9))
#define SPI_RXLVL_EMPTY                 CTL1_RXLVL(0)                           /*!< SPI RXFIFO is empty */
#define SPI_RXLVL_QUARTER_FULL          CTL1_RXLVL(1)                           /*!< SPI RXFIFO is a quarter of full */
#define SPI_RXLVL_HAlF_FULL             CTL1_RXLVL(2)                           /*!< SPI RXFIFO is a half of full */
#define SPI_RXLVL_FULL                  CTL1_RXLVL(3)                           /*!< SPI RXFIFO is full */

/* I2S audio sample rate */
#define I2S_AUDIOSAMPLE_8K              ((uint32_t)8000U)                       /*!< I2S audio sample rate is 8KHz */
#define I2S_AUDIOSAMPLE_11K             ((uint32_t)11025U)                      /*!< I2S audio sample rate is 11KHz */
#define I2S_AUDIOSAMPLE_16K             ((uint32_t)16000U)                      /*!< I2S audio sample rate is 16KHz */
#define I2S_AUDIOSAMPLE_22K             ((uint32_t)22050U)                      /*!< I2S audio sample rate is 22KHz */
#define I2S_AUDIOSAMPLE_32K             ((uint32_t)32000U)                      /*!< I2S audio sample rate is 32KHz */
#define I2S_AUDIOSAMPLE_44K             ((uint32_t)44100U)                      /*!< I2S audio sample rate is 44KHz */
#define I2S_AUDIOSAMPLE_48K             ((uint32_t)48000U)                      /*!< I2S audio sample rate is 48KHz */
#define I2S_AUDIOSAMPLE_96K             ((uint32_t)96000U)                      /*!< I2S audio sample rate is 96KHz */
#define I2S_AUDIOSAMPLE_192K            ((uint32_t)192000U)                     /*!< I2S audio sample rate is 192KHz */

/* I2S frame format */
#define I2SCTL_DTLEN(regval)            (BITS(1,2) & ((uint32_t)(regval) << 1))
#define I2S_FRAMEFORMAT_DT16B_CH16B     I2SCTL_DTLEN(0)                         /*!< I2S data length is 16 bit and channel length is 16 bit */
#define I2S_FRAMEFORMAT_DT16B_CH32B     (I2SCTL_DTLEN(0) | SPI_I2SCTL_CHLEN)    /*!< I2S data length is 16 bit and channel length is 32 bit */
#define I2S_FRAMEFORMAT_DT24B_CH32B     (I2SCTL_DTLEN(1) | SPI_I2SCTL_CHLEN)    /*!< I2S data length is 24 bit and channel length is 32 bit */
#define I2S_FRAMEFORMAT_DT32B_CH32B     (I2SCTL_DTLEN(2) | SPI_I2SCTL_CHLEN)    /*!< I2S data length is 32 bit and channel length is 32 bit */

/* I2S master clock output */
#define I2S_MCKOUT_DISABLE              ((uint32_t)0x00000000U)                 /*!< I2S master clock output disable */
#define I2S_MCKOUT_ENABLE               SPI_I2SPSC_MCKOEN                       /*!< I2S master clock output enable */

/* I2S operation mode */
#define I2SCTL_I2SOPMOD(regval)         (BITS(8,9) & ((uint32_t)(regval) << 8))
#define I2S_MODE_SLAVETX                I2SCTL_I2SOPMOD(0)                      /*!< I2S slave transmit mode */
#define I2S_MODE_SLAVERX                I2SCTL_I2SOPMOD(1)                      /*!< I2S slave receive mode */
#define I2S_MODE_MASTERTX               I2SCTL_I2SOPMOD(2)                      /*!< I2S master transmit mode */
#define I2S_MODE_MASTERRX               I2SCTL_I2SOPMOD(3)                      /*!< I2S master receive mode */

/* I2S standard */
#define I2SCTL_I2SSTD(regval)           (BITS(4,5) & ((uint32_t)(regval) << 4))
#define I2S_STD_PHILLIPS                I2SCTL_I2SSTD(0)                        /*!< I2S phillips standard */
#define I2S_STD_MSB                     I2SCTL_I2SSTD(1)                        /*!< I2S MSB standard */
#define I2S_STD_LSB                     I2SCTL_I2SSTD(2)                        /*!< I2S LSB standard */
#define I2S_STD_PCMSHORT                I2SCTL_I2SSTD(3)                        /*!< I2S PCM short standard */
#define I2S_STD_PCMLONG                 (I2SCTL_I2SSTD(3) | SPI_I2SCTL_PCMSMOD) /*!< I2S PCM long standard */

/* I2S clock polarity */
#define I2S_CKPL_LOW                    ((uint32_t)0x00000000U)                 /*!< I2S clock polarity low level */
#define I2S_CKPL_HIGH                   SPI_I2SCTL_CKPL                         /*!< I2S clock polarity high level */

/* SPI DMA constants definitions */
#define SPI_DMA_TRANSMIT                ((uint8_t)0x00U)                        /*!< SPI transmit data use DMA */
#define SPI_DMA_RECEIVE                 ((uint8_t)0x01U)                        /*!< SPI receive data use DMA */

/* SPI CRC constants definitions */
#define SPI_CRC_TX                      ((uint8_t)0x00U)                        /*!< SPI transmit CRC value */
#define SPI_CRC_RX                      ((uint8_t)0x01U)                        /*!< SPI receive CRC value */

/* SPI/I2S interrupt enable/disable constants definitions */
#define SPI_I2S_INT_TBE                 SPI_CTL1_TBEIE                          /*!< transmit buffer empty interrupt */
#define SPI_I2S_INT_RBNE                SPI_CTL1_RBNEIE                         /*!< receive buffer not empty interrupt */
#define SPI_I2S_INT_ERR                 SPI_CTL1_ERRIE                          /*!< error interrupt */

/* SPI/I2S interrupt flag constants definitions */
#define SPI_I2S_INT_FLAG_TBE            ((uint8_t)0x00U)                        /*!< transmit buffer empty interrupt flag */
#define SPI_I2S_INT_FLAG_RBNE           ((uint8_t)0x01U)                        /*!< receive buffer not empty interrupt flag */
#define SPI_I2S_INT_FLAG_RXORERR        ((uint8_t)0x02U)                        /*!< overrun interrupt flag */
#define SPI_INT_FLAG_CONFERR            ((uint8_t)0x03U)                        /*!< config error interrupt flag */
#define SPI_INT_FLAG_CRCERR             ((uint8_t)0x04U)                        /*!< CRC error interrupt flag */
#define I2S_INT_FLAG_TXURERR            ((uint8_t)0x05U)                        /*!< underrun error interrupt flag */
#define SPI_I2S_INT_FLAG_FERR           ((uint8_t)0x06U)                        /*!< format error interrupt flag */

/* SPI/I2S flag definitions */
#define SPI_FLAG_RBNE                   SPI_STAT_RBNE                           /*!< receive buffer not empty flag */
#define SPI_FLAG_TBE                    SPI_STAT_TBE                            /*!< transmit buffer empty flag */
#define SPI_FLAG_CRCERR                 SPI_STAT_CRCERR                         /*!< CRC error flag */
#define SPI_FLAG_CONFERR                SPI_STAT_CONFERR                        /*!< mode config error flag */
#define SPI_FLAG_RXORERR                SPI_STAT_RXORERR                        /*!< receive overrun error flag */
#define SPI_FLAG_TRANS                  SPI_STAT_TRANS                          /*!< transmit on-going flag */
#define SPI_FLAG_FERR                   SPI_STAT_FERR                           /*!< format error flag */
#define I2S_FLAG_RBNE                   SPI_STAT_RBNE                           /*!< receive buffer not empty flag */
#define I2S_FLAG_TBE                    SPI_STAT_TBE                            /*!< transmit buffer empty flag */
#define I2S_FLAG_CH                     SPI_STAT_I2SCH                          /*!< channel side flag */
#define I2S_FLAG_TXURERR                SPI_STAT_TXURERR                        /*!< underrun error flag */
#define I2S_FLAG_RXORERR                SPI_STAT_RXORERR                        /*!< overrun error flag */
#define I2S_FLAG_TRANS                  SPI_STAT_TRANS                          /*!< transmit on-going flag */
#define I2S_FLAG_FERR                   SPI_STAT_FERR                           /*!< format error flag */

/* function declarations */
/* SPI deinitialization and initialization functions */
/* reset SPI and I2S */
void spi_i2s_deinit(uint32_t spi_periph);
/* initialize the parameters of SPI structure with the default values */
void spi_struct_para_init(spi_parameter_struct *spi_struct);
/* initialize SPI parameters */
ErrStatus spi_init(uint32_t spi_periph, spi_parameter_struct *spi_struct);
/* enable SPI */
void spi_enable(uint32_t spi_periph);
/* disable SPI */
void spi_disable(uint32_t spi_periph);

/* I2S initialization functions */
/* initialize I2S parameters */
void i2s_init(uint32_t spi_periph, uint32_t mode, uint32_t standard, uint32_t ckpl);
/* configure I2S prescaler */
void i2s_psc_config(uint32_t spi_periph, uint32_t audiosample, uint32_t frameformat, uint32_t mckout);
/* enable I2S */
void i2s_enable(uint32_t spi_periph);
/* disable I2S */
void i2s_disable(uint32_t spi_periph);

/* NSS functions */
/* enable SPI NSS output */
void spi_nss_output_enable(uint32_t spi_periph);
/* disable SPI NSS output */
void spi_nss_output_disable(uint32_t spi_periph);
/* SPI NSS pin high level in software mode */
void spi_nss_internal_high(uint32_t spi_periph);
/* SPI NSS pin low level in software mode */
void spi_nss_internal_low(uint32_t spi_periph);

/* DMA functions */
/* enable SPI DMA */
void spi_dma_enable(uint32_t spi_periph, uint8_t dma);
/* disable SPI DMA */
void spi_dma_disable(uint32_t spi_periph, uint8_t dma);
/* configure SPI total number of data to be transmitted by DMA is odd or not */
void spi_transmit_odd_config(uint32_t spi_periph, uint16_t odd);
/* configure SPI total number of data to be received by DMA is odd or not */
void spi_receive_odd_config(uint32_t spi_periph, uint16_t odd);

/* communication functions */
/* configure SPI data frame format */
ErrStatus spi_i2s_data_frame_format_config(uint32_t spi_periph, uint16_t frame_format);
/* configure SPI access size to FIFO(8-bit or 16-bit) */
void spi_fifo_access_size_config(uint32_t spi_periph, uint16_t fifo_access_size);
/* configure SPI bidirectional transfer direction */
void spi_bidirectional_transfer_config(uint32_t spi_periph, uint32_t transfer_direction);
/* SPI transmit data */
void spi_i2s_data_transmit(uint32_t spi_periph, uint16_t data);
/* SPI receive data */
uint16_t spi_i2s_data_receive(uint32_t spi_periph);

/* SPI CRC functions */
/* set SPI CRC polynomial */
void spi_crc_polynomial_set(uint32_t spi_periph, uint16_t crc_poly);
/* get SPI CRC polynomial */
uint16_t spi_crc_polynomial_get(uint32_t spi_periph);
/* set CRC length */
void spi_crc_length_set(uint32_t spi_periph, uint16_t crc_length);
/* turn on SPI CRC function */
void spi_crc_on(uint32_t spi_periph);
/* turn off SPI CRC function */
void spi_crc_off(uint32_t spi_periph);
/* SPI next data is CRC value */
void spi_crc_next(uint32_t spi_periph);
/* get SPI CRC send value or receive value */
uint16_t spi_crc_get(uint32_t spi_periph, uint8_t crc);

/* SPI TI mode functions */
/* enable SPI TI mode */
void spi_ti_mode_enable(uint32_t spi_periph);
/* disable SPI TI mode */
void spi_ti_mode_disable(uint32_t spi_periph);

/* SPI NSS pulse mode functions */
/* enable SPI NSS pulse mode */
void spi_nssp_mode_enable(uint32_t spi_periph);
/* disable SPI NSS pulse mode */
void spi_nssp_mode_disable(uint32_t spi_periph);

/* quad wire SPI functions */
/* enable quad wire SPI */
void spi_quad_enable(uint32_t spi_periph);
/* disable quad wire SPI */
void spi_quad_disable(uint32_t spi_periph);
/* enable quad wire SPI write */
void spi_quad_write_enable(uint32_t spi_periph);
/* enable quad wire SPI read */
void spi_quad_read_enable(uint32_t spi_periph);
/* enable quad wire SPI_IO2 and SPI_IO3 pin output */
void spi_quad_io23_output_enable(uint32_t spi_periph);
/* disable quad wire SPI_IO2 and SPI_IO3 pin output */
void spi_quad_io23_output_disable(uint32_t spi_periph);

/* flag and interrupt functions */
/* get SPI and I2S flag status */
FlagStatus spi_i2s_flag_get(uint32_t spi_periph, uint32_t flag);
/* enable SPI and I2S interrupt */
void spi_i2s_interrupt_enable(uint32_t spi_periph, uint8_t interrupt);
/* disable SPI and I2S interrupt */
void spi_i2s_interrupt_disable(uint32_t spi_periph, uint8_t interrupt);
/* get SPI and I2S interrupt status */
FlagStatus spi_i2s_interrupt_flag_get(uint32_t spi_periph, uint8_t interrupt);
/* clear SPI CRC error flag status */
void spi_crc_error_clear(uint32_t spi_periph);

#endif /* GD32E23X_SPI_H */
