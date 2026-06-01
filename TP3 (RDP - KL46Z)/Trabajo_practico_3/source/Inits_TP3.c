/*
 * Inits_TP3.c
 *
 *  Created on: 18 nov. 2024
 *      Author: Gonzalez Targon, Joaquin
 */

#include "Inits_TP3.h"

/*******************************************************************************
 * Inicializaciones Board
 ******************************************************************************/

void board_init(void){
//LED VERDE
SIM ->SCGC5 |= (1<<12); 			//Habilito el clock al puerto D
PORTD ->PCR[5] |= PORT_PCR_MUX(4); 	//EN ESTE CASO CONFIGURO EL PIN 5 CON EL TPM0 (VER MANUAL PAG 175)

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

/*******************************************************************************
 * Timer SysTick
 ******************************************************************************/


void SysTick_Init(void) {
	SysTick->LOAD = 2000000 - 1;       // Cargar para contar 2 seg (2M ciclos de 1us)
	SysTick->VAL = 0;
	SysTick->CTRL = 3;              // Habilitar SysTick, interrupciones y seleccionar el reloj del sistema
}


void Systick_Reset(void){ // Reseteo contador
    SysTick->VAL = 0;
	SysTick -> CTRL &= ~(0x01);
}

void SysTick_Handler(void) { // LLamada a la interrupcion (llego a los 2 segundos)
	Ft2 = 1;
}

void Manejo_cuenta(){
	if(Ct2==1 && contando==0){
		contando=1; // Bandera para que no se reinicie el timer
		SysTick_Init();
	}
	if (Ft2==1 || Ct2==0){
		Systick_Reset();
	}
	if (Ct2==0){contando=0;Ft2=0;}
}


/*******************************************************************************
 * Timer PIT
 ******************************************************************************/

float timer_value;

void PIT_init(){
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	 // turn on PIT
	PIT->MCR = 0x00;
	 // Timer 1
	PIT->CHANNEL[0].LDVAL =  4000000 - 1;

	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // Habilito contador e interrupcion
	NVIC_EnableIRQ(PIT_IRQn);
}

void PIT_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    if (L==1){
    	LED_ROJO_TOGGLE;
    }else{
    	LED_ROJO_OFF;
    }
}

/*******************************************************************************
 * ADC
 ******************************************************************************/


int canal_actual=5;
void ADC_init(){
	SIM->SCGC6 |= 1<<27; //Activo clock (pag 164 del manual)
	//PORTE->PCR[20] = PORT_PCR_MUX(0); // Asigna funcion ADC0_DP3/ADC0_SE3 a PTE22

	ADC0->CFG1 = ADC_CFG1_ADIV(0B11) | ADC_CFG1_ADLSMP_MASK |  ADC_CFG1_MODE(0B10) | ADC_CFG1_ADICLK(0B01);
	/* (MANUAL PAG 479)
	 * DC_CFG1_ADIV(0B11): Divisor de 8
	 * ADC_CFG1_ADLSMP_MASK: Modo muestreo largo, reduce el ruido de señales de alta impedancia
	 * ADC_CFG1_MODE(0B11): Resolucion de 10 bits
	 * ADC_CFG1_ADICLK(0B01):
	 */

	NVIC_EnableIRQ(ADC0_IRQn); //Habilito las interrupciones para el ADC0
	ADC0->SC3 |= ADC_SC3_ADCO_MASK; // Activa el modo de conversión continua
	NVIC_SetPriority(ADC0_IRQn, 0); //prioridad de la IRQ15 (valor por defecto en este caso)


	PORTE->PCR[17] |= 0x300; // Pongo el MUX del pin 17 puerto E en la alternativa ADC0 SE canal 5

	ADC0->SC1[0] = ADC_SC1_ADCH(5) | ADC_SC1_AIEN_MASK;
	// Inicio conversion por canal 5 (sensor de temperatura) habilitando interrupciones (Pag 477 del manual)
}

float adc_temp_value;
float adc_luz_value;

void ADC_luz(){
	   /*
		* El valor que me devuelve el ADC esta variando en un rango de
		*  1000 con luz apagada,
		*  500 a 800 con luz normal ambiente
		*  y puede bajar hasta 60 si le agrego luz
		*/
	   if(adc_luz_value<900){
		   timer_value = ( adc_luz_value*8000);
	   }
	   if(adc_luz_value>900){ // Para que sea mas apreciable en la oscuridad
		   timer_value = ( adc_luz_value*9000);
	   }
	  PIT->CHANNEL[0].LDVAL =  timer_value;

}


void ADC_temp(){
	/*
	* Valores que me devuevuelve el ADC
	* 533 a temperatura ambiente
	* Si subo la temperatura con un encendedor va hasta 580
	*/
	int escala_temp = 500; // Cambiar este valor segun la temperatura del dia
	cmp_value=(adc_temp_value-escala_temp);

	if (adc_temp_value<540){
		cmp_value=cmp_value*0.7;
	}
	if (adc_temp_value>540 && adc_temp_value<580){
		cmp_value=cmp_value*1;
	}
	if (adc_temp_value>580){
		cmp_value=cmp_value*1.2;
	}
	TPM0->CONTROLS[5].CnV = cmp_value;
}

void ADC0_IRQHandler(void) {

	if (canal_actual==5){ //Verifico que sea el canal 5 (de temperatura) por el que salto la interrupcion
		adc_temp_value = ADC0->R[0]; // Lee el valor de la conversión limpia la bandera automáticamente
		ADC_temp(); // Varia el valor del CMP
		ADC0->SC1[0] = ADC_SC1_ADCH(3) | ADC_SC1_AIEN_MASK;
		// Inicio conversion por canal 3 (sensor de luz) habilitando interrupciones
		canal_actual=3;
	}else{
		adc_luz_value = ADC0->R[0]; // Lee el valor de la conversión limpia la bandera automáticamente
		ADC_luz(); // Vario el tiempo del PIT
		ADC0->SC1[0] = ADC_SC1_ADCH(5) | ADC_SC1_AIEN_MASK;
		// Inicio conversion por canal 5 (sensor de luz) habilitando interrupciones
		canal_actual=5;
	}
}

/*******************************************************************************
 * Timer TPM en modo PWM
 ******************************************************************************/

float cmp_value;

void init_PWM(){
// Activo clockal TPM0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

// Configura TPMSRC a 01 (MCGFLLCLK)
	SIM->SOPT2 |= (0x03<<24); //Utilizar clock MCGIRCLK de 1MHz


// Seteo el valor en el que el temporizador cambia de estado en el canal correspondiente. Ejemplo:
	TPM0->CONTROLS[5].CnV = cmp_value; // Para usar el led verde con TPM0 debo seleccionar canal 5 en el mux (pag 175 del manual)

// Calculo el valor de MOD en base a la frecuencia que tengo y al PWM que busco. Ejemplo:
	TPM0->MOD = 100;

// Configuro el prescaler (TPM0->SC). Ejemplo:
	TPM0->SC |= 0;          // Configura prescaler a 1 (sin dividir)
	TPM0->SC |= (1 << 3);   // Establece CMOD en 01 para habilitar el contador
	//TPM0->SC |= (1 << 6);   // Habilita TOIE para interrupciones de desbordamiento
	TPM0->SC = TPM_SC_PS(0);	//Prescaler 1

//Modifico el status and control del canal (PAG 569 DEL MANUAL)
	TPM0->CONTROLS[5].CnSC |= 0x24; // Opcion PWM y pulsos bajos (tambien podrian ser altos)

}
