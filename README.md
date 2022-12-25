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

We have 3 I2C channels:

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

