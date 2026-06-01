/*
 ============================================================================
 Name        : Trabajo practico 3
 Author      : Gonzalez Targon, Joaquin
 Descripcion :
 Si se aprietan los dos pulsadores en un lapso de 2 segundos se prenden ambos leds
 El led rojo variara su intensidad segun la temperatura y el led verde variara su frecuencia de parpadeo segun la luz
 Al apretar ambos switchs a la vez se apagan ambos led
 ============================================================================
 */

#include "Inits_TP3.h"
#include "MKL46Z4.h"



int L=0; // Variable que determina que se llego al lugar 9
int Ct2=0;
int contando=0;
int Ft2=0;

void RDP(){

	//Marcado inicial
	static int8_t L1=1,L2=0,L3=0,L4=0,L5=0,L6=0,L7=0,L8=0,L9=0; //Se define estatica porque sino cuando salga de la funcion se borra su estado y quiero que se conservar el valor de la variable

	// Entradas
	int8_t P1=pulsadorSw1_get();
	int8_t P2=pulsadorSw3_get();
	int tiempo=Ft2;

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
	T10=L5 && (tiempo);
	T11=L8 && (tiempo);
	T12=L4 && (tiempo);
	T13=L7 && (tiempo);
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
	L=L9;
	Ct2= L4 || L7 || L5 || L8;

}



int main(void) {
	board_init();
	LED_VERDE_OFF;
	LED_ROJO_OFF;
	init_clock_MCGIRCLK();
	PIT_init();
	ADC_init();
	init_PWM();

	for(;;){
		RDP();
		Manejo_cuenta();
		if (L==1){
					TPM0->SC =TPM_SC_CMOD(1);  // Modo de contador habilitado
				}else{
					TPM0->SC =TPM_SC_CMOD(0);  // Modo de contador habilitado
				}
				if (L==0){
					LED_ROJO_OFF;
					LED_VERDE_OFF;
				}

    }

    return 0 ;
}

