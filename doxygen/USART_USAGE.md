USART Module {#USART_Usage}
==========================

The module basically lets you
- Transmit a single byte or a string.
- Easily assign an ISR to handle the incoming bytes.
- Maintain a ring buffer with arbitrary length for the received data.

Also provided are routines to
- Unidirectionally parse the ring buffer
- Toggle the RXNEIE & RE control bits
- Check the RXNE & TXE flags.
- Initialize or destroy USART wrapper instances dynamically.

@ref HIERODULE_USART_Wrapper "HIERODULE_USART_Wrapper" is the main item in the module, with which you can access the variables related to the ring buffer and use the module routines to implement your RXNE ISR.

@rv_module_no_init Make sure to have configured the baudrate and GPIO modes of the pins for your peripheral.

Also notice that the USART IRQs are defined in the module's source file. The compiler will throw a multiple definition error if an IRQ is defined somewhere else.

To start using the module, first thing you need is a double pointer to @ref HIERODULE_USART_Wrapper "HIERODULE_USART_Wrapper" to create an instance for the USART peripheral.
```c
HIERODULE_USART_Wrapper **My_USART1_Wrapper = NULL;
```
It has to be a double pointer, since the wrapper initializer passes you the wrapper pointer defined in the module by reference. The wrapper instance is handled in the module with a pointer and not with a variable of type @ref HIERODULE_USART_Wrapper "HIERODULE_USART_Wrapper", since you use pointers for dynamic memory allocation.
<br><br>Secondly, you need a void function with a byte parameter for the RXNE ISR. You might as well just pass NULL if you intend to use the received data somewhere else. Or you can even maintain an array for the function pointers (i.e. void (\*)(uint8_t)) to switch between ISRs on the fly.
<br>Here's a simple example that assigns a task to the ISR that sums up the bytes received at USART1 and initializes the wrapper for USART1 with a ring buffer that's 12 bytes long.
<br>
```c
uint32_t Acc = 0;

void Add(uint8_t NewValue)
{
    Acc += NewValue;
}

/*

...

*/

My_USART1_Wrapper = HIERODULE_USART_InitWrapper(USART1, 12, Add);
```
<br>Once initialized, the wrapper may be used to transmit data.
```c
uint8_t SingleByte = '!';
HIERODULE_USART_TransmitByte((*My_USART1_Wrapper), SingleByte);

HIERODULE_USART_TransmitString((*My_USART1_Wrapper), "Hello World!\n");
```
<br>You need to enable the RE and RXNEIE bits at the control register to enable the USART IRQ and start receiving data. Simply call:
```c
HIERODULE_USART_Enable_IT_RXNE((*My_USART1_Wrapper));
```
<br>You can also disable the receive control bits via:
```c
HIERODULE_USART_Disable_IT_RXNE((*My_USART1_Wrapper));
```
<br>
You can "release" the instance of your wrapper to free memory:
```c
HIERODULE_USART_ReleaseWrapper((*My_USART1_Wrapper));
```
The release routine also clears the peripheral flags. Keep in mind this only frees the wrapper pointer in the module and not your double pointer that points to it. It's best to free and nullify that, as well:
```c
free(My_USART1_Wrapper);
My_USART1_Wrapper = NULL;
```
Notice that trying to access a freed memory address might turn out ugly for your run-time. However, you can reuse your double pointer to re-initialize a USART peripheral after releasing it.
<br><br>
