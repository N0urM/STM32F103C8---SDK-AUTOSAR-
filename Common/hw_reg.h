/******************************************************/
/* Author    : Nourhan MAnsour                        */
/* Date      : 28/1/2021                              */
/* Version   : 1.0                                    */
/* File      : HW_reg.h                               */
/******************************************************/

#ifndef HW_REG_H
#define HW_REG_H

/*********************** PORT A **********************/
#define GPIOA_CRL           *((volatile uint32 *) 0x40010800)
#define GPIOA_CRH           *((volatile uint32 *) 0x40010804)
#define GPIOA_IDR           *((volatile uint32 *) 0x40010808)
#define GPIOA_ODR           *((volatile uint32 *) 0x4001080C)
#define GPIOA_BSRR          *((volatile uint32 *) 0x40010810)
#define GPIOA_BRR           *((volatile uint32 *) 0x40010814)
#define GPIOA_LCKR          *((volatile uint32 *) 0x40010818)

/*********************** PORT B **********************/
#define GPIOB_CRL           *((volatile uint32 *) 0x40010C00)
#define GPIOB_CRH           *((volatile uint32 *) 0x40010C04)
#define GPIOB_IDR           *((volatile uint32 *) 0x40010C08)
#define GPIOB_ODR           *((volatile uint32 *) 0x40010C0C)
#define GPIOB_BSRR          *((volatile uint32 *) 0x40010C10)
#define GPIOB_BRR           *((volatile uint32 *) 0x40010C14)
#define GPIOB_LCKR          *((volatile uint32 *) 0x40010C18)

/*********************** PORT C **********************/
#define GPIOC_CRL           *((volatile uint32 *) 0x40011000)
#define GPIOC_CRH           *((volatile uint32 *) 0x40011004)
#define GPIOC_IDR           *((volatile uint32 *) 0x40011008)
#define GPIOC_ODR           *((volatile uint32 *) 0x4001100C)
#define GPIOC_BSRR          *((volatile uint32 *) 0x40011010)
#define GPIOC_BRR           *((volatile uint32 *) 0x40011014)
#define GPIOC_LCKR          *((volatile uint32 *) 0x40011018)


/************************* SPI *************************/

#define SPI1_BASE           *((volatile uint32 *) 0x40013000)
#define SPI2_BASE           *((volatile uint32 *) 0x40003800)
#define SPI3_BASE           *((volatile uint32 *) 0x40013C00)

/********** Registers offsets ****************/
#define SPI_CR1_OFFSET          0x00
#define SPI_CR2_OFFSET          0x04
#define SPI_SR_OFFSET           0x08
#define SPI_DR_OFFSET           0x0C
#define SPI_CRCPR_OFFSET        0x10
#define SPI_RXCRCR_OFFSET       0x14
#define SPI_TXCRCR_OFFSET       0x18
#define SPI_I2SCFGR_OFFSET      0x1C
#define SPI_I2SPR_OFFSET        0x20

/********************** SPI1 Registers ***************/
#define SPI1_CR1               *((volatile uint32 *) (SPI1_BASE + SPI_CR1_OFFSET))
#define SPI1_CR2               *((volatile uint32 *) (SPI1_BASE + SPI_CR2_OFFSET))
#define SPI1_SR                *((volatile uint32 *) (SPI1_BASE + SPI_SR_OFFSET))
#define SPI1_DR                *((volatile uint32 *) (SPI1_BASE + SPI_DR_OFFSET))
#define SPI1_CRCPR             *((volatile uint32 *) (SPI1_BASE + SPI_CRCPR_OFFSET))
#define SPI1_RXCRCR            *((volatile uint32 *) (SPI1_BASE + SPI_RXCRCR_OFFSET))
#define SPI1_TXCRCR            *((volatile uint32 *) (SPI1_BASE + SPI_TXCRCR_OFFSET))
#define SPI1_I2SCFGR           *((volatile uint32 *) (SPI1_BASE + SPI_I2SCFGR_OFFSET))
#define SPI1_I2SPR             *((volatile uint32 *) (SPI1_BASE + SPI_I2SPR_OFFSET))

/********************** SPI2 Registers ***************/
#define SPI2_CR1               *((volatile uint32 *) (SPI2_BASE + SPI_CR1_OFFSET))
#define SPI2_CR2               *((volatile uint32 *) (SPI2_BASE + SPI_CR2_OFFSET))
#define SPI2_SR                *((volatile uint32 *) (SPI2_BASE + SPI_SR_OFFSET))
#define SPI2_DR                *((volatile uint32 *) (SPI2_BASE + SPI_DR_OFFSET))
#define SPI2_CRCPR             *((volatile uint32 *) (SPI2_BASE + SPI_CRCPR_OFFSET))
#define SPI2_RXCRCR            *((volatile uint32 *) (SPI2_BASE + SPI_RXCRCR_OFFSET))
#define SPI2_TXCRCR            *((volatile uint32 *) (SPI2_BASE + SPI_TXCRCR_OFFSET))
#define SPI2_I2SCFGR           *((volatile uint32 *) (SPI2_BASE + SPI_I2SCFGR_OFFSET))
#define SPI2_I2SPR             *((volatile uint32 *) (SPI2_BASE + SPI_I2SPR_OFFSET))

/******** Registers Bits *************/

#define SPI_CR1_CPHA        0
#define SPI_CR1_CPOL        1
#define SPI_CR1_MSTR        2
#define SPI_CR1_BR0         3
#define SPI_CR1_BR1         4
#define SPI_CR1_BR2         5
#define SPI_CR1_SPE         6
#define SPI_CR1_LSBF        7
#define SPI_CR1_SSI         8
#define SPI_CR1_SSM         9
#define SPI_CR1_RXONLY      10
#define SPI_CR1_DFF         11
#define SPI_CR1_CRCNXT      12
#define SPI_CR1_CRCEN       13
#define SPI_CR1_BIDIOE      14
#define SPI_CR1_BIDIMODE    15

/***************************************/

#define SPI_CR2_RXDMAEN     0
#define SPI_CR2_TXDMAEN     1
#define SPI_CR2_SSOE        2
#define SPI_CR2_ERRIE       5
#define SPI_CR2_RXNEIE      6
#define SPI_CR2_TXEIE       7

/***************************************/

#define SPI_SR_RXNE         0
#define SPI_SR_TXE          1
#define SPI_SR_CHSIDE       2
#define SPI_SR_UDR          3
#define SPI_SR_CRCERR       4
#define SPI_SR_MODF         5
#define SPI_SR_OVR          6
#define SPI_SR_BSY          7

#endif
