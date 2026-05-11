#include "gd32vf103.h"
#include "drivers.h"
#include "eclicw.h"
#include "lcd.h"
#include "ds18b20.h"
#include "usart.h"
#include "switch1.h"//Hannah
#include "pwm.h"


#define EI 1
#define DI 0

void ds18B20callBack(unsigned int temp);

int main(void){
    switch_hw_init();
    int ms=0, s=0, key, pKey=-1, c=0, idle=0;
    int lookUpTbl[16]={1,4,7,14,2,5,8,0,3,6,9,15,10,11,12,13};
    char msg[]="*";
    int currentInput=0;

    t5omsi();                               // Initialize timer5 1k
    keyinit();                              // Initialize keyboard toolbox
    Lcd_SetType(LCD_INVERTED);              // LCD_INVERTED/LCD_NORMAL!
    Lcd_Init();
    LCD_Clear(WHITE);
    u0init(DI);                             // Initialize USART0 toolbox

    ds18B20init(&ds18B20callBack);          // Initialize ds18B20 temp sensor
    

    eclic_global_interrupt_enable();
    int last_status=-1;
    int status=0;
    TpowerUpInitPWM(0);
    while(1){
        idle++;
        
        LCD_WR_Queue();
        u0_TX_Queue();
       
        status=get_switch_status();
       
        if(status==1){
            set_led_status(1);
            
            if(last_status==0){
                LCD_Clear(BLACK);
            }

        }else{
            set_led_status(0);
            if (last_status==1){ 
                LCD_Clear(BLACK);  
            }
        }  
        last_status=status;
    }    
    return 0;   
}

void ds18B20callBack(unsigned int temp){
    if(get_switch_status()==1){
        LCD_ShowNum(8, 10, temp>>4, 2, WHITE);
        LCD_ShowString(30, 10, (unsigned char *)"C", WHITE);
    }
}