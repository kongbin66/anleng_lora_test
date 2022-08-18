#include "Uart.h"
#include "tremo_gpio.h"
#include "tremo_rcc.h"
#include "tremo_uart.h"
void uart_log_init(void)
{
	  rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
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


//1.����UART.C��UART.H�ļ�����Ŀ�ļ����С����ڹ�������ӡ�ע������UART.H���ļ�·����
//2.���������Printf-stdarg.c�ļ�
//3.��keil�����ļ���CONFIG_DEBUG_UART=UART0,(ע������Ƕ���)
//4.���������#include "Uart.h"   ��Main�����uart_log_init();printf("hello world\r\n"); 
 
	