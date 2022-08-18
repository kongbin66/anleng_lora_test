#include <stdio.h>
#include "tremo_regs.h"
#include "tremo_rtc.h"
#include "tremo_rcc.h"
#include "tremo_delay.h"
#include "Uart.h"
#include "tremo_pwr.h"


//配置RTC_CYC周期，和中断唤醒。
void rtc_cyc()
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
    // enable the clk
    rcc_enable_oscillator(RCC_OSC_XO32K, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
    /* NVIC config */
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 2);

    rtc_config_cyc_max(327680);
    rtc_config_cyc_wakeup(ENABLE);
    rtc_cyc_cmd(true);
    rtc_config_interrupt(RTC_CYC_IT, ENABLE);   
}


int main(void)
{
	uart_log_init();
	printf("hello world\r\n"); 
    delay_init();
    rtc_cyc();
    u32 j=0;
    

    /* Infinite loop */
    while (1) { 
        rtc_check_syn();
		 printf("wakeup:j=%d\r\n",j++);
         delay_ms(100);
         printf("sleep\r\n");
        pwr_deepsleep_wfi(PWR_LP_MODE_STOP3);//阻塞型函数
		
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


 //此函数是中断
 
 void rtc_IRQHandler(void)
 {
     uint8_t intr_stat;
     intr_stat = rtc_get_status(RTC_CYC_SR);
     if (intr_stat == true) {
         rtc_config_interrupt(RTC_CYC_IT, DISABLE);
         rtc_set_status(RTC_CYC_SR, false);
         rtc_config_interrupt(RTC_CYC_IT, ENABLE);
         rtc_check_syn();
     }
 }