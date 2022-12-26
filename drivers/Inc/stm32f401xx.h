/*
 * stm32f401xx.h
 *
 *  Created on: Dec 16, 2022
 *      Author: r.freitas
 */

#ifndef INC_STM32F401XX_H_
#define INC_STM32F401XX_H_

#include <stddef.h> // to include macro definitions such as NULL
#include <stdint.h>
//#include <stdio.h>

/********************* [START] Processor Specific Details ************/
/*
 *   ARM Cortex M4 Processor NVIC ISERx register addresses
 */
#define NVIC_ISER0		((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1		((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2		((volatile uint32_t*)0xE000E108)
/*
 *   ARM Cortex M4 Processor NVIC ICERx register addresses
 */
#define NVIC_ICER0		((volatile uint32_t*)0xE000E180)
#define NVIC_ICER1		((volatile uint32_t*)0xE000E184)
#define NVIC_ICER2		((volatile uint32_t*)0xE000E188)
/*
 *   ARM Cortex M4 Processor Priority register addresses (calc)
 */
#define NVIC_PR_BASE_ADDR	((volatile uint32_t*)0xE000E400)
/*
 *   ARM Cortex M4 Processor number of Priority bits implemented in Priority register
 */
#define NO_PR_BITS_IMPLEMENTED 4
/********************* [END] Processor Specific Details ************/

// Some generic macros
#define ENABLE 				1
#define DISABLE 			0
#define SET 				ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET 		SET
#define GPIO_PIN_RESET		RESET
#define FLAG_RESET			RESET
#define FLAG_SET			SET

// Bit position definitions of SPI CR1 peripheral
#define SPI_CR1_CPHA		0	//bit field of CPHA in SPI_CR1 register
#define SPI_CR1_CPOL		1	//bit field of CPOL in SPI_CR1 register
#define SPI_CR1_MSTR		2	//bit field for Master selection
#define SPI_CR1_BR			3	//starts in 3 - BR [2:0] = bit fields 3 to 5 (Baud rate)
#define SPI_CR1_SPE			6	//bit field for SPI enable
#define SPI_CR1_LSBFIRST	7	//bit field for Frame format
#define SPI_CR1_SSI			8	//bit field for Internal slave select
#define SPI_CR1_SSM			9	//bit field for Software slave management
#define SPI_CR1_RXONLY		10	//bit field for Receive only
#define SPI_CR1_DFF			11	//bit field for Data frame format
#define SPI_CR1_CRCNEXT		12	//bit field for	CRC transfer next
#define SPI_CR1_CRCEN		13	//bit field for Hardware CRC calculation enable
#define SPI_CR1_BIDIOE		14	//bit field for Output enable in bidirectional mode
#define SPI_CR1_BIDIMODE	15	//bit field for Bidirectional data mode enable
// Bit position definitions of SPI CR2 peripheral
#define SPI_CR2_RXDMAEN 	0	//bit field for RX buffer DMA enable
#define SPI_CR2_TXDMAEN 	1	//bit field for TX buffer DMA enable
#define SPI_CR2_SSOE 		2	//bit field for SS output enable
#define SPI_CR2_FRF 		4	//bit field for Frame format
#define SPI_CR2_ERRIE 		5	//bit field for Error interrupt enable
#define SPI_CR2_RXNEIE		6	//bit field for RX buffer not empty interrupt enable
#define SPI_CR2_TXEIE 		7	//bit field for TX buffer empty interrupt enable
// Bit position definitions of SPI SR peripheral
#define SPI_SR_RXNE			0	//bit field for Receive buffer not empty
#define SPI_SR_TXE			1	//bit field for Transmit buffer empty
#define SPI_SR_CHSIDE		2	//bit field for Channel side
#define SPI_SR_UDR			3	//bit field for Underrun flag
#define SPI_SR_CRCERR		4	//bit field for CRC error flag
#define SPI_SR_MODF			5	//bit field for Mode fault
#define SPI_SR_OVR			6	//bit field for Overrun flag
#define SPI_SR_BSY			7	//bit field for Busy flag
#define SPI_SR_FRE			8	//bit field for Frame format error

// Bit position definitions of I2C CR1 peripheral
#define I2C_CR1_PE			0	//bit field for Peripheral enable
#define I2C_CR1_NOSTRETCH	7	//bit field for Clock stretching disable (Slave mode)
#define I2C_CR1_START		8	//bit field for Start generation
#define I2C_CR1_STOP		9	//bit field for Stop generation
#define I2C_CR1_ACK			10	//bit field for Acknowledge enable
#define I2C_CR1_SWRST		15	//bit field for Software reset

// Bit position definitions of I2C CR2 peripheral
#define I2C_CR2_FREQ		0	//bit field for Peripheral clock frequency FREQ[5:0]
#define I2C_CR2_ITERREN		8	//bit field for Error interrupt enable
#define I2C_CR2_ITEVTEN		9	//bit field for Event interrupt enable
#define I2C_CR2_ITBUFEN		10	//bit field for Buffer interrupt enable

// Bit position definitions of I2C OAR1 peripheral
#define I2C_OAR1_ADD0		0	//bit field for Interface address
#define I2C_OAR1_ADD71		1	//bit field for Interface address - ADD[7:1]
#define I2C_OAR1_ADD98		8	//bit field for Interface address - ADD[9:8]
#define I2C_OAR1_ADDMODE	15	//bit field for Addressing mode (slave mode)

// Bit position definitions of I2C SR1 peripheral
#define I2C_SR1_SB			0	//bit field for Start bit (Master mode)
#define I2C_SR1_ADDR		1	//bit field for Address sent (master mode)/matched (slave mode)
#define I2C_SR1_BTF			2	//bit field for Byte transfer finished
#define I2C_SR1_ADD10		3	//bit field for 10-bit header sent (Master mode)
#define I2C_SR1_STOPF		4	//bit field for Stop detection (slave mode)
#define I2C_SR1_RXNE		6	//bit field for Data register not empty (receivers)
#define I2C_SR1_TXE			7	//bit field for Data register empty (transmitters)
#define I2C_SR1_BERR		8	//bit field for Bus erroR
#define I2C_SR1_ARLO		9	//bit field for Arbitration lost (master mode)
#define I2C_SR1_AF			10	//bit field for Acknowledge failure
#define I2C_SR1_OVR			11	//bit field for Overrun/Underrun
#define I2C_SR1_TIMEOUT		14	//bit field for Timeout or Tlow error

// Bit position definitions of I2C SR2 peripheral
#define I2C_SR2_MSL			0	//bit field for Master 1/slave 0
#define I2C_SR2_BUSY		1	//bit field for Bus busy
#define I2C_SR2_TRA			2	//bit field for Transmitter/receiver
#define I2C_SR2_GENCALL		4	//bit field for General call address (Slave mode)
#define I2C_SR2_DUALF		7	//bit field for Dual flag (Slave mode)

// Bit position definitions of I2C CCR peripheral
#define I2C_CCR_CCR			0	//bit field for Clock control register in Fm/Sm mode (Master mode) CCR[11:0]
#define I2C_CCR_DUTY		14	//bit field for Fm mode duty cycle
#define I2C_CCR_FS			15	//bit field for I2C master mode selection

/*
    Base addresses of Flash and SRAM memories
*/
#define FLASH_BASEADDR	0x08000000UL //typecast also would work (uint32_t) isntead of UL
#define SRAM1_BASEADDR	0x20000000UL
#define SRAM SRAM1_BASEADDR
// #define SRAM2_BASEADDR	// uC doen't have SRAM2
#define ROM_BASEADDR	0x1FFF0000UL // ROM = System memory

/*
    Base addresses of AHBx and APBx Bus Peripheral
*/
#define PERIPH_BASE	0x40000000UL
#define APB1PERIPH_BASE PERIPH_BASE
#define APB2PERIPH_BASE 0x40010000UL
#define AHB1PERIPH_BASE 0x40020000UL
#define AHB2PERIPH_BASE 0x50000000UL

/*
    Base addresses of peripherals which are hanging on AHB2 bus
*/
#define USB_OTG_FS_BASEADDR (AHB2PERIPH_BASE + 0x0000)

/*
    Base addresses of peripherals which are hanging on AHB1 bus
*/
#define GPIOA_BASEADDR 		(AHB1PERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR 		(AHB1PERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR 		(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR 		(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR 		(AHB1PERIPH_BASE + 0x1000)
#define GPIOH_BASEADDR 		(AHB1PERIPH_BASE + 0x1C00)
#define CRC_BASEADDR 		(AHB1PERIPH_BASE + 0x3000)
#define RCC_BASEADDR 		(AHB1PERIPH_BASE + 0x3800)
#define FIR_BASEADDR 		(AHB1PERIPH_BASE + 0x3C00)
#define DMA1_BASEADDR 		(AHB1PERIPH_BASE + 0x6000)
#define DMA2_BASEADDR 		(AHB1PERIPH_BASE + 0x6400)
//#define USB_OTG_FS_BASEADDR (AHB1PERIPH_BASE + 0xFFE0000)

/*
    Base addresses of peripherals which are hanging on APB2 bus
*/
#define TIM1_BASEADDR 		(APB2PERIPH_BASE + 0x0000)
#define USART1_BASEADDR 	(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR 	(APB2PERIPH_BASE + 0x1400)
#define ADC1_BASEADDR 		(APB2PERIPH_BASE + 0x2000)
#define SDIO_BASEADDR 		(APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASEADDR 		(APB2PERIPH_BASE + 0x3000)
#define SPI4_BASEADDR 		(APB2PERIPH_BASE + 0x3400)
#define SYSCFG_BASEADDR 	(APB2PERIPH_BASE + 0x3800)
#define EXTI_BASEADDR 		(APB2PERIPH_BASE + 0x3C00)
#define TIM9_BASEADDR 		(APB2PERIPH_BASE + 0x4000)
#define TIM10_BASEADDR 		(APB2PERIPH_BASE + 0x4400)
#define TIM11_BASEADDR 		(APB2PERIPH_BASE + 0x4800)

/*
    Base addresses of peripherals which are hanging on APB1 bus
*/
#define TIM2_BASEADDR 		(APB1PERIPH_BASE + 0x0000)
#define TIM3_BASEADDR 		(APB1PERIPH_BASE + 0x0400)
#define TIM4_BASEADDR 		(APB1PERIPH_BASE + 0x0800)
#define TIM5_BASEADDR 		(APB1PERIPH_BASE + 0x0C00)
#define RTC_n_BKP_BASEADDR 	(APB1PERIPH_BASE + 0x2800)
#define WWDG_BASEADDR 		(APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASEADDR 		(APB1PERIPH_BASE + 0x3000)
#define I2S2ext_BASEADDR 	(APB1PERIPH_BASE + 0x3400)
#define SPI2_BASEADDR 		(APB1PERIPH_BASE + 0x3800) // SPI2 and I2S2 has same base addres
#define I2S2_BASEADDR 		(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR 		(APB1PERIPH_BASE + 0x3C00) // SPI3 and I2S3 has same base addres
#define I2S3_BASEADDR 		(APB1PERIPH_BASE + 0x3C00)
#define I2S3ext_BASEADDR 	(APB1PERIPH_BASE + 0x4000)
#define USART2_BASEADDR		(APB1PERIPH_BASE + 0x4400)
#define I2C1_BASEADDR 		(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR 		(APB1PERIPH_BASE + 0X5800)
#define I2C3_BASEADDR 		(APB1PERIPH_BASE + 0x5C00)
#define PWR_BASEADDR 		(APB1PERIPH_BASE + 0x7000)


/*
    Peripheral register definition structure for EXTI
*/
typedef struct{
	volatile uint32_t IMR; // Address offset: 0x00
	volatile uint32_t EMR; // Address offset: 0x04
	volatile uint32_t RTSR; // Address offset: 0x08
	volatile uint32_t FTSR; // Address offset: 0x0C
	volatile uint32_t SWIER; // Address offset: 0x10
	volatile uint32_t PR; // Address offset: 0x14
} EXTI_RegDef_t;

/*
    Peripheral register definition structure for SYSCFG
*/
typedef struct{
	volatile uint32_t MEMRMP; // Address offset: 0x00
	volatile uint32_t PMC; // Address offset: 0x04
	volatile uint32_t EXTICR[4]; // Address offset: 0x08~0x14
	//SYSCFG_EXTICR decides from which port the EXT will be used
	//by defaut it is considered port A
	//SYSCFG_EXTICR4 -> EXT13[3:0] -> used to configur PC13 -> 0b0010 PC[x] pin
	//SYSCFG_EXTICR4 = EXTICR[3]
	uint32_t RESERVED1[2]; // Address offset: 0x08~0x1C
	volatile uint32_t CMPCR; // Address offset: 0x20
	uint32_t RESERVED2[2]; // Address offset: 0x28
	volatile uint32_t CFGR; // Address offset: 0x2C
} SYSCFG_RegDef_t;

/*
    Peripheral register definition structure for RCC
*/
typedef struct {
	volatile uint32_t CR; // RCC clock control register - Address offset: 0x00
	volatile uint32_t PLLCFGR; // RCC PLL configuration register - Address offset: 0x04
	volatile uint32_t CFGR; // RCC clock configuration register - Address offset: 0x08
	volatile uint32_t CIR; // RCC clock interrupt register - Address offset: 0x0C
	volatile uint32_t AHB1RSTR; // RCC AHB1 peripheral reset register - Address offset: 0x10
	volatile uint32_t AHB2RSTR; // RCC AHB2 peripheral reset register - Address offset: 0x14
	volatile uint32_t Reserved0; // Reserved - Address offset: 0x18
	volatile uint32_t Reserved1; // Reserved - Address offset: 0x1C
	volatile uint32_t APB1RSTR; // RCC APB1 peripheral reset register - Address offset: 0x20
	volatile uint32_t APB2RSTR; // RCC APB2 peripheral reset register - Address offset: 0x24
	volatile uint32_t Reserved2; // Reserved - Address offset: 0x28
	volatile uint32_t Reserved3; // Reserved - Address offset: 0x2C
	volatile uint32_t AHB1ENR; // RCC AHB1 peripheral clock enable register - Address offset: 0x30
	volatile uint32_t AHB2ENR; // RCC AHB2 peripheral clock enable register - Address offset: 0x34
	volatile uint32_t Reserved4; // Reserved - Address offset: 0x38
	volatile uint32_t Reserved5; // Reserved - Address offset: 0x3C
	volatile uint32_t APB1ENR; // RCC APB1 peripheral clock enable register - Address offset: 0x40
	volatile uint32_t APB2ENR; // RCC APB2 peripheral clock enable register - Address offset: 0x44
	volatile uint32_t Reserved6; // Reserved - Address offset: 0x48
	volatile uint32_t Reserved7; // Reserved - Address offset: 0x4C
	volatile uint32_t AHB1LPENR; // RCC AHB1 peripheral clock enable in low power mode register - Address offset: 0x50
	volatile uint32_t AHB2LPENR; // RCC AHB2 peripheral clock enable in low power mode register - Address offset: 0x54
	volatile uint32_t Reserved8; // RCC - Address offset: 0x58
	volatile uint32_t Reserved9; // RCC - Address offset: 0x5C
	volatile uint32_t APB1LPENR; // RCC APB1 peripheral clock enable in low power mode register - Address offset: 0x60
	volatile uint32_t APB2LPENR; // RCC APB2 peripheral clock enable in low power mode register - Address offset: 0x64
	volatile uint32_t Reserved10; // Reserved - Address offset: 0x68
	volatile uint32_t Reserved11; // Reserved - Address offset: 0x6C
	volatile uint32_t BDCR; // RCC Backup domain control register - Address offset: 0x70
	volatile uint32_t CSR; // RCC clock control & status register - Address offset: 0x74
	volatile uint32_t Reserved12; // Reserved - Address offset: 0x78
	volatile uint32_t Reserved13; // Reserved - Address offset: 0x7C
	volatile uint32_t SSCGR; // RCC spread spectrum clock generation register - Address offset: 0x80
	volatile uint32_t PLLI2SCFGR; // RCC PLLI2S configuration register - Address offset: 0x84
	volatile uint32_t Reserved14; // Reserved - Address offset: 0x88
	volatile uint32_t DCKCFGR; // RCC  Dedicated Clocks Configuration Register - Address offset: 0x8C
} RCC_RegDef_t;

typedef struct {
	volatile uint32_t CR1; // SPI control register 1 - Address offset: 0x00
	volatile uint32_t CR2; // SPI control register 2 - Address offset: 0x04
	volatile uint32_t SR; // SPI status register - Address offset: 0x08
	volatile uint32_t DR; // SPI data register - Address offset: 0x0C
	volatile uint32_t CRCPR; // SPI CRC polynomial register - Address offset: 0x10
	volatile uint32_t RXCRCR; // SPI RX CRC register - Address offset: 0x14
	volatile uint32_t TXCRCR; // SPI TX CRC register - Address offset: 0x18
	volatile uint32_t I2SCFGR; // SPI I2S configuration register - Address offset: 0x1C
	volatile uint32_t I2SPR; // SPI I2S prescaler register - Address offset: 0x20
} SPI_RegDef_t;

typedef struct {
	volatile uint32_t CR1; // I2C control register 1 - Address offset: 0x00
	volatile uint32_t CR2; // I2C control register 2 - Address offset: 0x04
	volatile uint32_t OAR1; // I2C Own address register 1 - Address offset: 0x08
	volatile uint32_t OAR2; // I2C Own address register 2 - Address offset: 0x0C
	volatile uint32_t DR; // I2C Data register - Address offset: 0x10
	volatile uint32_t SR1; // I2C Status register 1 - Address offset: 0x14
	volatile uint32_t SR2; // I2C Status register 2 - Address offset: 0x18
	volatile uint32_t CCR; // I2C Clock control register - Address offset: 0x1C
	volatile uint32_t TRISE; // I2C Max Rise Time register - Address offset: 0x20
	volatile uint32_t FLTR; // I2C Filter register - Address offset: 0x24
} I2C_RegDef_t;

/*
    Peripheral definitions (Peripheral base addresses typecasted to xxx_RegDef_t)
*/
#define GPIOA 		((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB		((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 		((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 		((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 		((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOH 		((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC			((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI		((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG		((SYSCFG_RegDef_t*)SYSCFG_BASEADDR) //check if it is done

#define SPI1		((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2		((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3		((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4		((SPI_RegDef_t*)SPI4_BASEADDR)

#define I2C1		((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2		((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3		((I2C_RegDef_t*)I2C3_BASEADDR)

/*
    This macro returns a code (between 0 to 5) for given GPIO base address (x)
*/
//if its A, else B (C conditional operator)  - this comment in next line bugged my macro
#define GPIO_BASEADDR_TO_CODE(x)   ((x == GPIOA)?0:\
									(x == GPIOB)?1:\
									(x == GPIOC)?2:\
									(x == GPIOD)?3:\
									(x == GPIOE)?4:\
									(x == GPIOH)?5:0)

// Generic IRQ number macros 		Table 38. Vector table for STM32F401xB/CSTM32F401xD/E
// Interrupt Request Number for STM32F401xx MCU
#define IRQ_NO_WWDG 				0
#define IRQ_NO_EXTI16 				1
#define IRQ_NO_PVD 					1
#define IRQ_NO_EXTI21 				2
#define IRQ_NO_TAMP_STAMP			2
#define IRQ_NO_EXTI22 				3
#define IRQ_NO_RTC_WKUP				3
#define IRQ_NO_FLASH 				4
#define IRQ_NO_RCC	 				5
#define IRQ_NO_EXTI0 				6
#define IRQ_NO_EXTI1 				7
#define IRQ_NO_EXTI2 				8
#define IRQ_NO_EXTI3 				9
#define IRQ_NO_EXTI4 				10
#define IRQ_NO_DMA1_STREAM0			11
#define IRQ_NO_DMA1_STREAM1			12
#define IRQ_NO_DMA1_STREAM2			13
#define IRQ_NO_DMA1_STREAM3			14
#define IRQ_NO_DMA1_STREAM4			15
#define IRQ_NO_DMA1_STREAM5			16
#define IRQ_NO_DMA1_STREAM6			17
#define IRQ_NO_ADC					18
#define IRQ_NO_EXTI9_5 				23
#define IRQ_NO_TIM1_BRK_TIM9		24
#define IRQ_NO_TIM1_UP_TIM10		25
#define IRQ_NO_TIM1_TRG_COM_TIM11	26
#define IRQ_NO_TIM1_CC	 			27
#define IRQ_NO_TIM2	 				28
#define IRQ_NO_TIM3 				29
#define IRQ_NO_TIM4 				30
#define IRQ_NO_I2C1_EV 				31
#define IRQ_NO_I2C1_ER 				32
#define IRQ_NO_I2C2_EV 				33
#define IRQ_NO_I2C2_ER 				34
#define IRQ_NO_SPI1 				35
#define IRQ_NO_SPI2 				36
#define IRQ_NO_USART1				37
#define IRQ_NO_USART2	 			38
#define IRQ_NO_EXTI15_10 			40
#define IRQ_NO_EXTI17	 			41
#define IRQ_NO_RTC_ALARM 			41
#define IRQ_NO_EXTI18	 			42
#define IRQ_NO_OTG_FS_WKUP 			42
#define IRQ_NO_DMA1_STREAM7			47
#define IRQ_NO_SDIO		 			49
#define IRQ_NO_TIM5		 			50
#define IRQ_NO_SPI3		 			51
#define IRQ_NO_TIM5		 			50
#define IRQ_NO_DMA2_STREAM0			56
#define IRQ_NO_DMA2_STREAM1			57
#define IRQ_NO_DMA2_STREAM2			58
#define IRQ_NO_DMA2_STREAM3			59
#define IRQ_NO_DMA2_STREAM4			60
#define IRQ_NO_OTG_FS		 		67
#define IRQ_NO_DMA2_STREAM5			68
#define IRQ_NO_DMA2_STREAM6			69
#define IRQ_NO_DMA2_STREAM7			70
#define IRQ_NO_USART6	 			71
#define IRQ_NO_I2C3_EV	 			72
#define IRQ_NO_I2C3_ER		 		73
#define IRQ_NO_FPU		 			81
#define IRQ_NO_SPI4		 			84

// IRQ Priority macros
#define NVIC_IRQ_PRI0				0
#define NVIC_IRQ_PRI1				1
#define NVIC_IRQ_PRI2				2
#define NVIC_IRQ_PRI3				3
#define NVIC_IRQ_PRI4				4
#define NVIC_IRQ_PRI5				5
#define NVIC_IRQ_PRI6				6
#define NVIC_IRQ_PRI7				7
#define NVIC_IRQ_PRI8				8
#define NVIC_IRQ_PRI9				9
#define NVIC_IRQ_PRI10				10
#define NVIC_IRQ_PRI11				11
#define NVIC_IRQ_PRI12				12
#define NVIC_IRQ_PRI13				13
#define NVIC_IRQ_PRI14				14
#define NVIC_IRQ_PRI15				15


// Clock Enable Macros for GPIOx peripherals
#define GPIOA_PCLK_EN() (RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN() (RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN() (RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN() (RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN() (RCC->AHB1ENR |= (1<<4))

// Clock Enable Macros for I2Cx peripherals
#define I2C1_PCLK_EN() (RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN() (RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN() (RCC->APB1ENR |= (1<<23))

// Clock Enable Macros for SPIx peripherals
#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1<<15))
#define SPI4_PCLK_EN() (RCC->APB2ENR |= (1<<13))

// Clock Enable Macros for UARTx/USARTx peripherals
#define USART1_PCLK_EN() (RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN() (RCC->APB1ENR |= (1<<17))
#define USART6_PCLK_EN() (RCC->APB2ENR |= (1<<5))

// Clock Enable Macros for SYSCFG peripherals
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1<<14))

// Clock Disable Macros for GPIOx peripherals
#define GPIOA_PCLK_DI() (RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI() (RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI() (RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI() (RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI() (RCC->AHB1ENR &= ~(1<<4))

// Clock Disable Macros for I2Cx peripherals
#define I2C1_PCLK_DI() (RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI() (RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DI() (RCC->APB1ENR &= ~(1<<23))

// Clock Disable Macros for SPIx peripherals
#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~(1<<15))
#define SPI4_PCLK_DI() (RCC->APB2ENR &= ~(1<<13))

// Clock Disable Macros for USARTx peripherals
#define USART1_PCLK_DI() (RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DI() (RCC->APB1ENR &= ~(1<<17))
#define USART6_PCLK_DI() (RCC->APB2ENR &= ~(1<<5))

// Clock Disable Macros for SYSCFG peripherals
#define SYSCFG_PCLK_DI() (RCC->APB2ENR &= ~(1<<14))


//To reset the port, the value must go to 1 (being set) and then must return to 0 (or it will stay in reset mode);
#define GPIOA_REG_RESET() do { (RCC->AHB1RSTR |= (1<<0)); (RCC->AHB1RSTR &= ~(1<<0)); } while(0)
#define GPIOB_REG_RESET() do { (RCC->AHB1RSTR |= (1<<1)); (RCC->AHB1RSTR &= ~(1<<1)); } while(0)
#define GPIOC_REG_RESET() do { (RCC->AHB1RSTR |= (1<<2)); (RCC->AHB1RSTR &= ~(1<<2)); } while(0)
#define GPIOD_REG_RESET() do { (RCC->AHB1RSTR |= (1<<3)); (RCC->AHB1RSTR &= ~(1<<3)); } while(0)
#define GPIOE_REG_RESET() do { (RCC->AHB1RSTR |= (1<<4)); (RCC->AHB1RSTR &= ~(1<<4)); } while(0)
#define GPIOH_REG_RESET() do { (RCC->AHB1RSTR |= (1<<7)); (RCC->AHB1RSTR &= ~(1<<7)); } while(0)


#include "stm32f401xx_gpio_driver.h"
#include "stm32f401xx_spi_driver.h"
#include "stm32f401xx_i2c_driver.h"

#endif /* INC_STM32F401XX_H_ */
