#include "TExaS.h"
#include "PLL.h"
#include "LCD.h"
#include "keypad.h"
#include "Inits.h"
#include "Calculations.h"
#include <stdint.h>
#include "math.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define EXPR_MAX 32
bool Ans = false;
float AnsValue;
int main(void){
	char expr[EXPR_MAX + 1];
	int length = 0;
	expr[0] = '\0';		
	// System init
	PLL_Init();
	SysTick_Init();
	PortA_Init();     
	PortB_Init();     
	PortC_Init();
	PortD_Init();
	PortE_Init();
	PortF_Init();
	InitDisplayPort();
  bool shift_check = ShiftActive();	  
  clearDisplayScreen();
  moveDisplayCursor(0,0);
	SysTick_Waitms(20);
	
  while(1){
	char k = KeypadRead();
	bool now_shift = ShiftActive();
  if(now_shift != shift_check){
        shift_indic(now_shift);        
        shift_check = now_shift;
    }
	if(!k) continue;
		
		float result;
		if(k){
			SysTick_Waitms(10);
			while (KeypadRead()) {}

			if (k == '&') {                 // clear
				clearDisplayScreen();
				length = 0;
				expr[0] = '\0';
				moveDisplayCursor(0,0);
				continue;
				}				
			if (k == '#') {                 // backspace
			if (length > 0) {
				length--;
				expr[length] = '\0';
				EraseCharacter();   
		} continue;
		}
	
			if( (k >= '0' && k <= '9') || k=='.' || k=='+' || k=='-' || k=='x' || k=='/' || k=='E'){
				LCD_Data(k);
				if (length < EXPR_MAX + 1) {
						expr[length++] = k;
						expr[length] = '\0';
				}	
			}
			if(k == '='){
				clearDisplayScreen();
			
			if (calculate_expression(expr, &result)){
						AnsValue = result;
						Ans = true;
						clearDisplayScreen();
						moveDisplayCursor(0,0);
						printDisplay("Ans:");
						moveDisplayCursor(3,0);
				  	LCD_PrintFloat(result);
						SysTick_Waitms(20);			
						expr[0]= '\0';
						length = 0;										
				while (1){
					char k2 = KeypadRead();
					length = snprintf(expr, EXPR_MAX + 1, "%.6g", AnsValue);		
					if (k2 == '#' || k2 == '&'){
						clearDisplayScreen();
						SysTick_Waitms(20);									
						expr[0]= '\0';
						length = 0;
						break;							
						}else if (k2=='+' || k2=='-' || k2=='x' || k2=='/' || k2=='E'){
							while (KeypadRead() != 0) {
							SysTick_Waitms(5);					 
								} 
							clearDisplayScreen();
							moveDisplayCursor(0,0);
							printDisplay("Ans");
							LCD_Data(k2);
							if (length < EXPR_MAX) {
								expr[length++] = k2;
								expr[length] = '\0';
							}
							break;
						}
					}
			} 	else {
						clearDisplayScreen();
						moveDisplayCursor(0,0);
						printDisplay("Error");
						length = 0;
						expr[0] = '\0';
						while (KeypadRead() == 0){};
				    SysTick_Waitms(10);
						while (KeypadRead() != 0){};	
						clearDisplayScreen();
						moveDisplayCursor(0,0);	
					  continue;								
					}				
		  }
		}
	}
}


