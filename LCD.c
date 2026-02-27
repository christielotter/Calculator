#include "PLL.h"
#include "LCD.h"
#include <stdint.h>
//Port A== LCD RS & EN
#define GPIO_PORTA_DATA_R      (*((volatile unsigned long *)0x400043FC))

//PORT B== LCD DB
#define GPIO_PORTB_DATA_R      (*((volatile unsigned long *)0x400053FC))

//MASKS//
#define LCD_RS_PIN   (1U << 3)   // PA3
#define LCD_EN_PIN   (1U << 2)   // PA2
#define LCD_DB_PINS  (0xF0U)     // bits 4,5,6,7

static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

void pulseEnable(void){
	//EN=1
	GPIO_PORTA_DATA_R |= LCD_EN_PIN;
	//12.5 * 36 = 450	ns
	SysTick_Wait(36);
	//EN=0
	GPIO_PORTA_DATA_R &= ~LCD_EN_PIN;
		
	SysTick_Wait(36);
};
void SendDisplayNibble(uint8_t nibble, uint8_t rs){
	if(rs){
		GPIO_PORTA_DATA_R |= LCD_RS_PIN;
	}else{
		GPIO_PORTA_DATA_R &= ~LCD_RS_PIN;
	}
	
	//Clear DB pins
	GPIO_PORTB_DATA_R &= ~LCD_DB_PINS;
	GPIO_PORTB_DATA_R |= ((nibble & 0x0F) <<4);
	
	pulseEnable();
};

void SendtoDisplay(uint8_t value, uint8_t rs){
	SendDisplayNibble(value >> 4, rs);
	
	SendDisplayNibble(value & 0x0F, rs);
	
	SysTick_Waitus(40);
};

void InitDisplayPort(void){
	SysTick_Waitms(50);
	SendDisplayNibble(0x3, 0);
	SysTick_Waitms(5);
	SendDisplayNibble(0x3, 0);
	SysTick_Waitus(150);
	SendDisplayNibble(0x3, 0);
	SysTick_Waitus(150);
	SendDisplayNibble(0x2, 0);
	SysTick_Waitus(150);
	SendtoDisplay(0x28, 0);   
	SendtoDisplay(0x08, 0);   
	SendtoDisplay(0x01, 0);   
	SysTick_Waitms(2);          
	SendtoDisplay(0x06, 0);  
	SendtoDisplay(0x0C, 0);   
};



void LCD_Command(uint8_t cmd){
    SendtoDisplay(cmd, 0);    
};

void LCD_Data(uint8_t data){
    SendtoDisplay(data, 1);     
		cursor_col++;	
};

void clearDisplayScreen(void){
	LCD_Command(0x01);   // Clear display
	SysTick_Waitms(20);
	moveDisplayCursor(0, 0);
	cursor_col=0; cursor_row=0;
};

void moveDisplayCursor(uint8_t row, uint8_t col){
	uint8_t address;
	cursor_row = row;
  cursor_col = col;
	switch(row){
		case 0: address = 0x00 + col; break;
		case 1: address = 0x40 + col; break;
		case 2: address = 0x14 + col; break;
		case 3: address = 0x54 + col; break;
		default: return;
	}
	
	LCD_Command(0x80 | address);
	SysTick_Waitus(40);
	
};

void printDisplay(const char *string){
	while(*string){
		LCD_Data(*string++);
	}
};
void EraseCharacter (void){
		cursor_col--;
	  LCD_Command(0x10);
		SysTick_Waitms(5);
    LCD_Data(' ');     
		SysTick_Waitus(10);
    LCD_Command(0x10); 
};

void LCD_PrintFloat(float value){
    int i;
    char buf[12];  
// For negatives
    if (value < 0){
        LCD_Data('-');
        value = -value;
    }
    int intPart = (int)value;
    int fracPart = (int)((value - intPart) * 10000 + 0.5f); // 4 dp
		
		if (fracPart >= 10000){
			fracPart = 0;
			intPart +=1;
		}
    if (intPart == 0){
        LCD_Data('0');
    } else {
        i = 0;
        while (intPart > 0){
            buf[i++] = (intPart % 10) + '0';
            intPart /= 10;
        }
        while (i--)
            LCD_Data(buf[i]);
    }
		if (fracPart == 0)
    return;
    LCD_Data('.');
		
    int div = 1000;
    while (div > 1 && (fracPart % 10) == 0){
        fracPart /= 10;
        div /= 10;
    }
    while (div > 0){
        LCD_Data((fracPart / div) + '0');
        fracPart %= div;
        div /= 10;
    }
}

