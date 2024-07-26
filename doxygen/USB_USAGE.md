USB Module {#USB_Usage}
=======================

This module only covers the CDC mode for the USB peripheral, which lets you
- Unidirectionally parse the data received, which is kept in a ring buffer.
- Set up the data to be transmitted during the next transmission.
- Assign a callback routine to be invoked at the end of a transmission.

@ref HIERODULE_USB_Wrapper "HIERODULE_USB_Wrapper" is the main item in the module, with the instance of which you can access the variables related to the ring buffer and use the module routines to implement your USB comm procedure.

@rv_module_no_init Make sure to have configured your USB peripheral accordingly.

<br>First, update the CDC_Receive_FS routine defined within within usbd_cdc_if.c so that the module's reception callback routine will be invoked whenever an incoming transmission has been completed. Simply, add an include directive to the module at the top, then add a call to @ref HIERODULE_USB_Receive_Callback "HIERODULE_USB_Receive_Callback" right before the routine's return statement.
```c
/*
 *  usbd_cdc_if.c
 */

#include <hierodule_usb.h>

/*
 *  ...
 */

/*
 *  ...
 *  within CDC_Receive_FS body
 */
HIERODULE_USB_Receive_Callback(Buf, Len);
/*
 *  return ...
 */

```
Except the painfully simplifed syntax and omitted source code, it should pretty much look like this.

<br><br>Optionally, you can define a callback routine to be called at the end of a transmission.
```c
void TC_Handler(void)
{
/*
 * This will be performed at the end of a transmission.
 */
}
```
You can also just pass a null pointer if you do not need anything done at the end of the transmission.

<br><br>Next, initialize the wrapper with the callback routine and an arbitrary RX buffer size, 24 for this instance.
```c
HIERODULE_USB_InitWrapper(24, TC_Handler);
```

<br>And that's it! The peripheral, and consequently the module, should be able to handle the incoming transmissions from the host. To set up data to be transmitted at the next transmission commenced by the host, simply use @ref HIERODULE_USB_TransmitPackage "HIERODULE_USB_TransmitPackage"
```c
const char *TX_Buffer = "Hello World!\n";
uint32_t Size = strlen(TX_Buffer);

HIERODULE_USB_TransmitPackage(TX_Buffer, Size);
```
The first parameter to the routine is really an uint8_t pointer; your compiler might warn you about implicit casts, but you should be fine as long as it's a c-type string, a pointer or an array of uint8_t, char etc.
<br>Likewise, use the same routine to set up the data response for the next transmission inside your transmission complete callback. You can parse the received bytes via @ref HIERODULE_USB_GetNextByte "HIERODULE_USB_GetNextByte". Or, you can directly access the buffer contents with the wrapper's instance, that's declared within the header file as extern.
```c
Wrapper.RX_Buffer;
```

<br>Finally, you can "release" the wrapper to save memory.
```c
HIERODULE_USB_ReleaseWrapper();
```
This merely frees the ring buffer address and nullifies it. Make sure to initialize the wrapper with adequate buffer size before accessing it again.
