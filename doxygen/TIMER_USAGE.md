Timer Module {#TimerUsage}
==========================

This module can be roughly divided into two parts:
- PWM output management.
- Basic interrupt handling and ISR assignment for timers (the latter being optional).

##PWM

You can use the provided PWM routines to
- Toggle counters.
- Toggle channel outputs.
- Toggle automatic output for a timer.
- Toggle main output for a timer.
- Set period or frequency of a timer.
- Set the duty cycle of a channel.
- Clear the counter of a timer.

<br>The module does not address peripheral configuration and initialization, it is assumed those are performed beforehand. Also, it's best to set a proper prescaler value for the timer you're going to work with, since frequency and period changes are performed by updating the ARR value in relation to the peripheral bus clock and PSC.<br>

Here's the basic use of PWM related routines:
```c
HIERODULE_TIM_SetPeriod(TIM1, 0.5);          //Set the period of TIM1 to 0.5 seconds.
HIERODULE_TIM_SetFrequency(TIM1, 2);         //Alternatively, set the frequency of TIM1 to 2 Hz.

HIERODULE_TIM_SetDutyCycle(TIM1, 1, 0.5);    //Set the duty cycle of PWM output channel 1 of TIM1 to 50%.

HIERODULE_TIM_EnableCounter(TIM1);           //TIM1 counter starts to count here.
HIERODULE_TIM_EnableChannel(TIM1, 1);        //Enable PWM output channel 1 of TIM1.
```
You can really call the routines in any order you want and get results accordingly.<br>

While using an advanced timer, you might need to enable main output to get the output channels to function properly. You can simply enable the MOE bit of a timer via:
```c
HIERODULE_TIM_EnableMainOutput(TIM1);
```

If you'd like to reset the counter of the timer after a frequency or period update, do so via:
```c
HIERODULE_TIM_ClearCounter(TIM1);
```

Finally, if you'd like to have the channel outputs automatically re-enabled after a break interrupt once the break flag is cleared, set the AOE bit via:
```c
HIERODULE_TIM_EnableAutomaticOutput(TIM1);
```
<br>All enabler routines cited above are paired with their disabler counterparts. Likewise, "set" functions come with their "get" counterparts.<br>
You can define macro constants instead of using mere integers to select output channels if you prefer those to really stand out in the code.<br>
Keep in mind that not all timers have same the set of status flags, identical control registers or the same number of output channels. Consult the device manual to verify the peripheral's features.

##Timer Interrupts

The main goal of this section is to facilitate the assignment of an ISR to an IRQ through a single, straightforward call.
<br> The whole process is pretty straightforward, just make sure the IRQs are not defined anywhere else other than the module's source file.
<br> Here's the code to basically bind "Some_ISR_Function" to the counter update interrupt request of TIM1 and enable the interrupt:
```c
void Some_ISR_Function(void);

/*

...

*/

HIERODULE_TIM_Assign_ISR_UPD(TIM1, Some_ISR_Function);

HIERODULE_TIM_Enable_IT_UPD(TIM1);
```
The same routine can be called anywhere during runtime and different interrupt service routines may be assigned.<br>
To clear the ISR pointer, simply invoke the routine with a null pointer instead of a function pointer:
```c
HIERODULE_TIM_Assign_ISR_UPD(TIM1, NULL);
```
<br>You might consider clearing the interrupt flag before enabling an interrupt, since an interrupt flag may be set even though the interrupt is disabled and the program won't branch to the IRQ routine. For example, the update and capture compare flags will still get set if the timer counter is enabled, even in case of a break-input where the PWM outputs are disabled. Configure the peripherals and implement your ISR taking flag states into account.

<br>Keep in mind that the interrupt flag is automatically cleared before the program leaves the IRQ body, regardless of whether an ISR has been assigned or not. Likewise, if the IRQ is defined for multiple flags, the flag checks are performed automatically.
<br>If you prefer to manually manage the flag handling, simply comment out the macro constant definition
@ref HIERODULE_TIM_CONVENIENT_IRQ "HIERODULE_TIM_CONVENIENT_IRQ"
in the header file, like so:
```c
//#define HIERODULE_TIM_CONVENIENT_IRQ
```
@ref HIERODULE_TIM_HANDLE_IRQ "HIERODULE_TIM_HANDLE_IRQ" is still required to be defined for this approach.
<br>You need to use a different sort of assignment routine if you take this route:
```c
HIERODULE_TIM_Assign_TIM1_UP_ISR(Some_ISR_Function);
```
No timer parameter is needed in this case, since the assignment is performed per IRQ, explicitly.<br>
Also provided are routines that let you toggle the timer interrupts and check or clear the interrupt flags, as you would make use of while implementing the ISR.
```c
void Some_ISR_Function(void)
{
    if( HIERODULE_TIM_IsSetFlag_UPD(TIM1) == 1)
    {
        /*

            Perform some tasks here...

        */

        HIERODULE_TIM_ClearFlag_UPD(TIM1);
    }
}


/*

    ...

*/
HIERODULE_TIM_Assign_TIM1_UP_ISR(Some_ISR_Function);

HIERODULE_TIM_Enable_IT_UPD(TIM1);

```
Notice that you'll need to check and clear multiple flags if the IRQ function is wired to be branched to on multiple interrupts. Advanced timers usually have different IRQs for break, update and capture compare interrupts but basic timers tend to have all of them lumped together within a single IRQ. Furthermore, some devices use the same IRQ for a combination of flags from different timers, such as TIM1_TRG_COM_TIM11_IRQHandler for STM32F401xC.
<br>If you do not want the IRQs defined within the timer module source file, simply comment out the macro constant definition
@ref HIERODULE_TIM_HANDLE_IRQ "HIERODULE_TIM_HANDLE_IRQ"
in the header file, like so:
```c
//#define HIERODULE_TIM_HANDLE_IRQ
```
It is irrelevant whether
@ref HIERODULE_TIM_CONVENIENT_IRQ "HIERODULE_TIM_CONVENIENT_IRQ"
is defined or not at this point.
<br>In this case, you need to define your IRQs somewhere else. You can still use the interrupt related routines except the assignment ones.

<br><br>Finally, you can use
@ref HIERODULE_TIM_SetRepetition "HIERODULE_TIM_SetRepetition"
to extend the effective period of your timer.
```c
HIERODULE_TIM_SetRepetition(TIM1, 9);
```
As for the given example, your capture compare interrupts will still be triggered only once every period, but your update interrupt will be triggered after 9 additional periods, which is in effect multiplying the UPD interrupt period by ten.<br>
This feature is usually limited to advanced timers.

