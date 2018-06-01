// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#include "Nokia5110.h"
#include <time.h>
#include <stdlib.h>



#define GPIO_PORTD_DATA_BITS_R  ((volatile unsigned long *)0x40007000)
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_IS_R         (*((volatile unsigned long *)0x40007404))
#define GPIO_PORTD_IBE_R        (*((volatile unsigned long *)0x40007408))
#define GPIO_PORTD_IEV_R        (*((volatile unsigned long *)0x4000740C))
#define GPIO_PORTD_IM_R         (*((volatile unsigned long *)0x40007410))
#define GPIO_PORTD_RIS_R        (*((volatile unsigned long *)0x40007414))
#define GPIO_PORTD_MIS_R        (*((volatile unsigned long *)0x40007418))
#define GPIO_PORTD_ICR_R        (*((volatile unsigned long *)0x4000741C))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DR2R_R       (*((volatile unsigned long *)0x40007500))
#define GPIO_PORTD_DR4R_R       (*((volatile unsigned long *)0x40007504))
#define GPIO_PORTD_DR8R_R       (*((volatile unsigned long *)0x40007508))
#define GPIO_PORTD_ODR_R        (*((volatile unsigned long *)0x4000750C))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_PDR_R        (*((volatile unsigned long *)0x40007514))
#define GPIO_PORTD_SLR_R        (*((volatile unsigned long *)0x40007518))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
#define GPIO_PORTD_ADCCTL_R     (*((volatile unsigned long *)0x40007530))
#define GPIO_PORTD_DMACTL_R     (*((volatile unsigned long *)0x40007534))




#define NVIC_EN0_R							(*((volatile unsigned long *)0xE000E100))
#define PD0							 				(*((volatile unsigned long *)0x40007004))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R   		  (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R  		  (*((volatile unsigned long *)0xE000E018))
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define NVIC_SYS_PRI3_R					(*((volatile unsigned long *)0xE000ED20))
#define NVIC_PRI0_R 						(*((volatile unsigned long *)0xE000E400))
#define GPIO_PORTA_IM_R					(*((volatile unsigned long *)0x40004410))

// 2. Declarations Section
//   Global Variables


int RandomCounter=2500;
volatile unsigned long Score=0;
char ScoreMs[4];
 int shifter;
int IsReady=1;
int FalseStart=0;
int Player1Score=0;
int Player2Score=0;
int Counter;
volatile unsigned long IsWin=1;



void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2          
  NVIC_ST_CTRL_R = 0x07; // enable SysTick with core clock and interrupts
  // enable interrupts after all initialization is finished
}

void EnableInterrupts(void);	
void WaitForInterrupt(void); 


char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}


void PortA_Init(void)
{
	
	
	
	
	
	
	SYSCTL_RCGC2_R |= 0x00000008; // activate port A 
	GPIO_PORTD_AMSEL_R = 0x3;    // no analog
  GPIO_PORTD_PCTL_R &= ~0x0FFFF0FF; // regular function
  GPIO_PORTD_DIR_R &= ~0x03;       // make pd1,pd0 input  		
  GPIO_PORTD_AFSEL_R &= ~0x78;  // disable alt funct 
  GPIO_PORTD_DEN_R |= 0x3;     // enable digital I/O 
	GPIO_PORTD_IS_R &= ~0x08;    //PA3 edge sensetive
	GPIO_PORTD_IBE_R&= ~0x03;	  	 //pd0,pd1 is not both edges
	GPIO_PORTD_IEV_R |= 0x03;    //PA3 rising edge event
	GPIO_PORTD_IM_R |= 0x03;      //PA3 rising edge event
	GPIO_PORTD_ICR_R = 0x03;			//PA3 clear flag
	
	NVIC_EN0_R = 0x08;  //enable interrupts on portA
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
	NVIC_PRI0_R =  (NVIC_PRI0_R & 0xFFFFFF0F)|0x20;
 
}


void GPIOPortD_Handler(void){ 


		GPIO_PORTD_ICR_R = 0x03;
	if(FalseStart==0)
	{
	
	if(GPIO_PORTD_DATA_R == 0x01)
	{
	if(IsReady==1 && Score<100 )
		{Nokia5110_SetCursor(1, 1);
	Nokia5110_OutString("P1");
	Nokia5110_SetCursor(1, 2);	
	Nokia5110_OutString("P1 False   Start P2WIN");
			FalseStart=1;
		}
			else
			{
					if	(Player1Score==0)
				{
					Player1Score=Score;
	Nokia5110_SetCursor(1, 1);
	Nokia5110_OutString("P1");
	Nokia5110_SetCursor(1, 2);	
	Nokia5110_OutString(itoa(Player1Score,ScoreMs));
		if(IsWin==1)
		{
			Nokia5110_SetCursor(1, 3);	
			Nokia5110_OutString("P1 Win");
			IsWin=0;
		}
	}
	}
}
		if(GPIO_PORTD_DATA_R == 0x02)
	{
		if(IsReady==1 && Score<100)
		{Nokia5110_SetCursor(1, 1);
	Nokia5110_OutString("P2");
	Nokia5110_SetCursor(1, 2);	
	Nokia5110_OutString("P2 False   Start P1 WIN");
   	FalseStart=1;
		}
			else
			{
			if	(Player2Score==0)
				{
					Player2Score=Score;
	Nokia5110_SetCursor(6, 1);
	Nokia5110_OutString("P2");
	Nokia5110_SetCursor(6, 2);	
	Nokia5110_OutString(itoa(Player2Score,ScoreMs));
			if(IsWin==1)
		{
			Nokia5110_SetCursor(1, 3);	
			Nokia5110_OutString("P2 Win");
			IsWin=0;
		}
	}
	}
	}
}
	
	 
}


//SysTick timer interruptu her 1ms de bir kez aktif oluyor
void SysTick_Handler(void){
 
Counter++;;
if(RandomCounter==Counter)
  {
		Nokia5110_SetCursor(1, 0);
		Nokia5110_OutString("    PRESS");
		IsReady=0;
		RandomCounter=0xFFFFFFFF;
		Score=0;
		RandomCounter=-1;
	}
	if (RandomCounter==-1)
	{
		Score++;
	}
}



int main(void){ // running at 16 MHz
  PortA_Init(); //portA kurulumu
 SysTick_Init(16000);        // initialize SysTick timer, every 0,5 s
 
	Nokia5110_Init();
  Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();      // draw buffer

 //  enable after everything initialized
	Nokia5110_SetCursor(1, 0);
  Nokia5110_OutString("GET READY");
	 srand(9);                        
   RandomCounter=((rand()%9)+1)*1000;
	
	EnableInterrupts();  
  while(1){  
		WaitForInterrupt();
}
}