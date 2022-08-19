#include "my_config1.h"




/*************************************************************
                 //系统初始化
**************************************************************/
void sys_init()
{
// enable the clk
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
  rcc_enable_oscillator(RCC_OSC_XO32K, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOC, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOD, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_SAC, true);
  rcc_enable_peripheral_clk(RCC_PERIPHERAL_LORA, true);

  delay_init();
  delay_ms(100);
  pwr_xo32k_lpm_cmd(true);

  uart_log_init();
  printf("ok\r\n");

  rtc_alarm_wakeup();
  //初始化I2C
  my_i2c_init();

  //设置LORA通讯参数
  Ra08KitLoraTestStart();

  // //按键中断初始化
  // gpio_set_iomux(GPIOA, GPIO_PIN_2, 0);
  // // rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
  // gpio_init(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT_PULL_DOWN);
  // gpio_config_interrupt(GPIOA, GPIO_PIN_2, GPIO_INTR_RISING_EDGE);

  // /* NVIC config */
  // NVIC_EnableIRQ(GPIO_IRQn);
  // NVIC_SetPriority(GPIO_IRQn, 2);
  //初始化GPIO
  gpio_set_iomux(GPIOA, GPIO_PIN_4, 0);
  gpio_set_iomux(GPIOA, GPIO_PIN_5, 0);
  gpio_set_iomux(GPIOA, GPIO_PIN_7, 0);
  gpio_init(GPIOA, GPIO_PIN_4, GPIO_MODE_OUTPUT_PP_LOW);
  gpio_init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_PP_LOW);
  gpio_init(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_PP_LOW);
}


/*************************************************************
                 //主函数
**************************************************************/
int main(void)
{
  sys_init();
  while(1)
  {
     //1.采集数据打包并存储在本地
      get_sys_data();
     //2.发送到指定网关设备
      send_data_to_gateway();
     //3.从网关设备同步设置参数
      rec_gateway_to_node();
     //4.刷新参数
     //5.休眠
    printf("sleep\r\n");
    pwr_deepsleep_wfi(PWR_LP_MODE_STOP3); //阻塞型函数
  }
  

}



























#ifdef USE_FULL_ASSERT
void assert_failed(void *file, uint32_t line)
{
  (void)file;
  (void)line;

  while (1)
  {
  }
}
#endif
