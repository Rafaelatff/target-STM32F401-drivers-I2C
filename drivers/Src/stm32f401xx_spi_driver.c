/*
 * stm32f401_spi_driver.c
 *
 *  Created on: 21 de dez de 2022
 *      Author: r.freitas
 */
#include "stm32f401xx_spi_driver.h"


// Private functions for this driver
// 'static' indicates that are actually private helper function (app cannot call them, compiler generates an error if so)
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);



/*******************************************************
 * 			API supported by this driver
 *
 *******************************************************/


/*
 *	@name 	SPI_PeriClkCtrl
 *	@brief	Clock Control for SPIO peripheral
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@param	EnOrDi can be ENABLE or DISABLE
 *	@return void type
 */
void SPI_PeriClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if (EnOrDi == ENABLE){
		if(pSPIx == SPI1){
			SPI1_PCLK_EN();
		} else if (pSPIx == SPI2){
			SPI2_PCLK_EN();
		} else if (pSPIx == SPI3){
			SPI3_PCLK_EN();
		} else if (pSPIx == SPI4){
			SPI4_PCLK_EN();
		}
	}else{
		if(pSPIx == SPI1){
			SPI1_PCLK_DI();
		} else if (pSPIx == SPI2){
			SPI2_PCLK_DI();
		} else if (pSPIx == SPI3){
			SPI3_PCLK_DI();
		} else if (pSPIx == SPI4){
			SPI4_PCLK_DI();
		}
	}
}

/*
 *	@name 	SPI_
 *	@brief
 *
 *	@param
 *	@return void type
 */
void SPI_Init(SPI_Handle_t *pSPIHandle){

	// automatically enables the peripheral clock
	SPI_PeriClkCtrl(pSPIHandle->pSPIx, ENABLE);

	// 1. Lets configure the SPI_CR1 register
	uint32_t tempreg = 0;

	// 1.a. Configure the device mode
	// NO MACROS: tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	// 1.b. Configure the bus configuration
	if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_BUS_CONFIG_FD){
		//Bi-direcional mode should be cleared
		//tempreg &= ~(SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_BUS_CONFIG_HD){
		//Bi-direcional mode should be set
		tempreg |= (SPI_CR1_BIDIMODE);
	}else if (pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_BUS_CONFIG_S_RXONLY){
		//Bi-direcional mode should be cleared
		tempreg &= ~(SPI_CR1_BIDIMODE);
		//RX Only bit must be set
		tempreg |= (SPI_CR1_RXONLY);
	}else if (pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_BUS_CONFIG_S_TXONLY){

	}
	// 1.c. Configure the SPI serial clock speed (baud rate)
	// NO MACROS: tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << 3;
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	// 1.d. Configure the DFF
	// NO MACROS: tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11;
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	// 1.e. Configure the CPOL
	// NO MACROS: tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1;
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	// 1.f. Configure the CPHA
	// NO MACROS: tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << 0;
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	// 1.g. Configure the SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	// Send values do SPI_CR1
	pSPIHandle->pSPIx->CR1 = tempreg;

	// 2. Lets configure the SPI_CR2 register
}

/*
 *	@name 	SPI_DeInit
 *	@brief
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@return void type
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 *	@name 	SPI_SendData
 *	@brief
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@param	*pTxBuffer
 *	@param  Len
 *	@return void type
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
	//SPI_SR_TXE says if the Transmit buffer empty (1 = empty)
	//RXNE flag set when RX buffer is not empty
	while (Len>0){
		//1. Wait until TXE is set
		//ONE WAY: while(!(pSPIx->SR & (1<<1)));
		while (SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET); //OR ANOTHER
		// Here we are polling for the TXE flag to SET [NOT GOOD]
		// Later we will implement an interrupt so code won't get stuck in here

		//2. Check the DFF  bit in CR1
		if((pSPIx->CR1 & (1 << SPI_CR1_DFF))){
			// 16-bit DFF
			//2.a. Load the data in to the DR
			pSPIx-> DR = *((uint16_t*)pTxBuffer); //de-referenced value typecasted to 16 bit lenght
			Len--;
			Len--; // 2 bytes sent at time
			(uint16_t*)pTxBuffer++; // Increment the buffer
		}else{
			// 8-bit DFF
			pSPIx-> DR = *pTxBuffer;
			Len--;
			pTxBuffer++; // Increment the buffer
		}
	}
}

/*
 *	@name 	SPI_ReceiveData
 *	@brief	Receive data in Rx buffer
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@param	*pTxBuffer
 *	@param  Len
 *	@return void type
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	//SPI_SR_TXE says if the Transmit buffer empty (1 = empty)
	//RXNE flag set when RX buffer is not empty
	while (Len>0){
		//1. Wait until RXNE is set
		while (SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET);
		// Here we are polling for the TXE flag to SET [NOT GOOD]
		// Later we will implement an interrupt so code won't get stuck in here

		//2. Check the DFF  bit in CR1
		if((pSPIx->CR1 & (1 << SPI_CR1_DFF))){
			// 16-bit DFF
			//2.a. Read from the DR into RX buffer
			*((uint16_t*)pRxBuffer) = pSPIx-> DR;
			Len--;
			Len--; // 2 bytes sent at time
			(uint16_t*)pRxBuffer++; // Increment the buffer
		}else{
			// 8-bit DFF
			*(pRxBuffer) = pSPIx-> DR;
			Len--;
			pRxBuffer++; // Increment the buffer
		}
	}
}
/*
 *  ***************       IRQ Configuration and ISR Handling ******************
 */


/*
 *	@name 	SPI_IRQInterruptConfig
 *	@brief	Configures the IRQ peripherals
 *
 *	@param	IRQNumber base on position of Table 38. Vector table for STM32F401xB/CSTM32F401xD/E
 *			-> macros created IRQ_NO_x
 *	@param  EnOrDi can be 0 or >= 1 -> macros created: ENABLE, DISABLE
 *	@return void type
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi){
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
 *	@name 	SPI_IRQPriorityConfig
 *	@brief	Configures the priority for the IRQ
 *
 *	@param	IRQNumber base on position of Table 38. Vector table for STM32F401xB/CSTM32F401xD/E
 *			-> macros created IRQ_NO_x
 *	@param  IRQPriority is a value from 0 to 15
 *			-> macros created NVIC_IRQ_PRIx
 *	@return void type
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority){
	//1. first lets find out the IPR register
	uint8_t iprx = IRQNumber /4;
	uint8_t iprx_section = IRQNumber %4;

	//*(NVIC_PR_BASE_ADDR + (iprx*4) |= (IRQPriority << (8 * iprx_section));
		//lower 4 bits are not implemented, so we need to shift 8 bits to the right

	uint8_t shift_amout = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amout);
}

void SPI_IRQHandling(SPI_Handle_t *pHandle){
	uint8_t temp1, temp2;
	// First, lets check all possible interruptions (TXE, RXNE and ERREIE)

	// Let's start with TXE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);
	if(temp1 && temp2){
		// handle TXE
		spi_txe_interrupt_handle(pHandle);
	}

	// Now let's check RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2){
		// handle TXE
		spi_rxe_interrupt_handle(pHandle);
	}

	// Now let's check for ERROs (NOT IMPLEMENTED: CRC ERROR, MODF [multi-master] and FRE [frame-format])
	// Implementing OVR - Overrun flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2){
		// handle TXE
		spi_ovr_err_interrupt_handle(pHandle); //
	}
}

/*
 *	@name 	SPI_PeripheralControl
 *	@brief	Enable or Disable the SPI. All configuration of SPI must occur with
 *			SPI disabled. After all configs are made, then (and only then) the
 *			SPI can be enable.
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@param	EnOrDi can be ENABLE or DISABLE
 *	@return void type
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/*
 *	@name 	SPI_SSIConfig
 *	@brief	Enable or Disable the SSI bit from CR1. This bit field must be enabled, or
 *			when using SW NSS, the  MODF error will occur.
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@param	EnOrDi can be ENABLE or DISABLE
 *	@return void type
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

/*
 *	@name 	SPI_SSOEConfig
 *	@brief	Enable or Disable the SPI SSOE (Slave Select Output Enable) from CR2 register.
 *			With SSOE = 1, NSS pin runs according to SPE status.
 *
 *	@param	*pSPIx -> pointer to SPIx address (structure SPI_RegDef_t type)
 *	@param	EnOrDi can be ENABLE or DISABLE
 *	@return void type
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi){
	if(EnOrDi == ENABLE){
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}else{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}


// Wait until flag is cleared using pooling method
// pooling is not ideal since the CPU stops inside this funcion
// the ideal method is by using interruption (later on this course I guess)
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
	if(pSPIx->SR & FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*
 *	@name 	SPI_SendDataIT
 *	@brief
 *
 *	@param
 *	@return void type
 */
uint8_t SPI_SendDataWithIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){
	uint8_t state = pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX){ // Check if SPI isn't busy receiving data already
		// 1. Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		// 2. Mark the SPI state as busy in transmission so that no
		//	  other code can take over same SPI peripheral until tx is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

		// 4. Data tx will be handled by the ISR code (implement later)
		// TBC
	}
	return state;
}

/*
 *	@name 	SPI_ReceiveDataIT
 *	@brief
 *
 *	@param
 *	@return void type
 */
uint8_t SPI_ReceiveDataWithIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
	uint8_t state = pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX){
		// 1. Save the Tx buffer address and Len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		// 2. Mark the SPI state as busy in transmission so that no
		//	  other code can take over same SPI peripheral until rx is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		// 4. Data tx will be handled by the ISR code (implement later)
		// TBC
	}
	return state;
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx){
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp; // Compiler wont complain about unused variable
}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen =0;
	pSPIHandle->TxState = SPI_READY;
}
void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen =0;
	pSPIHandle->RxState = SPI_READY;
}

__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent){
	// This is a weak implementation that will be override by another
}


/// helper functions implementations
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// Check the DFF  bit in CR1
	if((pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))){
		// 16-bit DFF
		//2.a. Load the data in to the DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer); //de-referenced value typecasted to 16 bit lenght
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--; // 2 bytes sent at time
		(uint16_t*)pSPIHandle->pTxBuffer++; // Increment the buffer
	}else{
		// 8-bit DFF
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer); //de-referenced value typecasted to 16 bit lenght
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++; // Increment the buffer
	}

	if (!pSPIHandle->TxLen){ // if TxLen = 0, Tx is over
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}
	//If Len isnt 0, the interruption is call again
}
static void spi_rxe_interrupt_handle(SPI_Handle_t *pSPIHandle){
	// do rxing as per the dff (8bit or 16bit)
	if(pSPIHandle->pSPIx->CR1 & (1 << 11)){
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer--;
		pSPIHandle->pRxBuffer--;
	}else{
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer--;
	}
	if (!pSPIHandle->RxLen){ // if RxLen = 0, Rx is over
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){
	uint8_t temp;
	// 1. Clear the OVR flag (by reading DR and SR)
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp; // Compiler wont complain about unused variable
	// 2. Inform the application
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}
