/*
 * 005spi_tx_testing.c
 *
 *  Created on: 21 de dez de 2022
 *      Author: r.freitas
 *
 *      For this exercise we are going to:
 *      1 - Test the SPI_SendData API to send the string "Hello world":
 *      2 - Using SPI2 Master mode
 *      3 - Using SCLK max possible
 *      4.a. - Using DFF = 0
 *      4.b. - Using DFF = 1
 *
 *      PB14 --> MISO - Not required
 *      PB15 --> MOSI
 *      PB13 --> SCLK
 *      PB12 --> NSS  - Not required
 *
 */
#include <string.h> // to use strlen() function
#include "stm32f401xx.h"

void SPI_GPIOInits(void);
void SPI2_Inits(void);


int main (void){

	char user_data[] = "Hello World";

	SPI_GPIOInits(); //Initialize GPIO pins to behave as SPI2 pins

	SPI2_Inits(); //Initialize the SPI2 peripheral parameters

	//Error since SS isnt config
	SPI_SSIConfig(SPI2,ENABLE);

	// Enable the SPI2 peripheral
	SPI_PeripheralControl(SPI2,ENABLE);
	//SPI_PeripheralControl(SPI2,DISABLE);
	//while (1) {
		SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));
	//}

		//SPI_PeripheralControl(SPI2,DISABLE);

	while (1);
}

void SPI_GPIOInits(void){
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOpType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//PB13 --> SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//PB15 --> MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//PB14 --> MISO --> not used, can be commented
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//PB12 --> NSS --> not used, can be commented
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPins);

}

void SPI2_Inits(void){
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = 1;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2; // Generates SCLK 8MHz
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_HIGH;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_HIGH;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // sw slave management en for NSS pin

	SPI_Init(&SPI2Handle);
}
