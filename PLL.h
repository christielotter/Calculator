#ifndef _PLL_H_
#define _PLL_H_

void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Waitms(unsigned long ms);
void SysTick_Waitus(unsigned long us);
#endif
