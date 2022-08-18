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


//1.复制UART.C和UART.H文件到项目文件夹中。并在工程中添加。注意引用UART.H的文件路径。
//2.工程中添加Printf-stdarg.c文件
//3.在keil配置文件中CONFIG_DEBUG_UART=UART0,(注意后面是逗号)
//4.在上面包含#include "Uart.h"   在Main中添加uart_log_init();printf("hello world\r\n"); 
 
	