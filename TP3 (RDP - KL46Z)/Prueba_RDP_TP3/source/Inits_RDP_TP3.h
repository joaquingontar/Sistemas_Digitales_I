/*
 * Inits_RDP_TP3.h
 *
 *  Created on: 8 nov. 2024
 *      Author: Grupo 9 - Gonzalez Targon, Dellabianca
 */

//LAS INICIALIZACIONES ESTAN HECHAS PARA LA KL46

#ifndef INITS_RDP_TP3_H_
#define INITS_RDP_TP3_H_

/*******************************************************************************
 * MACROS
 ******************************************************************************/
#define LED_VERDE_ON PTD->PCOR |= (1 << 5)
#define LED_VERDE_OFF PTD->PSOR |= (1 << 5)
#define LED_VERDE_TOGGLE PTD->PTOR |= (1 << 5)
#define LED_ROJO_ON PTE->PCOR |= 1 << 29
#define LED_ROJO_OFF PTE->PSOR |= 1 << 29
#define LED_ROJO_TOGGLE PTE->PTOR |= 1 << 29

/*******************************************************************************
 *INICIALIZACIONES
 ******************************************************************************/
void board_init(void){
//LED VERDE
SIM ->SCGC5 |= (1<<12); 			//Habilito el clock al puerto D
PORTD ->PCR[5] |= PORT_PCR_MUX(1); 	//Configuro el pin 5 del puerto D para que sea GPIO
PTD ->PDDR |= (1<<5); 				//Configuro el pin 5 del puerto D como salida
PTD -> PSOR |= (1 << 5);			// Reseteo la salida para prender el LED


//LED ROJO
SIM ->SCGC5 |= (1 << 13); 			//Habilito el clock al puerto E
PORTE ->PCR[29] |= 1 << 8; 			//Configuro el pin 29 del puerto E para que sea GPIO
PTE -> PDDR |= (1 << 29); 			//Configuro el pin 29 del puerto E como salida
PTE -> PSOR |= (1 << 29);			// Seteo la salida para apagar el LED


//SW3
SIM ->SCGC5 |= SIM_SCGC5_PORTC_MASK; // CLOCK
PORTC ->PCR[12] |= PORT_PCR_MUX(1); // GPIO
PTC ->PDDR  &= ~(1<<12);
PORTC ->PCR[12] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

//SW1
SIM ->SCGC5 |= SIM_SCGC5_PORTC_MASK; // CLOCK
PORTC -> PCR[3] |= PORT_PCR_MUX(1); // GPIO
PTC -> PDDR &= ~ (1<<3);
PORTC -> PCR[3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

}

int8_t pulsadorSw3_get(void){
	return(PTC->PDIR & (1<<12))?0:1;
}

int8_t pulsadorSw1_get(void){
	return(PTC->PDIR & (1<<3))?0:1;
}



void init_clock_MCGIRCLK(){
	MCG->C1 |= 0x02; //Habilito clock MCGIRCLK
	MCG->C2 |= 0x01; // Selecciono el reloj de 4MHz
	MCG->SC=0x4; // Utilizo el divisor de 4 para obtener un clock de 1Mhz (ciclo de 1us)
}


volatile int contando=0; // Bandera que me dice que el timer esta contando para que no se reinicie

void SysTick_Init(void) {
	if (contando==0){
		SysTick->LOAD = 2000000 - 1;       // Cargar para contar 2 seg (2M ciclos de 1us)
		SysTick->VAL = 0;
		SysTick->CTRL = 3;              // Habilitar SysTick, interrupciones y seleccionar el reloj del sistema
	}
}

volatile int Ft2 = 0;              // Bandera para indicar que pasó 2 segundos

void SysTick_Handler(void) { // LLamada a la interrupcion (llego a los 2 segundos)
	Ft2 = 1;
    }

void Systick_Reset(void){ // Reseteo contador
    SysTick->VAL = 0;
    Ft2=0;
}


#endif /* INITS_RDP_TP3_H_ */
