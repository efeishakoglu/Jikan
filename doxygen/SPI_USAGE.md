SPI Module {#SPI_Usage}
=======================

With this module, you can
- Initiate an SPI transmission as master.
- Unidirectionally parse the data received, which is kept in a ring buffer.
- Set up the data to be transmitted during the next transmission.
- Assign callback routines to be invoked at the end of a transmission as master or slave.

@ref HIERODULE_SPI_Wrapper "HIERODULE_SPI_Wrapper" is the main item in the module, with which you can access the variables related to the ring buffer and use the module routines to implement your SPI comm procedure.

@rv_module_no_init Make sure to have configured your SPI peripheral accordingly. Only full duplex slave and master modes are supported for 8-bit long transmissions.
<br>NSS a.k.a. chip selection is not handled by the module, you might want to simply assign a GPIO pin to each slave device, or use a multiplexer, as the master.
You might as well just use the hardware input/output functionality of the peripheral.
<br>Make sure to use external PU/PD resistors on the data and chip select lines, especially at higher baud rates. Consult the device manual the manufacturer provides for further directives.

@rv_usage_irq{SPI}

To start using the module, first thing you need is a double pointer to @ref HIERODULE_SPI_Wrapper "HIERODULE_SPI_Wrapper" to create an instance for the SPI peripheral.
```c
HIERODULE_SPI_Wrapper **My_SPI1_Wrapper = NULL;
```
@rv_usage_wrapper_double_ptr{HIERODULE_SPI_Wrapper}
<br><br>Next, define callback routine to be called at the end of a transmission.
```c
void TC_Handler(void)
{
/*
 * This will be performed at the end of a transmission.
*/
}
```
<br>You only need to implement the callback routines according to the device role and the consequent procedure, and let the module handle the communication. It's possible to switch to a different callback routine for a transmission mode as well, as long as the pointer to the routine is of type void(\*)(void).
<br>You can also just pass a null pointer if you do not need anything done at the end of a certain transmission mode.
<br>Keep in mind that the callback routine will be Called at the end of an entire transmission in master mode. As for slave mode, the routine will get invoked at the end of each byte transmission.

<br><br>Next, initialize the wrapper as master for SPI1 with the callback routine and an arbitrary RX buffer size, 24 for this instance.
```c
My_SPI1_Wrapper = HIERODULE_SPI_InitWrapper (SPI1, 1, 24, TC_Handler);
```
<br>Or, in case you intend to use the peripheral in slave mode, simply call the initializer with a mode parameter value of 0.
```c
My_SPI1_Wrapper = HIERODULE_SPI_InitWrapper (SPI1, 0, 24, TC_Handler);
```

<br>Now you're all set. If you've initialized the wrapper in slave mode, the peripheral should be able to handle the incoming transmissions from masters. Depending on your implementation, you might want to load an initial value to the data register, an exclamation mark for example:
```c
HIERODULE_SPI_TransmitByte(*My_SPI1_Wrapper, '!');
```
Likewise, use @ref HIERODULE_SPI_TransmitByte "HIERODULE_SPI_TransmitByte" to set up the data response for the next transmission inside your transmission complete callback, which is invoked *whenever the peripheral receives a byte at the MOSI line and sends its respond through the MISO line*. You can parse the received bytes via @ref HIERODULE_SPI_GetNextByte "HIERODULE_SPI_GetNextByte".
<br>You can also manipulate the transmit buffer to determine the next response with @ref HIERODULE_SPI_Wrapper::TX_Buffer "TX_Buffer" and @ref HIERODULE_SPI_Wrapper::TX_Counter "TX_Counter". Keeping different byte arrays and swapping between them on the fly with the received data in regard might be a good idea.

<br><br>To start a transmission as master, call the master transmit routine with your wrapper, a byte array and the number of bytes to be transmitted.
```c
uint8_t SPI1_TX_Buffer[6];

/*

 * ...

*/

HIERODULE_SPI_TransmitPackage(*My_SPI1_Wrapper, SPI1_TX_Buffer, 6);
```
In this case, your transmission complete callback is invoked *when the entire transmission sequence is completed and the peripherals have returned to the idle state*. Again, you can parse the received bytes via @ref HIERODULE_SPI_GetNextByte "HIERODULE_SPI_GetNextByte".
<br><br>@rv_usage_wrapper_release{SPI,My_SPI1_Wrapper}
