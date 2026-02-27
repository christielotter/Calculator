#include "PLL.h"
#include <stdbool.h>
//===== DEFINITIONS =====//

//Port A== LCD RS & EN
#define SYSCTL_RCGCG2_R        (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTA_DATA_R      (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R       (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R     (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R       (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_CR_R        (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R     (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R      (*((volatile unsigned long *)0x4000452C))

//PORT B== LCD DB
#define GPIO_PORTB_DATA_R      (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R       (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R     (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R       (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_CR_R        (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R     (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R      (*((volatile unsigned long *)0x4000552C))

#define GPIO_PORTC_DATA_R      (*((volatile unsigned long *)0x400063FC))
#define GPIO_PORTC_DIR_R       (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_AFSEL_R     (*((volatile unsigned long *)0x40006420))
#define GPIO_PORTC_DEN_R       (*((volatile unsigned long *)0x4000651C))
#define GPIO_PORTC_CR_R        (*((volatile unsigned long *)0x40006524))
#define GPIO_PORTC_AMSEL_R     (*((volatile unsigned long *)0x40006528))
#define GPIO_PORTC_PCTL_R      (*((volatile unsigned long *)0x4000652C))


//PORT D== KEYPAD COLUMNS
#define GPIO_PORTD_DATA_R      (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R       (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R     (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DEN_R       (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_CR_R        (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R     (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R      (*((volatile unsigned long *)0x4000752C))

//PORT E== KEYPAD ROWS
#define GPIO_PORTE_DATA_R      (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R       (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R     (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PDR_R       (*((volatile unsigned long *)0x40024514))   // Keypad row inputs require pull-down resistors
#define GPIO_PORTE_DEN_R       (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_CR_R        (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R     (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R      (*((volatile unsigned long *)0x4002452C))

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
	
//===== MASKS =====//
#define LCD_RS_PIN   (1U << 3)   // PA3
#define LCD_EN_PIN   (1U << 2)   // PA2


void PortA_Init (void){
	SYSCTL_RCGCG2_R |= 0x01;
	SysTick_Waitus(5);
	GPIO_PORTA_CR_R |= (LCD_RS_PIN | LCD_EN_PIN);
	GPIO_PORTA_AMSEL_R &= ~(LCD_RS_PIN | LCD_EN_PIN);
	GPIO_PORTA_PCTL_R  &= ~0x0000FF00; 
	GPIO_PORTA_DIR_R   |= (LCD_RS_PIN | LCD_EN_PIN);
	GPIO_PORTA_AFSEL_R &= ~(LCD_RS_PIN | LCD_EN_PIN);
	GPIO_PORTA_DEN_R   |= (LCD_RS_PIN | LCD_EN_PIN);
};
void PortB_Init (void){
	SYSCTL_RCGCG2_R |= 0x02;
	SysTick_Waitus(5);
	GPIO_PORTB_CR_R = 0xFF;
	GPIO_PORTB_AMSEL_R = 0x00;
	GPIO_PORTB_PCTL_R  = 0x00000000; 
	GPIO_PORTB_DIR_R   |= 0x000000FF;
	GPIO_PORTB_AFSEL_R &= ~0xFF;
	GPIO_PORTB_DEN_R   |= 0xFF;
};
#define COL_PC_PINS  ((1U<<5)|(1U<<6))   // PC5 + PC6

void PortC_Init(void){
  SYSCTL_RCGCG2_R |= 0x04;
  SysTick_Waitus(5);

  GPIO_PORTC_AFSEL_R &= ~COL_PC_PINS;
  GPIO_PORTC_AMSEL_R &= ~COL_PC_PINS;
  GPIO_PORTC_PCTL_R  &= ~0x0FF00000;    

  GPIO_PORTC_DIR_R   |= COL_PC_PINS;     // outputs
  GPIO_PORTC_DEN_R   |= COL_PC_PINS;     // digital enable

  GPIO_PORTC_DATA_R  &= ~COL_PC_PINS;   
}

#define KEYPAD_COL_D_PINS  0x0CU   // PD0–PD3
void PortD_Init(void){
  SYSCTL_RCGCG2_R |= 0x08;               
  SysTick_Waitus(5);

  GPIO_PORTD_AFSEL_R &= ~KEYPAD_COL_D_PINS;
  GPIO_PORTD_AMSEL_R &= ~KEYPAD_COL_D_PINS;
  GPIO_PORTD_PCTL_R  &= ~0x0000FF00;     
  GPIO_PORTD_DIR_R   |= KEYPAD_COL_D_PINS; 
  GPIO_PORTD_DEN_R   |= KEYPAD_COL_D_PINS;
  GPIO_PORTD_DATA_R  |= KEYPAD_COL_D_PINS;
}

#define KEYPAD_ROW_PINS 0x0F   // PE0–PE3

void PortE_Init(void){
    SYSCTL_RCGCG2_R |= 0x10;               
    SysTick_Waitus(5);
    GPIO_PORTE_AFSEL_R &= ~KEYPAD_ROW_PINS; 
    GPIO_PORTE_AMSEL_R &= ~KEYPAD_ROW_PINS;  
    GPIO_PORTE_PCTL_R  &= ~0x0000FFFF;       
    GPIO_PORTE_DIR_R   &= ~KEYPAD_ROW_PINS; 
    GPIO_PORTE_PDR_R   |=  KEYPAD_ROW_PINS;  // enable pull-downs
    GPIO_PORTE_DEN_R   |=  KEYPAD_ROW_PINS;  // digital enable
}

void PortF_Init(void){ 
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  SysTick_Waitus(5);        // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
void shift_indic(bool shift_active){   
if (shift_active){
	  GPIO_PORTF_DATA_R |= 0x06;
                } else{                  
      GPIO_PORTF_DATA_R = 0x00; 
    }	
  }

