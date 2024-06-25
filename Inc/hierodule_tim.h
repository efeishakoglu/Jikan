/**
  ******************************************************************************
  * @file           : hierodule_tim.h
  * @author         : [ushumgigal](https://github.com/ushumgigal)
  * @brief          : Header file for the timer module.
  * @attention      : Copyrighted (2024) by
  * [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of
  * which may be found in the root folder of the
  * [repository](https://github.com/ushumgigal/hierodule).
  ******************************************************************************
  */
#ifndef __HIERODULE_TIM_H
#define __HIERODULE_TIM_H

#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup Hierodule_Tim Timer Module
  * @brief PWM output & timer interrupt management
  * @details Refer to the @ref TimerUsage "Usage" page for directives.
  * @{
  */
/** @addtogroup TIM_Public Global
  * @brief Elements of the module that are not confined to the scope of the
  * compilation unit. Perfectly corresponds to the module's header file, except
  * the IRQ handlers.
  * @details Consists of PWM output management and Timer interrupt handling
  * routines, as well as a void function pointer alias typedef for convenience
  * and a pair of precompiler constants to configure module behaviour.\n
  * Device-specific macro constants and type definitions are imported with an
  * include directive to main.h, where the device driver headers are assumed to
  * be included.\n
  * Also performed is an include directive to stdlib.h for abs.
  * @{
  */

/** @brief Precompiler constant to toggle timer ISR management elements
  * @details When commented out, IRQ handler routines won't be defined
  * in the source file.
  */
#define HIERODULE_TIM_HANDLE_IRQ

/** @brief Precompiler constant to select timer ISR assignment routines
  * @details When commented out, ISRs will be assigned per IRQ instead
  * of per interrupt flag and the routine @ref Check_IT "Check_IT" will
  * be omitted.\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}
  */
#define HIERODULE_TIM_CONVENIENT_IRQ

#include <main.h>
#include <stdlib.h>

/** @brief Typedef as for an alias for the void function pointer.
  * @details Used for convenience, really. It's used a lot, especially
  * in the source file. It would be a clutter of asterisks and paranthesis
  * otherwise.
  */
typedef void (*FUNC_POINTER)(void);

/** @brief Sets the period duration of a timer.
  * @rv_param_timer
  * @param DurationSec: Duration of period in seconds.
  * @return None
  */
void HIERODULE_TIM_SetPeriod(TIM_TypeDef *Timer, double DurationSec);

/** @brief Returns the period duration of a timer in seconds.
  * @rv_param_timer
  * @return Period duration in seconds.
  */
double HIERODULE_TIM_GetPeriod(TIM_TypeDef *Timer);

/** @brief Sets the frequency of a timer.
  * @rv_param_timer
  * @param Frequency_Hz: Frequency in Hertz.
  * @return None
  */
void HIERODULE_TIM_SetFrequency(TIM_TypeDef *Timer, double Frequency_Hz);

/** @brief Returns the frequency of a timer in Hertz.
  * @rv_param_timer
  * @return Frequency in Hertz.
  */
double HIERODULE_TIM_GetFrequency(TIM_TypeDef *Timer);

/** @brief Clears the counter register of a timer.
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearCounter(TIM_TypeDef *Timer);

/** @brief Sets the additional period repetitions of a timer.
  * @rv_param_timer
  * @param Reps: Number of repetitions.
  * @return None
  */
void HIERODULE_TIM_SetRepetition(TIM_TypeDef *Timer, uint32_t Reps);

/** @brief Returns the number of additional period repetitions of a timer.
  * @rv_param_timer
  * @return Number of repetitions.
  */
uint32_t HIERODULE_TIM_GetRepetition(TIM_TypeDef *Timer);

/** @brief @rv_triple_action{Enables,PWM output channel of a timer}
  * @rv_param_timer
  * @rv_param_ch_14
  * @return None
  */
void HIERODULE_TIM_EnableChannel(TIM_TypeDef *Timer, int8_t Channel);

/** @brief @rv_triple_action{Disables,PWM output channel of a timer}
  * @rv_param_timer
  * @rv_param_ch_14
  * @return None
  */
void HIERODULE_TIM_DisableChannel(TIM_TypeDef *Timer, int8_t Channel);

/** @brief @rv_triple_action{Checks the status of,PWM output channel of a timer}
  * @rv_param_timer
  * @rv_param_ch_14
  * @return @rv_bool_ret_en{channel}
  */
uint32_t HIERODULE_TIM_IsEnabledChannel(TIM_TypeDef *Timer, int8_t Channel);

/** @brief @rv_bdtr_bit_toggle{Sets,main output enable}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_EnableMainOutput(TIM_TypeDef *Timer);

/** @brief @rv_bdtr_bit_toggle{Clears,main output enable}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_DisableMainOutput(TIM_TypeDef *Timer);

/** @brief Sets the duty cycle of the specified PWM output channel
  * of a timer.
  * @rv_param_timer
  * @rv_param_ch_14
  * @param NormalizedDutyCycle: Duty cycle percentage, normalized.
  * @return None
  */
void HIERODULE_TIM_SetDutyCycle(TIM_TypeDef *Timer, uint8_t Channel,
    double NormalizedDutyCycle);

/** @brief Returns the duty cycle of the specified PWM output channel
  * of a timer.
  * @rv_param_timer
  * @rv_param_ch_14
  * @return Duty cycle percentage, normalized.
  */
double HIERODULE_TIM_GetDutyCycle(TIM_TypeDef *Timer, uint8_t Channel);

/** @brief @rv_action_tim_it_flag{Clears, update}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearFlag_UPD(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Clears, capture compare channel 1}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearFlag_CC1(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Clears, capture compare channel 2}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearFlag_CC2(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Clears, capture compare channel 3}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearFlag_CC3(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Clears, capture compare channel 4}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearFlag_CC4(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Clears, break}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_ClearFlag_BRK(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Checks the status of, update}
  * @rv_param_timer
  * @return @rv_tim_it_ret
  */
uint32_t HIERODULE_TIM_IsSetFlag_UPD(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Checks the status of, capture compare channel 1}
  * @rv_param_timer
  * @return @rv_tim_it_ret
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC1(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Checks the status of, capture compare channel 2}
  * @rv_param_timer
  * @return @rv_tim_it_ret
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC2(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Checks the status of, capture compare channel 3}
  * @rv_param_timer
  * @return @rv_tim_it_ret
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC3(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Checks the status of, capture compare channel 4}
  * @rv_param_timer
  * @return @rv_tim_it_ret
  */
uint32_t HIERODULE_TIM_IsSetFlag_CC4(TIM_TypeDef *Timer);

/** @brief @rv_action_tim_it_flag{Checks the status of, break}
  * @rv_param_timer
  * @return @rv_tim_it_ret
  */
uint32_t HIERODULE_TIM_IsSetFlag_BRK(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Enables,update}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Enable_IT_UPD(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Enables,capture compare channel 1}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Enable_IT_CC1(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Enables,capture compare channel 2}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Enable_IT_CC2(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Enables,capture compare channel 3}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Enable_IT_CC3(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Enables,capture compare channel 4}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Enable_IT_CC4(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Enables,break}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Enable_IT_BRK(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Disables,update}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Disable_IT_UPD(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Disables,capture compare channel 1}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Disable_IT_CC1(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Disables,capture compare channel 2}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Disable_IT_CC2(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Disables,capture compare channel 3}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Disable_IT_CC3(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Disables,capture compare channel 4}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Disable_IT_CC4(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Disables,break}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_Disable_IT_BRK(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Checks,update}
  * @rv_param_timer
  * @return @rv_bool_ret_en{update interrupt}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_UPD(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Checks,capture compare channel 1}
  * @rv_param_timer
  * @return @rv_bool_ret_en{capture compare channel 1 interrupt}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC1(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Checks,capture compare channel 2}
  * @rv_param_timer
  * @return @rv_bool_ret_en{capture compare channel 2 interrupt}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC2(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Checks,capture compare channel 3}
  * @rv_param_timer
  * @return @rv_bool_ret_en{capture compare channel 3 interrupt}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC3(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Checks,capture compare channel 4}
  * @rv_param_timer
  * @return @rv_bool_ret_en{capture compare channel 4 interrupt}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_CC4(TIM_TypeDef *Timer);

/** @brief @rv_toggle_tim_it{Checks,break}
  * @rv_param_timer
  * @return @rv_bool_ret_en{break interrupt}
  */
uint32_t HIERODULE_TIM_IsEnabled_IT_BRK(TIM_TypeDef *Timer);

/** @brief @rv_bdtr_bit_toggle{Sets,automatic output enable}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_EnableAutomaticOutput(TIM_TypeDef *Timer);

/** @brief @rv_bdtr_bit_toggle{Clears,automatic output enable}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_DisableAutomaticOutput(TIM_TypeDef *Timer);

/** @brief @rv_triple_action{Enables,counter of a timer}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_EnableCounter(TIM_TypeDef *Timer);

/** @brief @rv_triple_action{Disables,counter of a timer}
  * @rv_param_timer
  * @return None
  */
void HIERODULE_TIM_DisableCounter(TIM_TypeDef *Timer);

/** @brief @rv_triple_action{Checks the status of,counter of a timer}
  * @rv_param_timer
  * @return @rv_bool_ret_en{counter}
  */
uint32_t HIERODULE_TIM_IsEnabledCounter(TIM_TypeDef *Timer);

/** \cond */
#ifdef HIERODULE_TIM_HANDLE_IRQ
    #ifdef HIERODULE_TIM_CONVENIENT_IRQ /** \endcond */
/** @brief @rv_tim_assign_isr_conv{update}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_timer
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_ISR_UPD(TIM_TypeDef *Timer, FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_conv{capture compare channel 1}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_timer
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_ISR_CC1(TIM_TypeDef *Timer, FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_conv{capture compare channel 2}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_timer
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_ISR_CC2(TIM_TypeDef *Timer, FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_conv{capture compare channel 3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_timer
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_ISR_CC3(TIM_TypeDef *Timer, FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_conv{capture compare channel 4}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_timer
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_ISR_CC4(TIM_TypeDef *Timer, FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_conv{break}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_timer
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_ISR_BRK(TIM_TypeDef *Timer, FUNC_POINTER ISR);
    /** \cond */
    #else /** \endcond */
/** @brief @rv_tim_assign_isr_plain{timer 1 capture compare}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_TIM1_CC_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 2}\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
        /** \cond */
        #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
        void HIERODULE_TIM_Assign_TIM2_ISR(FUNC_POINTER ISR);
        /** \cond */
        #endif /** \endcond */

/** @brief @rv_tim_assign_isr_plain{timer 3}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
        void HIERODULE_TIM_Assign_TIM3_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 4}\n
  * @rv_def_req_device{__STM32F103xB_H or __STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
        /** \cond */
        #if ( (defined __STM32F103xB_H) || (defined __STM32F401xC_H) ) /** \endcond */
        void HIERODULE_TIM_Assign_TIM4_ISR(FUNC_POINTER ISR);
        /** \cond */
        #endif /** \endcond */

        /** \cond */
        #ifdef __STM32F103xB_H /** \endcond */
/** @brief @rv_tim_assign_isr_plain{timer 1 update}\n
  * @rv_def_req_device{__STM32F103xB_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM1_UP_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 1 break}\n
  * @rv_def_req_device{__STM32F103xB_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM1_BRK_ISR(FUNC_POINTER ISR);
        /** \cond */
        #elif defined __STM32F401xC_H /** \endcond */
/** @brief @rv_tim_assign_isr_plain{timer 1 update - timer 10}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM1_UP_TIM10_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 1 break - timer 9}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM1_BRK_TIM9_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 5}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM5_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 1 trigger comm - timer 11}\n
  * @rv_def_req_device{__STM32F401xC_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM1_TRG_COM_TIM11_ISR(FUNC_POINTER ISR);

        /** \cond */
        #elif defined __STM32F030x6_H /** \endcond */

/** @brief @rv_tim_assign_isr_plain{timer 1 break - update - trigger comm}\n
  * @rv_def_req_device{__STM32F030x6_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM1_BRK_UP_TRG_COM_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 14}\n
  * @rv_def_req_device{__STM32F030x6_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM14_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 16}\n
  * @rv_def_req_device{__STM32F030x6_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM16_ISR(FUNC_POINTER ISR);

/** @brief @rv_tim_assign_isr_plain{timer 17}\n
  * @rv_def_req_device{__STM32F030x6_H}\n
  * @rv_def_req{HIERODULE_TIM_HANDLE_IRQ}\n
  * @rv_not_def_req{HIERODULE_TIM_CONVENIENT_IRQ}
  * @rv_param_fp_isr
  * @return None
  */
            void HIERODULE_TIM_Assign_TIM17_ISR(FUNC_POINTER ISR);

        /** \cond */
        #endif
    #endif //HIERODULE_TIM_CONVENIENT_IRQ
#endif //HIERODULE_TIM_HANDLE_IRQ /** \endcond */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __HIERODULE_TIM_H */

