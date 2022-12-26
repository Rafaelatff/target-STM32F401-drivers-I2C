/*
 * stm32f401xx_i2c_driver.h
 *
 *  Created on: Dec 25, 2022
 *      Author: rafaela
 */

#ifndef INC_STM32F401XX_I2C_DRIVER_H_
#define INC_STM32F401XX_I2C_DRIVER_H_

#include "stm32f401xx.h"

/*
 * 	Configuration structure for I2Cx peripheral
 */
typedef struct{
	uint32_t 	I2C_SCLSpeed;
	uint8_t		I2C_DeviceAddress;
	uint8_t		I2C_ACKControl;
	uint16_t	I2C_FMDutyCycle;
}I2C_Config_t;

/*
 * 	Handle structure for I2Cx peripheral
 */
typedef struct{
	I2C_RegDef_t	*pI2Cx;
	I2C_Config_t	I2C_Config;
}I2C_Handle_t;

/*
 * 	@I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM		100000		// SM = standard mode
#define I2C_SCL_SPEED_FM4K		400000		// FM = fast mode
#define I2C_SCL_SPEED_FM2K		200000		// FM = fast mode

/*
 * 	@I2C_ACKControl
 */
#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABLE			0

/*
 * 	@I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1

/*******************************************************
 * 			API supported by this driver
 *
 *******************************************************/

/*
 *  Peripheral Clock Setup
 */
void I2C_PeriClkCtrl(I2C_RegDef_t *pSPIx, uint8_t EnOrDi);

/*
 *  Init and De-Init
 */
void I2C_Init(I2C_Handle_t *pSPIHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 *  Data Send and Receive
 */


/*
 *  IRQ Configuration and ISR Handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

/*
 * 	Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);


/*
 * 	Application callback
 */
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEvent);

#endif /* INC_STM32F401XX_I2C_DRIVER_H_ */
