#include <stdio.h>
#include "tremo_regs.h"
#include "tremo_rtc.h"
#include "tremo_rcc.h"
#include "tremo_pwr.h"
#include "Uart.h"
#include "tremo_delay.h"

void rtc_alarm_wakeup()
{
    rtc_calendar_t time        = { 2019, 4, 12, 12, 23, 58, 15, 0 }; 
    rtc_calendar_t alarm0      = { 2019, 4, 12, 12, 23, 58, 25, 3700 };
    rtc_alarm_mask_t alarmMask = { 1, 0, 1, 1, 1, 15 };


    /* NVIC config */
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 2);

    rtc_set_calendar(&time);
    rtc_set_alarm(0, &alarmMask, &alarm0);
    
    rtc_config_alarm_wakeup(0, ENABLE);
    rtc_config_interrupt(RTC_ALARM0_IT, ENABLE);
    rtc_alarm_cmd(0, true);
    rtc_calendar_cmd(true);
}


void rtc_IRQHandler(void)
{
    uint8_t intr_stat;

    intr_stat = rtc_get_status(RTC_ALARM0_SR);
    printf("run\r\n");
    if (intr_stat == true) {
        rtc_config_interrupt(RTC_ALARM0_IT, DISABLE);
        rtc_set_status(RTC_ALARM0_SR, false);
        rtc_config_interrupt(RTC_ALARM0_IT, ENABLE);
        rtc_check_syn();
    }
}


int main(void)
{
     delay_init();
	  uart_log_init();
	  printf("ok\r\n");
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);

    // enable the clk
    rcc_enable_oscillator(RCC_OSC_XO32K, true);

    rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);

    rtc_alarm_wakeup();

    rtc_check_syn();
    printf("sleep\r\n");
    pwr_deepsleep_wfi(PWR_LP_MODE_STOP3);//阻塞型函数
    printf("if sleep?\r\n");
    /* Infinite loop */
    while (1) { 
        static  u32 i=0;
        printf("i=%d\r\n",i);
        i++;
        delay_ms(500);
    }
}


#ifdef USE_FULL_ASSERT
void assert_failed(void* file, uint32_t line)
{
    (void)file;
    (void)line;

    while (1) { }
}
#endif
