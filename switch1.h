// funktionen switch existerar och skickar en 1 eller 0 beroende om den är på eller av
#ifndef __SWITCH_H 
#define __SWITCH_H  
void switch_hw_init(void); // initierar register därav void 
int get_switch_status(void); // returnerar 1 eller 0 beroende på switchens status
void set_led_status(int state); // styr LED:en, tar in 1 eller 0 beroende på om den ska vara på eller av
#endif
