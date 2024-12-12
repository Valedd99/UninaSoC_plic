#include <stdint.h>

int main(){

    /* Insert your code here */
/*
    uint32_t * gpio_addr = (uint32_t *) 0x20000;

    while(1){
	for(int i = 0; i < 100000; i++);
    	*gpio_addr = 0x00000001;
    	for(int i = 0; i < 100000; i++);
    	*gpio_addr = 0x00000000;
    }
*/

/*
typedef struct plic_t {
    uint32_t cr;            // 0x0
    uint32_t state;         // 0x4
    uint16_t a;             // 0x8
    // 0xA
    ...
    ...
}
*/
    uint32_t * gpio_addr = (uint32_t *) 0x20000;
    uint32_t * rv_plic_addr = (uint32_t *) 0x4000000;
    uint32_t * tim_addr = (uint32_t *) 0x40000;
    uint32_t reg_value_plic;
    uint32_t reg_value_gpio;
    uint32_t reg_value_tim;


    //SOURCES
    for (int i = 0; i < 32; i++) {
        
        reg_value_plic = *(rv_plic_addr + (0x4 * i) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_addr = 0x00000001;  
        }
    }
    

    //PENDING
    reg_value_plic = *(rv_plic_addr + (0x1000) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_addr = 0x00000002;  
        }

    //ENABLE
    reg_value_plic = *(rv_plic_addr + (0x2000) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_addr = 0x00000003;  
        }

    
    //THRESHOLD
    *(rv_plic_addr + (0x200000) / sizeof(uint32_t)) = 0x3;
    reg_value_plic = *(rv_plic_addr + (0x200000) / sizeof(uint32_t));

        
        if (reg_value_plic == 0x3) {
            *gpio_addr = 0x00000004;  //scrittura con successo
        }

    //CLAIM/COMPLETE
    reg_value_plic = *(rv_plic_addr + (0x200004) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_addr = 0x00000005;  
        }
    

    while(1);

    return 0;
}
