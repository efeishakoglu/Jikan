ADC Module {#ADC_Usage}
=======================
The module lets you easily assign an ISR to handle the data received at the ADC peripheral.
<br><br>
@ref HIERODULE_ADC_Wrapper "HIERODULE_ADC_Wrapper" is the main item in the module, with which you can access the variables related to the data received to implement your EOC ISR.
<br><br>
@rv_module_no_init Enable continuous conversion mode for the ADC channel you intend to use for exactly one conversion/rank.
<br><br>
@rv_usage_irq{ADC}
<br><br>
Create a double pointer to @ref HIERODULE_ADC_Wrapper "HIERODULE_ADC_Wrapper" to create an instance for the ADC peripheral.
```c
HIERODULE_ADC_Wrapper **ADC1_Wrapper = NULL;
```
@rv_usage_wrapper_double_ptr{HIERODULE_ADC_Wrapper}
<br><br>@rv_usage_wrapper_isr{uint16_t,EOC}
<br><br>Here's a simple example that assigns a task to the ISR that accumulates the data received at the ADC peripheral and initializes the wrapper for ADC1 with a filter weight of 0.75.
<br>
```c
uint32_t Acc = 0;

void Add(uint16_t NewValue)
{
    Acc += NewValue;
}

/*

...

*/

ADC1_Wrapper = HIERODULE_ADC_InitWrapper(ADC1, 12, Add);
(*ADC1_Wrapper)->FilterWeight = 0.75;
```
<br>If you do not want your data filtered, simply comment out the macro constant definition
@ref HIERODULE_ADC_SMOOTHENING_FILTER "HIERODULE_ADC_SMOOTHENING_FILTER"
in the header file, like so:
```c
//#define HIERODULE_ADC_SMOOTHENING_FILTER
```
<br>ADC conversion and ISR action will start once you enable the peripheral:
```c
HIERODULE_ADC_Enable(*ADC1_Wrapper);
```
<br>Likewise, you can halt the process via:
```c
HIERODULE_ADC_Disable(*ADC1_Wrapper);
```
<br>@rv_usage_wrapper_release{ADC,ADC1_Wrapper}
