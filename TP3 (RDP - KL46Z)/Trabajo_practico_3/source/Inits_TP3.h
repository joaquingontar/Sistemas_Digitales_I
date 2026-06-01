/*
 * Inits_RDP_TP3.h
 *
 *  Created on: 8 nov. 2024
 *      Author: Gonzalez Targon, Joaquin
 */


#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"


#ifndef INITS_TP3_H_
#define INITS_TP3_H_

// Uso de "extern": Para usar la variable en otros archivos

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
 * GPIO
 ******************************************************************************/

void board_init(void);

int8_t pulsadorSw3_get(void);

int8_t pulsadorSw1_get(void);

void Manejo_cuenta();

extern int L; // Se activa cuando llego al lugar 9 de la red de petri
extern int Ft2;
extern int Ct2;


/*******************************************************************************
 * CLOCK
 ******************************************************************************/

void init_clock_MCGIRCLK();

/*******************************************************************************
 * Timer SysTick
 ******************************************************************************/

extern int contando; // Bandera que me dice que el timer esta contando para que no se reinicie
extern int Ft2; // Bandera para indicar que pasó 2 segundos

void SysTick_Init(void);

void SysTick_Handler(void);

void Systick_Reset(void);

/*******************************************************************************
 * Timer PIT
 ******************************************************************************/

void PIT_init();

void PIT_IRQHandler(void);

/*******************************************************************************
 * ADC
 ******************************************************************************/

extern float adc_luz_value;
extern float timer_value;
extern int canal_actual;

void ADC_init();

void ADC0_IRQHandler(void);

/*******************************************************************************
 * Timer TPM en modo PWM
 ******************************************************************************/

extern float adc_temp_value;
extern float cmp_value;

void init_PWM();

#endif /* INITS_TP3_H_ */
