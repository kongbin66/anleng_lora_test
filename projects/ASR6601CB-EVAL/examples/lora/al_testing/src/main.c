#include "my_config1.h"
 #include "stdio.h"
// #include "tremo_regs.h"
// #include "tremo_rtc.h"
// #include "tremo_rcc.h"
// #include "tremo_pwr.h"

// #include "tremo_delay.h"
//#define USE_MODEM_LORA

extern void uart_log_init(void);
rtc_calendar_t time = {2022, 5, 8, 12, 10, 40, 0, 0};
float temperature = 0.0; //温度
float humidity = 0.0;    //湿度
u32 time_stamp;          //时间戳
char sendBuf[100];   //发送缓存
typedef enum
{
   SYSID_IDLE=0,
   SYSID_GET,
   SYSID_SEND,
   SYSID_REC,
}sys_id_t;






//系统初始化
void sys_init()
{

  rcc_enable_peripheral_clk(RCC_PERIPHERAL_AFEC, true);
  // enable the clk
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



//获取发送数据
void get_sys_data()
{
   uint16_t gateway_freqcy=4700;
   uint16_t gateway_addr=1200;
   uint16_t node_freqcy=4700;
   uint16_t node_addr=2200;
   uint8_t signal=99;
   uint8_t charge=100;
   temperature=32.25;
   humidity=50.32;
   rtc_check_syn();
   rtc_get_calendar(&time);
  time_stamp = unixtime();
  printf("time_stamp =%d\r\n", time_stamp);
  //获取温湿度
  // SHT20_get_temp_humi(&temperature,&humidity);

  sprintf(sendBuf,"G_freqcy:%d,G_addr:%d,N_freqcy:%d,N_addr:%d,temper:%.2f,humi%.2f,signal:%d,charge:%d,time_stamp:%d",gateway_freqcy,gateway_addr,node_freqcy,node_addr,temperature,humidity,signal,charge,time_stamp);
	printf("sendBuf:%s\r\n",sendBuf);
   

}

//向网关发送数据
u8 send_data_to_geteway()
{
  static u8 sta = 1;
  Radio.Send((uint8_t *)sendBuf, strlen(sendBuf) + 1);
 
  if (sta)
    return 1;
  else
    return 0;
}

//从网关同步数据
u8 Sync_data()
{

  return 1;
}

int main(void)
{
  u8 sta = SYSID_SEND;
  sys_init();

  /* Infinite loop */
  while (1)
  {
    switch (sta)
    {
    case /* constant-expression */:
      /* code */
      break;
    
    default:
      break;
    }
    // 1.准备数据
    get_sys_data();
    // 2将数据发送给指定网关
    sta = send_data_to_geteway();
    if (sta)
    {
      printf("send_data_ok!\r\n");
    }
    else
      printf("send_data_fail !!!\r\n");
    // 3同步网关设置
    sta = Sync_data();
    if (sta)
    {
      printf("sync_data_ok!\r\n");
    }
    else
      printf("sync_data_fail !!!\r\n");
    // 4.休眠

    rtc_check_syn();
    rtc_get_calendar(&time);
    printf("%d/%d/%d,%d:%d:%d\r\n", time.year, time.month, time.day, time.hour, time.minute, time.second);

    //睡眠
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
