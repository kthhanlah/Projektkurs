#include "pwm.h"


void TpowerUpInitPWM(uint16_t ch){
   timer_oc_parameter_struct timer_ocinitpara;
   timer_parameter_struct timer_initpara;
   rcu_periph_clock_enable(RCU_GPIOB);
   rcu_periph_clock_enable(RCU_AF);
   if (ch&0x1) gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
   if (ch&0x2) gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
   if (ch&0x4) gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
   if (ch&0x8) gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
   rcu_periph_clock_enable(RCU_TIMER2);


   timer_deinit(TIMER2);
   timer_struct_para_init(&timer_initpara);
   timer_initpara.prescaler         = 10799;
   timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
   timer_initpara.counterdirection  = TIMER_COUNTER_UP;
   timer_initpara.period            = 999;
   timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
   timer_initpara.repetitioncounter = 0;
   timer_init(TIMER2, &timer_initpara);


   timer_channel_output_struct_para_init(&timer_ocinitpara);
   timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
   timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
   timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
   timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
   timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
   timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
   if (ch&0x1) timer_channel_output_config(TIMER2,TIMER_CH_2,&timer_ocinitpara);
   if (ch&0x2) timer_channel_output_config(TIMER2,TIMER_CH_2,&timer_ocinitpara);
   if (ch&0x4) timer_channel_output_config(TIMER2,TIMER_CH_2,&timer_ocinitpara);
   if (ch&0x8) timer_channel_output_config(TIMER2,TIMER_CH_2,&timer_ocinitpara);


   if (ch&0x1) {
       timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,11999);
       timer_channel_output_mode_config(TIMER2,TIMER_CH_2,TIMER_OC_MODE_PWM0);
       timer_channel_output_shadow_config(TIMER2,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
   }
  if (ch&0x2) {
       timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,999);
       timer_channel_output_mode_config(TIMER2,TIMER_CH_2,TIMER_OC_MODE_PWM0);
       timer_channel_output_shadow_config(TIMER2,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
   }
  if (ch&0x4) {
       timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,11999);
       timer_channel_output_mode_config(TIMER2,TIMER_CH_2,TIMER_OC_MODE_PWM0);
       timer_channel_output_shadow_config(TIMER2,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
   }
  if (ch&0x8) {
       timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,999);
       timer_channel_output_mode_config(TIMER2,TIMER_CH_2,TIMER_OC_MODE_PWM0);
       timer_channel_output_shadow_config(TIMER2,TIMER_CH_2,TIMER_OC_SHADOW_DISABLE);
   }
  
   timer_auto_reload_shadow_enable(TIMER2);
   timer_enable(TIMER2);
}


void T2setPWMch2(int value){
   timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,value); //här ändrades T1setPWMch2 till ett 0 ist & TIMER_CH_0 ist för 2
}


void T2setLEDPercent(int percent){ //lagt till här från rad 65 till 72 för att timer förväntar mellan 0-1600
   if (percent > 100) percent = 100;

   //Omvandlar 0 till 100% till 0-1600
   int rawValue = (percent * 16000) / 100;
   T2setPWMch2(rawValue);
}


void T2setPWMmotorB(int throttel){
   if (throttel>0) {
      timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,(throttel/100.0)*16000);
      timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,0);
   } else {
      timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,0);
      timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,(-throttel/100.0)*16000);
   }
}

