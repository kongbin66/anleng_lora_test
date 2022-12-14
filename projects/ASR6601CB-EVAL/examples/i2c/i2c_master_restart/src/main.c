
#include "My_config.h"





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
    float temp1=0;
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
    delay_init();
	uart_log_init();
	printf("hello world\r\n");   
    my_i2c_init();
    
    /* Infinite loop */
    while (1) 
    { 
        temp1=read_sht20_temp(0xe3);
        printf("valuee3=%.2f\r\n",temp1/10);
        temp1=read_sht20_temp(0xe5);
        printf("valuee5=%.2f\r\n",temp1/10);
        
      
     //   printf("valuee5=%.2f!\n",read_sht20_temp(0xe5));
        delay_ms(1000);
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
