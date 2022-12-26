/*
 * stm32f401xx_i2c_driver.c
 *
 *  Created on: Dec 25, 2022
 *      Author: rafaela
 */

void I2C_PeriClkCtrl(I2C_RegDef_t *I2C, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		if(pI2Cx == I2C1){
			I2C1_PCLK_EN();
		} else if (pI2Cx == I2C2){
			I2C2_PCLK_EN();
		} else if (pI2Cx == I2C3){
			I2C3_PCLK_EN();
		}
	}else{
		if(pI2Cx == I2C1){
			I2C1_PCLK_DI();
		} else if (pI2Cx == I2C2){
			I2C2_PCLK_DI();
		} else if (pI2Cx == I2C3){
			I2C3_PCLK_DI();
		}
	}
}

void I2C_Init(I2C_Handle_t *pI2CHandle){
	uint32_t tempreg = 0;
	// 1. Configure the mode (SM or FM)

	// 2. Configure the speed of SCL

	// 3. Configure the device address (when slave)

	// 4. Enable the acking
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << 10; //1

	// 5. Configure the rise time of the I2C pins

}

void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	// TODO -> Refer to RCC reset registers
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}else{
		pI2Cx->CR1 &= ~(1 << 0);
	}
}
