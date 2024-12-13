#include <stdint.h>

#define GPIO_DATA    0x0000  // Data Register
#define GPIO_TRI     0x0004  // Direction Register (1 per input)
#define GIER         0x011C  // Global Interrupt Enable Register
#define IP_IER       0x0128  // Interrupt Enable Register
#define IP_ISR       0x0120  // Interrupt Status Register

int main(){

    /* Insert your code here */
/*
    uint32_t * gpio_out_addr = (uint32_t *) 0x20000;

    while(1){
	for(int i = 0; i < 100000; i++);
    	*gpio_out_addr = 0x00000001;
    	for(int i = 0; i < 100000; i++);
    	*gpio_out_addr = 0x00000000;
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
    uint32_t * gpio_out_addr  = (uint32_t *) 0x20000;
    uint32_t * gpio_in_addr   = (uint32_t *) 0x30000;
    uint32_t * rv_plic_addr   = (uint32_t *) 0x4000000;
    uint32_t * tim_addr       = (uint32_t *) 0x40000;
    uint32_t reg_value_plic;
    uint32_t reg_value_gpio;
    uint32_t reg_value_tim;


    //Set interrupt priorities
    for (int i = 1; i < 4; i++) {
        
        *(rv_plic_addr + (0x4 * i) / sizeof(uint32_t)) = 0x1;

    }

    //Abilitare le interruzioni del plic delle prime 4 sorgenti 
    *(rv_plic_addr + (0x2000) / sizeof(uint32_t)) = 0xf;

     // 1. Configura il GPIO come input (1 nel GPIO_TRI)
    *(gpio_in_addr + (GPIO_TRI / sizeof(uint32_t))) = (0x1);  // Configura il primo pin come input

    // 2. Abilita l'interrupt per il canale (1 nel IP_IER)
    *(gpio_in_addr + (IP_IER / sizeof(uint32_t))) = (0x1);  // Abilita l'interrupt sul primo pin

    // 3. Abilita gli interrupt globali (1 nel GIER)
    *(gpio_in_addr + (GIER / sizeof(uint32_t))) = (0x80000000);  // Abilita gli interrupt globali scrivendo 1 nel 32simo bit del registro


    
    //*(tim_addr) = 0x000000D8;
    *(tim_addr + ( 0x4 )/ sizeof(uint32_t)) = 0x1312D00;  //Ossia 20000000 per contare un  secondo a 20 MHz
    //*(tim_addr + ( 0x4 )/ sizeof(uint32_t)) = 0x5F5E0FE;  //5 secondi
    //*(tim_addr) = 0x00000072;
    //*(tim_addr) |=0x40;



    // Passo 2: Settare il bit LOAD0 per trasferire il valore nel TCR0
    *(tim_addr) = 0x00000020;  // LOAD0 = 1 (bit 5), tutto il resto a 0

    // Passo 3: Abbassare LOAD0 (necessario per avviare correttamente il timer)
    *(tim_addr) &= ~0x20;  // LOAD0 = 0 (bit 5 abbassato)

    // Passo 4: Configurare Auto Reload e Down Counter
    *(tim_addr) |= 0x10;  // ARHT0 = 1 (bit 4), Auto Reload abilitato
    *(tim_addr) |= 0x02;  // UDT0 = 1 (bit 1), conteggio decrescente

    // Passo 5: Abilitare l'interruzione
    *(tim_addr) |= 0x40;  // ENIT0 = 1 (bit 6), interrupt abilitato

    // Passo 6: Abilitare il timer
    *(tim_addr) |= 0x80;  // ENT0 = 1 (bit 7), timer abilitato  
    





    /*
    //SOURCES
    for (int i = 0; i < 32; i++) {
        
        reg_value_plic = *(rv_plic_addr + (0x4 * i) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_out_addr = 0x00000001;  
        }
    }
    

    //PENDING
    reg_value_plic = *(rv_plic_addr + (0x1000) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_out_addr = 0x00000002;  
        }

    //ENABLE
    reg_value_plic = *(rv_plic_addr + (0x2000) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_out_addr = 0x00000003;  
        }

    
    //THRESHOLD
    *(rv_plic_addr + (0x200000) / sizeof(uint32_t)) = 0x3;
    reg_value_plic = *(rv_plic_addr + (0x200000) / sizeof(uint32_t));

        
        if (reg_value_plic == 0x3) {
            *gpio_out_addr = 0x00000004;  //scrittura con successo
        }

    //CLAIM/COMPLETE
    reg_value_plic = *(rv_plic_addr + (0x200004) / sizeof(uint32_t));

        
        if (reg_value_plic != 0) {
            *gpio_out_addr = 0x00000005;  
        }
    */

    while(1);

    return 0;
}
