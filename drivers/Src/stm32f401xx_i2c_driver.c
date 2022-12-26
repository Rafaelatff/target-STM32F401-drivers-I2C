/*
 * stm32f401xx_i2c_driver.c
 *
 *  Created on: Dec 25, 2022
 *      Author: rafaela
 */

#include "stm32f401xx_i2c_driver.h"

#define XTAL_VALUE 8000000

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512}; // aux RCCGetPCLK1Value function
uint8_t APB1_PreScaler[4] = {2,4,8,16}; // aux RCCGetPCLK1Value function

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);


/*
 *	@name 	I2C_PeriClkCtrl
 *	@brief	Clock Control for I2C peripheral
 *
 *	@param	*pI2Cx -> pointer to I2Cx address (structure I2C_RegDef_t type)
 *	@param	EnOrDi can be ENABLE or DISABLE
 *	@return void type
 */
void I2C_PeriClkCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
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

/*
 *	@name 	I2C_Init
 *	@brief
 *
 *	@param
 *	@return void type
 */
void I2C_Init(I2C_Handle_t *pI2CHandle){
	uint32_t tempreg = 0;
	// 1. Configure the mode (SM or FM)
	// bit 15 of I2C -> CCR register has a reset value of 0 (already in SM mode)

	// 2. Configure the speed of SCL
	// 2.a Configure the FREQ field of CR2 //2
	tempreg = 0;
	tempreg |= (RCC_GetPCLK1Value() /1000000U);
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3f); //mask the 5 first bits
	// 2.b Configure the CCR value of
	// Using thigh = tlow we have Tscl = 2 CCR x Tpclk
	// CCR = Fpclk / (2 x fscl) -> moved to frequency domain
	tempreg = 0;
	uint16_t ccr_value = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		ccr_value = (RCC_GetPCLK1Value() / (2 *pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= (ccr_value & 0xFFF); // only 12 bits are needed in CCR fields
	} else{
		tempreg |= (1 << 15); // bit 15 of I2C -> CCR register: 1 for FM
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if (pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
			ccr_value = (RCC_GetPCLK1Value() / (3 *pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}else {
			ccr_value = (RCC_GetPCLK1Value() / (25 *pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		tempreg |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

	// 3. Configure the device own address (when slave) //3
	//OAR1->ADDMODE has reset value as 0, being 7-bit slave mode,
	// don't need to config. unless we are working with 10-bit mode
	tempreg = 0;
	tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	// 4. Enable the acking //1
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << 10;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	// 5. Configure the rise time of the I2C pins

}

/*
 *	@name 	I2C_DeInit
 *	@brief
 *
 *	@param
 *	@return void type
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	// TODO -> Refer to RCC reset registers
}

/*
 *	@name 	I2C_MasterSendData
 *	@brief
 *
 *	@param	*pI2CHandle
 *	@param	*pTxbuffer
 *	@param	Len
 *	@param	SlaveAddr
 *	@param	Sr
 *	@return void type
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr){
	// 1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	// 2. Confirm that start generation is completed by checking the SB flag in the SR1
	//   Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_SB));

	// 3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx,SlaveAddr);

	// 4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_ADDR));

	// 5. Clear the ADDR flag according to its software sequence
	//   Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	I2C_ClearADDRFlag(pI2CHandle);

	// 6. Send the data until len becomes 0
	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE)); //Wait till TXE is set
		pI2CHandle->pI2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	// 7. when Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//   Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
	//   when BTF=1 SCL will be stretched (pulled to LOW)
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_TXE));
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_BTF));

	// 8. Generate STOP condition and master need not to wait for the completion of stop condition.
	//   Note: generating STOP, automatically clears the BTF
	if(Sr == I2C_DISABLE_SR)I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName){
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= ( 1 << I2C_CR1_STOP);
}


void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}else{
		pI2Cx->CR1 &= ~(1 << 0);
	}
}

uint32_t RCC_GetPLLOutputClock (void){
	// To be implemented
	return 0;
}




uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, SystemClk;
	uint8_t clksrc,temp,ahbp,apb1;

	// SWS
	clksrc = ((RCC->CFGR >>2) & 0x3);  // Bring those 2 bits to LSB and mask
	if(clksrc == 0){
		SystemClk = 16000000; // HSI
	} else if(clksrc == 1){
		SystemClk = XTAL_VALUE; // replace with xtal value (HSE)
	} else if(clksrc == 2){
		SystemClk = RCC_GetPLLOutputClock (); // To be implemented
	}

	// AHB prescaler
	temp = (RCC->CFGR >> 4) & 0xF; // Bring those 4 bits to LSB and mask
	if(temp <8){
		ahbp = 1;
	} else{
		ahbp = AHB_PreScaler[temp-8];
	}

	//APB1 prescaler
	temp = (RCC->CFGR >> 10) & 0x7; // Bring those 3 bits to LSB and mask
		if(temp <4){
			apb1 = 1;
		} else{
			apb1 = APB1_PreScaler[temp-4];
		}

	pclk1 = (SystemClk / ahbp) / apb1;
	return pclk1;
}

/////////local

static void  I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr){
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle){
	uint32_t dummyRead = pI2CHandle->pI2Cx->SR1;
	dummyRead = pI2CHandle->pI2Cx->SR2;
	(void)dummyRead;
}

/*static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle )
{
	uint32_t dummy_read;
	//check for device mode
	if(pI2CHandle->pI2Cx->SR2 & ( 1 << I2C_SR2_MSL))
	{
		//device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize  == 1)
			{
				//first disable the ack
				I2C_ManageAcking(pI2CHandle->pI2Cx,DISABLE);

				//clear the ADDR flag ( read SR1 , read SR2)
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}

		}
		else
		{
			//clear the ADDR flag ( read SR1 , read SR2)
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;

		}

	}
	else
	{
		//device is in slave mode
		//clear the ADDR flag ( read SR1 , read SR2)
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}


}*/


