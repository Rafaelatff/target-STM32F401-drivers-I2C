# -target-STM32F401-drivers-I2C
This project uses STM32CubeIDE and it's a program created to practice my C habilities during the course 'Mastering Microcontroller and Embedded Driver Development' from FastBit Embedded Brain Academy. I am using a NUCLEO-F401RE board.

## theory

* START - SCL in high, SDA high to low transition;
* Data is transfered from MSB to LSB;
* Data can only change status (low to high or high to low) during the low phase of clock; 
* Slave Address has 7 bits, followed by R/nW bit information.
* ACK is a low signal, sent by slave/master depending on the moment; nACK is high;
* repeated STAR, SCL in high, SDA high to low transition, can happen when the master wants to change from R/nW. 
* STOP - SCL in high, SDA low to high transition;

![image](https://user-images.githubusercontent.com/58916022/209480227-00214b3c-6b49-4dfd-b3ec-d6d368e0c286.png)

We have 3 I2C channels (according to RM0368 reference manual):

![image](https://user-images.githubusercontent.com/58916022/209480269-6dd7fff7-f07f-4e7c-8177-2d38690e327a.png)

Using 'Table 9. Alternate function mapping' from stm32f401re datasheet, we can find the I2C pins. They are allocated only in AF04 or AF09.

![image](https://user-images.githubusercontent.com/58916022/209480364-ecbd6a04-6ec9-4198-8115-f3d5e94fbda6.png)


| Pin | SCL | SDA | AF0x |
| --- | --- | --- | --- |
| PA8 | I2C3_SCL | x | (AF04) |
| PB3 | x |  I2C2_SDA | (AF09) |
| PB4 | x | I2C3_SDA | (AF09) |
| PB6 | I2C1_SCL | x | (AF04) |
| PB7 | x | I2C1_SDA | (AF04) |
| PB8 | I2C1_SCL | x | (AF04) |
| PB9 | x | I2C1_SDA | (AF04) |
| PB10 | I2C2_SCL | x | (AF04) |
| PB11 | x | I2C2_SDA | (AF04) |
| PC9 | x | I2C3_SDA | (AF04) |

## coding

Let's start by creating the header and source files for the i2c driver.

![image](https://user-images.githubusercontent.com/58916022/209480667-d6b15f15-3ca1-4174-9cb4-1f2579cace2e.png)

We already created the base address for I2C at the uC specific header file along with another important macros (such as clock EN and DI).

![image](https://user-images.githubusercontent.com/58916022/209480747-ab5f0b9b-be2e-43e9-a59a-dabb1cd4b025.png)

Let's create, inside the uC specific header file a struct for the I2C_RegDef_t, such as:

![image](https://user-images.githubusercontent.com/58916022/209480994-4caf8ea8-5524-4ff4-9f94-c0b36edb538b.png)

This struct is created according with the 'Table 72. I2C register map and reset values'.

![image](https://user-images.githubusercontent.com/58916022/209480916-a9732446-1977-4e92-8e10-559a3a925ace.png)

We create some peripheral definition macros for the I2C using the structure I2C_RegDef_t:

![image](https://user-images.githubusercontent.com/58916022/209481118-1e178132-ecba-4a66-99e2-2a82983b30c0.png)

Still inside uC header file we created macros for the bit position definitions inside the I2C peripheral:

```
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
```

In the I2C header file we create the configuration and the handle structures for I2C.We also made the connections with the headers according to others drivers created so far.

![image](https://user-images.githubusercontent.com/58916022/209481802-0d1247f9-707b-4a79-bbf8-13327d6cf6f0.png)

![image](https://user-images.githubusercontent.com/58916022/209481827-b99fcfb6-0e42-4241-a804-145bf1e665b8.png)

Then we created some macros to use with the structure configuration of the I2C peripheral.

![image](https://user-images.githubusercontent.com/58916022/209481970-8be48d7e-b736-4308-927c-2e2b4e01eb6d.png)

Then we created the APIs for the driver:

```
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
```

The serial clock is configured by following those registers:

![image](https://user-images.githubusercontent.com/58916022/209482399-92673a1c-8f47-45a3-9f65-dc8a0f904105.png)

![image](https://user-images.githubusercontent.com/58916022/209482441-464d0237-0403-4100-83db-9434eda53b88.png)

![image](https://user-images.githubusercontent.com/58916022/209482375-072ed70f-2028-4aa3-b306-9ad7275d1951.png)

![image](https://user-images.githubusercontent.com/58916022/209482454-8121ff7f-f5a2-4dc6-89c5-9cf103d05fb5.png)


