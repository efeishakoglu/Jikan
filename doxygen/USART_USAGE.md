USART Module {#USART_Usage}
===========================

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

@rv_usage_irq{USART}

To start using the module, first thing you need is a double pointer to @ref HIERODULE_USART_Wrapper "HIERODULE_USART_Wrapper" to create an instance for the USART peripheral.
```c
HIERODULE_USART_Wrapper **My_USART1_Wrapper = NULL;
```
@rv_usage_wrapper_double_ptr{HIERODULE_USART_Wrapper}
<br><br>@rv_usage_wrapper_isr{uint8_t,RXNE}
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
HIERODULE_USART_TransmitByte(*My_USART1_Wrapper, SingleByte);

HIERODULE_USART_TransmitString(*My_USART1_Wrapper, "Hello World!\n");
```
<br>You need to enable the RE and RXNEIE bits at the control register to enable the USART IRQ and start receiving data. Simply call:
```c
HIERODULE_USART_Enable_IT_RXNE(*My_USART1_Wrapper);
```
<br>You can also disable the receive control bits via:
```c
HIERODULE_USART_Disable_IT_RXNE(*My_USART1_Wrapper);
```
<br>@rv_usage_wrapper_release{USART,My_USART1_Wrapper}
