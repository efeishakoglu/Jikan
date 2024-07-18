I2C Module {#I2C_Usage}
=======================

With this module, you can
- Initiate an I2C transmission as master transmitter or master receiver.
- Unidirectionally parse the data received as slave receiver, which is kept in a ring buffer.
- Set up the data to be transmitted as master or slave during the next transmission.
- Assign callback routines to be invoked at the end of a transmission, for the four transmission modes.

@ref HIERODULE_I2C_Wrapper "HIERODULE_I2C_Wrapper" is the main item in the module, with which you can access the variables related to the ring buffer and use the module routines to implement your I2C comm procedure.

@rv_module_no_init Make sure to have configured your I2C peripheral accordingly. Also, make sure to use external pull up resistors since most of the time internal pull-ups won't be strong enough for high speed. Typically, a 4.7kΩ resistor on each line should be enough but it's best to consult the device manual to find out the recommended resistance value for the I2C lines.

@rv_usage_irq{I2C}

To start using the module, first thing you need is a double pointer to @ref HIERODULE_I2C_Wrapper "HIERODULE_I2C_Wrapper" to create an instance for the I2C peripheral.
```c
HIERODULE_I2C_Wrapper **My_I2C1_Wrapper = NULL;
```
@rv_usage_wrapper_double_ptr{HIERODULE_I2C_Wrapper}
<br><br>Next, define the four callback routines to be called at the end of transmissions.
```c
void SRX1_Handler(void)
{
/*
 * This will be performed at the end of a slave receiver mode transmission.
*/
}

void MTX1_Handler(void)
{
/*
 * This will be performed at the end of a master transmitter mode transmission.
*/
}

void STX1_Handler(void)
{
/*
 * This will be performed at the end of a slave transmitter mode transmission.
*/
}

void MRX1_Handler(void)
{
/*
 * This will be performed at the end of a master receiver mode transmission.
*/
}
```
<br>Unlike other modules, callback functions you assign to the wrapper do not follow the routine-per-interrupt scheme, a.k.a. an ISR. In other words, you do not assign routines to be invoked at each received or transmitted byte, or to handle interrupt signals. The four routines you provide will only be invoked at the end of an I2C transmission.
<br>You only need to implement the callback routines according to the device role and the consequent procedure, and let the module handle the communication. It's possible to switch to a different callback routine for a transmission mode as well, as long as the pointer to the routine is of type void(\*)(void).
<br>You can also just pass a null pointer if you do not need anything done at the end of a certain transmission mode.
<br><br>Next, initialize the wrapper with the callback routines and an arbitrary SRX buffer size, 24 for this instance.
```c
My_I2C1_Wrapper = HIERODULE_I2C_InitWrapper(I2C1, 24, SRX1_Handler, MTX1_Handler, STX1_Handler, MRX1_Handler);
```

<br>And you're good to go. Now your wrapper can handle comm requests from masters, perform the callback routines you've assigned to the modes and update the slave receiver buffer when a master is done transmitting data. You can parse the data received as slave via @ref HIERODULE_I2C_GetNextByte "HIERODULE_I2C_GetNextByte".
<br><br>To transmit data as master, call the master transmit routine with your wrapper, a slave address, a byte array and the number of bytes to be transmitted.
```c
uint8_t SlaveAddress = 0b01000101;

uint8_t MTX1_Buffer[6];

/*

 * ...

*/

HIERODULE_I2C_MasterTransmit(*My_I2C1_Wrapper, SlaveAddress, MTX1_Buffer, 6);
```
<br>Receiving data from a slave is pretty similar.
```c
uint8_t MRX1_Buffer[12];
HIERODULE_I2C_MasterReceive(*My_I2C1_Wrapper, SlaveAddress, MRX1_Buffer, 12);
```
You do not need to shift or set the LSB of the device address, the routines take care of that.
<br><br>@rv_usage_wrapper_release{I2C,My_I2C1_Wrapper}
