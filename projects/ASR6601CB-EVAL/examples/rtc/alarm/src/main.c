#include <stdio.h>
#include "tremo_regs.h"
#include "tremo_rtc.h"
#include "tremo_rcc.h"


#include "tremo_uart.h"
#include "tremo_gpio.h"
#include "tremo_delay.h"


void rtc_alarm()
{
    rtc_calendar_t time            = { 2019, 4, 12, 12, 23, 58, 00, 0 };
    rtc_calendar_t alarm0          = { 2019, 4, 12, 12, 23, 59, 00, 3700 };
    rtc_alarm_mask_t alarmMask = { 1, 0, 1, 1, 1, 15 };

    /* NVIC config */
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 2);

    rtc_set_calendar(&time);
    rtc_set_alarm(0, &alarmMask, &alarm0);
    rtc_config_interrupt(RTC_ALARM0_IT, ENABLE);
    rtc_alarm_cmd(0, true);
    rtc_calendar_cmd(true);
}

void rtc_IRQHandler(void)
{
    uint8_t intr_stat;

    intr_stat = rtc_get_status(RTC_ALARM0_SR);
    printf("intr_stat=%d\r\n",intr_stat);
    if (intr_stat == true) {
        rtc_config_interrupt(RTC_ALARM0_IT, DISABLE);
        rtc_set_status(RTC_ALARM0_SR, false);
        rtc_config_interrupt(RTC_ALARM0_IT, ENABLE);
    }
}
void uart_log_init(void)
{
    // uart0
    gpio_set_iomux(GPIOB, GPIO_PIN_0, 1);
    gpio_set_iomux(GPIOB, GPIO_PIN_1, 1);

    /* uart config struct init */
    uart_config_t uart_config;
    uart_config_init(&uart_config);

    uart_config.baudrate = UART_BAUDRATE_115200;
    uart_init(CONFIG_DEBUG_UART, &uart_config);
    uart_cmd(CONFIG_DEBUG_UART, ENABLE);
}

int main(void)
{
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
    delay_init();
	uart_log_init();
	printf("hello world\r\n");   
    

    rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);

    // enable the clk
    rcc_enable_oscillator(RCC_OSC_XO32K, true);

    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);

    rtc_alarm();

    /* Infinite loop */
    while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(void* file, uint32_t line)
{
    (void)file;
    (void)line;

    while (1) { }
}
#endif
