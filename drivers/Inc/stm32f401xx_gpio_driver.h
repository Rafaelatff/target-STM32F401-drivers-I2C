/*
 * stm32f401xx_gpio_driver.h
 *
 *  Created on: Dec 16, 2022
 *      Author: r.freitas
 */

#ifndef INC_STM32F401XX_GPIO_DRIVER_H_
#define INC_STM32F401XX_GPIO_DRIVER_H_

#include <stdint.h>


// Some generic macros
//@GPIO_PIN_NUMBERS
#define GPIO_PIN_NO_0 0
#define GPIO_PIN_NO_1 1
#define GPIO_PIN_NO_2 2
#define GPIO_PIN_NO_3 3
#define GPIO_PIN_NO_4 4
#define GPIO_PIN_NO_5 5
#define GPIO_PIN_NO_6 6
#define GPIO_PIN_NO_7 7
#define GPIO_PIN_NO_8 8
#define GPIO_PIN_NO_9 9
#define GPIO_PIN_NO_10 10
#define GPIO_PIN_NO_11 11
#define GPIO_PIN_NO_12 12
#define GPIO_PIN_NO_13 13
#define GPIO_PIN_NO_14 14
#define GPIO_PIN_NO_15 15

//@GPIO_PIN_MODES
#define GPIO_MODE_IN 0
#define GPIO_MODE_OUT 1
#define GPIO_MODE_ALTFN 2
#define GPIO_MODE_ANALOG 3 // value equal or less than 3 = non interrupt modes
#define GPIO_MODE_IT_FT 4 //interruptt falling edge trigger
#define GPIO_MODE_IT_RT 5 //input rising edge trigger
#define GPIO_MODE_IT_RFT 6 //input rising/falling edge trigger

//GPIO pin possible output types
#define GPIO_OP_TYPE_PP 0 // push pull
#define GPIO_OP_TYPE_OD 1 // open drain

//GPIO pin possible output speeds
//@GPIO_PIN_SPEEDS
#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST 2
#define GPIO_SPEED_HIGH 3

//GPIO pin possible pull up and down
#define GPIO_NO_PUPD 0
#define GPIO_PIN_PU 1
#define GPIO_PIN_PD 2

/*
    Peripheral register definition structure for GPIO
*/
typedef struct {
	volatile uint32_t MODER; // GPIO port mode register - Address offset: 0x00
	volatile uint32_t OTYPER; // GPIO port output type register - Address offset: 0x04
	volatile uint32_t OSPEEDER; // GPIO port output speed register - Address offset: 0x08
	volatile uint32_t PUPDR; // GPIO port pull-up/pull-down register - Address offset: 0x0C
	volatile uint32_t IDR; // GPIO port input data register - Address offset: 0x10
	volatile uint32_t ODR; // GPIO port output data register - Address offset: 0x14
	volatile uint32_t BSRR; // GPIO port bit set/reset register - Address offset: 0x18
	volatile uint32_t LCKR; // GPIO port configuration lock register - Address offset: 0x1C
	volatile uint32_t AFR[2]; // GPIO alternate function low register AFR[0] = AFRL and AFR[1] = AFRH - Address offset: 0x20-0X24
} GPIO_RegDef_t;

typedef struct{
	uint8_t GPIO_PinNumber; //@GPIO_PIN_NUMBERS
	uint8_t GPIO_PinMode; // possible valyes from @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed; //@GPIO_PIN_SPEEDS
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOpType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct{
	GPIO_RegDef_t *pGPIOx; //Holds the base address of the GPIO port
	GPIO_PinConfig_t GPIO_PinConfig; //Holds the GPIO pin config. settings
}GPIO_Handle_t;

/*******************************************************
 * 			API supported by this driver
 *
 *******************************************************/

/*
 *  Peripheral Clock Setup
 */
void GPIO_PeriClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

/*
 *  Init and De-Init
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

// Data read and write
uint8_t GPIO_Read(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber); //It also could be a boolean
uint16_t GPIO_PortRead(GPIO_RegDef_t *pGPIOx); // returns te port value
void GPIO_Out(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value); //value can be SET or RESET
void GPIO_PortOut(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_Toggle(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/*
 *  IRQ Configuration and ISR Handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t GPIO_PinNumber);


#endif /* INC_STM32F401XX_GPIO_DRIVER_H_ */
