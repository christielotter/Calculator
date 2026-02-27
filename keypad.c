#include <stdint.h>
#include "PLL.h"
#include "LCD.h"
#include <stdbool.h>

// DATA REGISTERS 
#define GPIO_PORTC_DATA_R   (*((volatile unsigned long *)0x400063FC))
#define GPIO_PORTD_DATA_R   (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTE_DATA_R   (*((volatile unsigned long *)0x400243FC))

// Declares
static const uint8_t column_C[2] = { (1U<<5), (1U<<6) }; // PC5, PC6
static const uint8_t column_D[2] = { (1U<<2), (1U<<3) }; // PD2, PD3
//Need this for LCD indicator
volatile bool shift_active = false;
// Keypad map
static const char keymap[4][4] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','.'},
  {'=','0','#','D'}
};

static const char keymap_shift[4][4] = {
  {'1','2','3','x'},
  {'4','5','6','/'},
  {'7','8','9','E'},
  {'=','0','&','D'}
};

// Read the rows
static uint8_t ReadRows(void){
    return (uint8_t)(GPIO_PORTE_DATA_R & 0x0F);
}

// Decode read rows into values for the column scan
static uint8_t decode_row(uint8_t rows){
    if(rows == 0x01) return 0;
    if(rows == 0x02) return 1;
    if(rows == 0x04) return 2;
    if(rows == 0x08) return 3;
    return 0xFF;
}

// Read Keypad
char KeypadRead(void){
  // Port D (COL2, COL3) 
    for(uint8_t c = 0; c < 2; c++){
			
			GPIO_PORTC_DATA_R &= ~((1U<<5)|(1U<<6));
			GPIO_PORTD_DATA_R &= ~((1U<<2)|(1U<<3));

        GPIO_PORTD_DATA_R = (GPIO_PORTD_DATA_R & ~((1U<<2)|(1U<<3))) | column_D[c];
        SysTick_Waitms(10);

        uint8_t r = decode_row(ReadRows());
        if(r != 0xFF){
					if (r == 3 && c == 1){
						shift_active = !shift_active;
						SysTick_Waitms(10);        
						while (ReadRows() != 0) {
							SysTick_Waitms(5);					 
					} 
						return 0; 
				}
					if (shift_active){						
						return keymap_shift[r][c+2];
					}
          else {			
					return keymap[r][c+2];
				}					
        } 
    }
    // Port C (COL0, COL1)
    for(uint8_t c = 0; c < 2; c++){
			GPIO_PORTC_DATA_R &= ~((1U<<5)|(1U<<6));
			GPIO_PORTD_DATA_R &= ~((1U<<2)|(1U<<3));

        GPIO_PORTC_DATA_R = (GPIO_PORTC_DATA_R & ~((1U<<5)|(1U<<6))) | column_C[c];
        SysTick_Waitms(10);
        uint8_t r = decode_row(ReadRows());
        if(r != 0xFF){
					if (shift_active){
						return keymap_shift[r][c];
					}
           else return keymap[r][c];
        }
    }
    // Return all columns to low
    GPIO_PORTC_DATA_R &= ~((1U<<5)|(1U<<6));
    GPIO_PORTD_DATA_R &= ~((1U<<2)|(1U<<3));

    return 0;
}

bool ShiftActive(void){
	return shift_active;
}


