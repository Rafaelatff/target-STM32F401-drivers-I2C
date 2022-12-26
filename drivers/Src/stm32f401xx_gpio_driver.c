/*
 * stm32f401xx_gpio_driver.c
 *
 *  Created on: Dec 16, 2022
 *      Author: r.freitas
 */

#include "stm32f401xx.h"
#include "stm32f401xx_gpio_driver.h"


/*
 *	@name 	GPIO_PeriClkCtrl
 *	@brief	Clock Control for GPIO peripheral
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@param	EnOrDi can be ENABLE or DISABLE
 *	@return void type
 */
void GPIO_PeriClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		} else if (pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		} else if (pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		} else if (pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		} else if (pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		}
	}else{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		} else if (pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		} else if (pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		} else if (pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		} else if (pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		}
	}
}

/*
 *	@name 	GPIO_Ini
 *	@brief	GPIO peripheral initialization
 *
 *	@param	*pGPIOHandle -> p -> pointer to GPIOHandle address (structure GPIO_Handle_t type)
 *	@return void type
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	uint32_t temp =0; // temp. register

	// automatically enables the peripheral clock
	GPIO_PeriClkCtrl(pGPIOHandle->pGPIOx, ENABLE);

	// 1 Configure the mode of GPIO pin
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){ // <3
		// value equal or less than 3 = non interrupt modes
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		// 2* pois são 2 bits o MODER
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Clear bit fields bf setting new value
		pGPIOHandle->pGPIOx->MODER |= temp;
	}else{ // >= 4 INTERRUPT MODES

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//1. configure the FTSR Falling Trigger Selecting Register
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			//1. configure the RTSR Rising Trigger
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			// Clear the corresponding RTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//1. configure both FTSR and RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber /4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber %4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 *4);

		//3. enable the EXTI interrupt delivery using IMR (interrupt mask register)
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp=0;

	// 2 Configure Speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Clear bit fields bf setting new value
	pGPIOHandle->pGPIOx->OSPEEDER |= temp;

	temp=0;

	// 3 Configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Clear bit fields bf setting new value
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp=0;

	// 4 Configure the OPTYPE
	// This should be set only for output mode OR the structure GPIO_Handle_t must
	// be cleared when initialized
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOpType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Clear bit fields bf setting new value
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	temp=0;

	// 5 Configure the alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber /8;
		//temp1 will find the which AFR to use ([0] or [1]) by:
		//number less than 0 = [0] .. pin 0 to 7 /8 is equal to 0.?
		//number bigger than 1 = [1] .. pin 8 to 16 /8 is equal to 1.? or 2
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber %8;
		//temp2 will find out the number of the field position (pin) by taking the mod by 8
		// 0 mod 8 = 0, 1 mod 8 = 1, 2 mod 8 = 2, 3 mod 8 = 3 ... 7 mod 8 = 7
		// 8 mod 8 = 0, 9 mod 8 = 1, 10 mod 8 = 2, 11 mod 8 = 3 ... 15 mod 8 = 7
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2)); //Clear bit
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
		//AFRL/AFRH uses 4 bit position to configure each pin, so 4 bits must be set (4*)
	}
}

/*
 *	@name 	GPIO_DeInit
 *	@brief	De-initialization for GPIO peripheral
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@return void type
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	} else if (pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	} else if (pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	} else if (pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	} else if (pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	} else if (pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}
}


/*
 *	@name 	GPIO_Read
 *	@brief	Reading function for a single input pin
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@param  PinNumber is 0 to 15 -> macros created: GPIO_PIN_NO_x
 *	@return value can be 0 or 1
 */
uint8_t GPIO_Read (GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	uint8_t value;
	// shift value 'Pin Number' times to the right, so it's possible to simple mask and then read LSB
	value = ((uint8_t)(pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value; // @return = return value can be 0 o 1
}

/*
 *	@name 	GPIO_PortRead
 *	@brief	Reading function for the whole port
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@return value can be 0 to 0xFFFF
 */
uint16_t GPIO_PortRead (GPIO_RegDef_t *pGPIOx){
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value; // @return = return value can be 0 to 0xFFFF
}

/*
 *	@name 	GPIO_Out
 *	@brief	Control the output (low or high) for a single pin
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@param  PinNumber is 0 to 15 -> macros created: GPIO_PIN_NO_x
 *	@param  Value can be 0 or 1 -> macros created: GPIO_PIN_SET or GPIO_PIN_RESET
 *	@return void type
 */
void GPIO_Out(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
 //value can be SET or RESET
	if(Value == GPIO_PIN_SET){
		//Write 1 to the output data reguster at the bit field corresponding to the pin
		pGPIOx->ODR |= (1 << PinNumber);
	}else{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/*
 *	@name 	GPIO_PortOut
 *	@brief	Control the output (low or high) for the whole port
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@param  Value can be 0 or 0xFFFF
 *	@return void type
 */
void GPIO_PortOut(GPIO_RegDef_t *pGPIOx, uint16_t Value){
	pGPIOx->ODR = Value;
}

/*
 *	@name 	GPIO_Toggle
 *	@brief	Toggle the output signal for a single pin. If is high, after toggled it
 *			becomes low. If is low, after toggled becomes high.
 *
 *	@param	*pGPIOx -> pointer to GPIOx address (structure GPIO_RegDef_t type)
 *	@param  PinNumber is 0 to 15 -> macros created: GPIO_PIN_NO_x
 *	@return void type
 */
void GPIO_Toggle(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
	pGPIOx->ODR ^= (1 << PinNumber);
}

/*******     interrupt functions  **********/

/*
 *	@name 	GPIO_IRQInterruptConfig
 *	@brief	Configures the IRQ peripherals
 *
 *	@param	IRQNumber base on position of Table 38. Vector table for STM32F401xB/CSTM32F401xD/E
 *			-> macros created IRQ_NO_x
 *	@param  EnOrDi can be 0 or >= 1 -> macros created: ENABLE, DISABLE
 *	@return void type
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		if(IRQNumber <= 32){
			//program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}else if (IRQNumber > 31 &&  IRQNumber < 64){ //32 to 63
			//program ISER1 register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);
		}else if (IRQNumber >= 64 &&  IRQNumber < 96){ // 64 to 95 (it ends in 81 for our MCU)
			//program ISER2 register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);
		}
	}else{
		if(IRQNumber <= 32){
			//program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if (IRQNumber > 31 &&  IRQNumber < 64){
			//program ICER1 register
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}else if (IRQNumber >= 64 &&  IRQNumber < 96){
			//program ICER2 register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
	}
}

/*
 *	@name 	GPIO_IRQPriorityConfig
 *	@brief	Configures the priority for the IRQ
 *
 *	@param	IRQNumber base on position of Table 38. Vector table for STM32F401xB/CSTM32F401xD/E
 *			-> macros created IRQ_NO_x
 *	@param  IRQPriority is a value from 0 to 15
 *			-> macros created NVIC_IRQ_PRIx
 *	@return void type
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	//1. first lets find out the IPR register
	uint8_t iprx = IRQNumber /4;
	uint8_t iprx_section = IRQNumber %4;

	//*(NVIC_PR_BASE_ADDR + (iprx*4) |= (IRQPriority << (8 * iprx_section));
		//lower 4 bits are not implemented, so we need to shift 8 bits to the right

	uint8_t shift_amout = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amout);
}

/*
 *	@name 	GPIO_IRQHandling
 *	@brief	Clear the EXTI PR Register corresponding to the pin number
 *
 *	@param	GPIO_PinNumber is 0 to 15 -> macros created: GPIO_PIN_NO_x
 *	@return void type
 */
void GPIO_IRQHandling(uint8_t GPIO_PinNumber){
	//Clear the EXTI PR Register corresponding to the pin number
	if(EXTI->PR & (1 << GPIO_PinNumber)){
		//clear
		EXTI->PR |= (1 << GPIO_PinNumber);
	}
}
