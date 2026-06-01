

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "Inits_RDP_TP3.h"



/*----------------------------------------------------------------------------
 * FUNCION RED DE PETRI UTILIZANDO LOS TIMERS
 * ---------------------------------------------------------------------------
 */



void RDP(){

	//Marcado inicial
	static int8_t L1=1,L2=0,L3=0,L4=0,L5=0,L6=0,L7=0,L8=0,L9=0; //Se define estatica porque sino cuando salga de la funcion se borra su estado y quiero que se conservar el valor de la variable

	// Entradas
	int8_t P1=pulsadorSw1_get();
	int8_t P2=pulsadorSw3_get();

	//Declaracion de transiciones
	int8_t T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14;

	// Transiciones
	T1=L1 && (!P1) && (!P2);
	T2=L2 && (P1);
	T3=L3 && (!P1);
	T4=L4 && (P2);
	T5=L5 && (!P2);
	T6=L2 && (!P1) && (P2);
	T7=L6 && (!P2);
	T8=L7 && (P1);
	T9=L8 && (!P1);
	T10=L5 && (Ft2);
	T11=L8 && (Ft2);
	T12=L4 && (Ft2);
	T13=L7 && (Ft2);
	T14=L9 && (P1) && (P2);


	//Evolucion marcado
	if(T1){L1=0;L2=1;}
	if(T2){L2=0;L3=1;}
	if(T3){L3=0;L4=1;}
	if(T4){L4=0;L5=1;}
	if(T5){L5=0;L9=1;}
	if(T6){L2=0;L6=1;}
	if(T7){L6=0;L7=1;}
	if(T8){L7=0;L8=1;}
	if(T9){L8=0;L9=1;}
	if(T10){L5=0;L6=1;}
	if(T11){L8=0;L3=1;}
	if(T12){L4=0;L1=1;}
	if(T13){L7=0;L1=1;}
	if(T14){L9=0;L1=1;}


	//Salidas
	if(L9==1){LED_VERDE_ON;LED_ROJO_ON;}
	if (L9==0){LED_VERDE_OFF;LED_ROJO_OFF;}

	if ((L4==1 ||L7==1) && contando==0){ // En estos lugares arranca a contar el Systick
		SysTick_Init();
		contando=1; // Bandera para que no se reinicie el timer
	}
	if (L1 || L9 || L3 || L6) { // Deja de contar el Systick
		Systick_Reset();
		contando=0;
	}
}

/*----------------------------------------------------------------------------
 * FUNCION PRUEBA TIMER SYSTICK
 * ---------------------------------------------------------------------------
 */

void Prueba_Systick(){
	int P1=pulsadorSw1_get();
	int P2=pulsadorSw3_get();

	if (P1){
		LED_VERDE_ON;
		SysTick_Init();
	}
	if (P2 || Ft2){ // P2 para parar con el pulsador y Ft2 cuando termina el tiempo
		LED_VERDE_OFF;
		Systick_Reset();
	}
}

/*----------------------------------------------------------------------------
 * FUNCION RED DE PETRI SIN UTILIZAR LOS TIMERS
 * ---------------------------------------------------------------------------
 */

void RDP_ST(){
	 //Marcado inicial
	 static int8_t L1=1,L2=0,L3=0,L4=0,L5=0,L6=0,L7=0,L8=0,L9=0; //Se define estatica porque sino cuando salga de la funcion se borra su estado y quiero que se conservar el valor de la variable

	 // Entradas
	 int8_t P1=pulsadorSw1_get();
	 int8_t P2=pulsadorSw3_get();

	 //Declaracion de transiciones
	 int8_t T1,T2,T3,T4,T5,T6,T7,T8,T9,T14;

	 // Transiciones
	 T1=L1 && (!P1) && (!P2);
	 T2=L2 && (P1);
	 T3=L3 && (!P1);
	 T4=L4 && (P2);
	 T5=L5 && (!P2);
	 T6=L2 && (!P1) && (P2);
	 T7=L6 && (!P2);
	 T8=L7 && (P1);
	 T9=L8 && (!P1);
	 T14=L9 && (P1) && (P2);

	 //Evolucion marcado
	 if(T1){L1=0;L2=1;}
	 if(T2){L2=0;L3=1;}
	 if(T3){L3=0;L4=1;}
	 if(T4){L4=0;L5=1;}
	 if(T5){L5=0;L9=1;}
	 if(T6){L2=0;L6=1;}
	 if(T7){L6=0;L7=1;}
	 if(T8){L7=0;L8=1;}
	 if(T9){L8=0;L9=1;}
	 if(T14){L9=0;L1=1;}

	 //Salidas
	 if(L9==1){LED_VERDE_ON;LED_ROJO_ON;}
	 if (L9==0){LED_VERDE_OFF;LED_ROJO_OFF;}

}

/*----------------------------------------------------------------------------
 * FUNCION PRUEBA LEDs
 * ---------------------------------------------------------------------------
 */

int b1=0,b2=0;//Bandera que indica que ya se solto el pulsador
int estado_Led1=0, estado_Led2=0;
void prueba_LED(){
	if (pulsadorSw1_get() && b1==0){
		if (estado_Led1==0){
			LED_VERDE_ON;
			b1=1;
			estado_Led1=1;
		}else{
			LED_VERDE_OFF;
			b1=1;
			estado_Led1=0;
		}
	}
	if (pulsadorSw3_get() && b2==0){
		if (estado_Led2==0){
			LED_ROJO_ON;
			b2=1;
			estado_Led2=1;
		}else{
			LED_ROJO_OFF;
			b2=1;
			estado_Led2=0;
		}
	}
	if (!pulsadorSw1_get()){b1=0;}
	if (!pulsadorSw3_get()){b2=0;}
}

int main(void) {
	board_init();
	LED_VERDE_OFF;
	LED_ROJO_OFF;
	init_clock_MCGIRCLK();

	//Prueba Systick de 2 segundos con interrupciones
	//init_systick_interrup();


	for(;;){
    	//Prueba LEDs
    	//prueba_LED();

    	//Prueba RDP sin timers
		//RDP_ST();

		//Prueba Systick
		Prueba_Systick();

		// RPD con timer
		//RDP();

    }

    return 0 ;
}

