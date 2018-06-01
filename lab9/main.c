// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#include "Nokia5110.h"
#include <stdlib.h>

//*****************************************************************************
//
// GPIO registers (PORTD)
//
//*****************************************************************************
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
	

//*****************************************************************************
//
// GPIO registers (PORTB)
//
//*****************************************************************************
#define GPIO_PORTB_DATA_BITS_R  ((volatile unsigned long *)0x40005000)
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_IS_R         (*((volatile unsigned long *)0x40005404))
#define GPIO_PORTB_IBE_R        (*((volatile unsigned long *)0x40005408))
#define GPIO_PORTB_IEV_R        (*((volatile unsigned long *)0x4000540C))
#define GPIO_PORTB_IM_R         (*((volatile unsigned long *)0x40005410))
#define GPIO_PORTB_RIS_R        (*((volatile unsigned long *)0x40005414))
#define GPIO_PORTB_MIS_R        (*((volatile unsigned long *)0x40005418))
#define GPIO_PORTB_ICR_R        (*((volatile unsigned long *)0x4000541C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DR2R_R       (*((volatile unsigned long *)0x40005500))
#define GPIO_PORTB_DR4R_R       (*((volatile unsigned long *)0x40005504))
#define GPIO_PORTB_DR8R_R       (*((volatile unsigned long *)0x40005508))
#define GPIO_PORTB_ODR_R        (*((volatile unsigned long *)0x4000550C))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))
#define GPIO_PORTB_SLR_R        (*((volatile unsigned long *)0x40005518))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_ADCCTL_R     (*((volatile unsigned long *)0x40005530))
#define GPIO_PORTB_DMACTL_R     (*((volatile unsigned long *)0x40005534))



#define NVIC_EN0_R		(*((volatile unsigned long *)0xE000E100))
#define PD0			(*((volatile unsigned long *)0x40007004))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R   	(*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R  	(*((volatile unsigned long *)0xE000E018))
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define NVIC_SYS_PRI3_R		(*((volatile unsigned long *)0xE000ED20))
#define NVIC_PRI0_R 		(*((volatile unsigned long *)0xE000E400))
#define GPIO_PORTA_IM_R		(*((volatile unsigned long *)0x40004410))

// 2. Declarations Section
//   Global Variables

char sendChar;//Gönderilecek karakter
char reciveChar;//alınan karakter
int flag;//Gönderilecek bit
int i=0;
int j=-1;
int k=0;
int ClockFlag=1;//İki port arasındaki clocku 1 ve 0 a set etmek için kullanılan değişken
int SystickCounter =0;//Systick interruptunun kaç defa çalıştığını sayan değişken
char message[20]= "mal akif";

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




void Port_Init(void)
{
	
	SYSCTL_RCGC2_R |= 0x0000000A; // activate port D and B
	GPIO_PORTD_AMSEL_R = 0x3;    // no analog
  	GPIO_PORTD_PCTL_R &= ~0x0FFFF0FF; // regular function
  	GPIO_PORTD_DIR_R &= ~0x03;       // make pd1,pd0 input  		
  	GPIO_PORTD_AFSEL_R &= ~0x3;   // disable alt funct 
  	GPIO_PORTD_DEN_R |= 0x3;      // enable digital I/O 
	GPIO_PORTD_IS_R &= ~0x01;    //PD0 edge sensetive
	GPIO_PORTD_IBE_R&= ~0x01;   //pd0,pd1 is not both edges
	GPIO_PORTD_IEV_R &= ~0x01;    //PD Falling edge
	GPIO_PORTD_IM_R |= 0x01;      //PD
	GPIO_PORTD_ICR_R = 0x01;			//PD clear flag
	GPIO_PORTD_PUR_R |= 0x03;    //Activate Pull up resistors on PD1 and PD2
	GPIO_PORTD_DATA_R = 0x00;
	
	GPIO_PORTB_AMSEL_R = 0x3;    // no analog
  	GPIO_PORTB_PCTL_R &= ~0x0FFFF0FF; // regular function
 	 GPIO_PORTB_DIR_R |= 0x03;       // make pd1,pd0 input  		
 	 GPIO_PORTB_AFSEL_R &= ~0x3;  // disable alt funct 
 	 GPIO_PORTB_DEN_R |= 0x3;     // enable digital I/O 

	
	
	
	
	NVIC_EN0_R = 0x08;  //enable interrupts on portD
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
	NVIC_PRI0_R =  (NVIC_PRI0_R & 0xFFFFFF0F)|0x20;
 
}



//set edilen clokc 1den 0 geçtiği anda pd1 deki veriyi okuyor.
void GPIOPortD_Handler(void){ 
		GPIO_PORTD_ICR_R = 0x01;//Clear flag
	
	  	j = j%8;	  
		reciveChar += ((GPIO_PORTD_DATA_R & 0x2) >> 1)<<j ;	
		j++;

	
}


//iki port arasındaki clocku set ediyor ve rising edge'de göndereceği biti porta yazıyor.
void SysTick_Handler(void){
  SystickCounter++;
	if(ClockFlag == 1 && sendChar!= 0)//veriyi yolla clock 1e set et
	{
	 
	  
    		flag = sendChar & 1;
		sendChar = sendChar >> 1;
	  
		if(flag == 1)
				GPIO_PORTB_DATA_R = 0x3;
		else
				GPIO_PORTB_DATA_R = 0x1;
	
		
		ClockFlag=0;
	}
	else//Clock 0a set et
	{
		GPIO_PORTB_DATA_R&= ~0x1;
		ClockFlag=1;
	}
}
	
	




int main(void){ // running at 16 MHz
  	Port_Init(); //portA kurulumu
	Nokia5110_Init();
  	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	EnableInterrupts();  
  while(1){  
		
		
		while(message[k] != '\0')//Stringin son char'ına kadar devam et
		{
		  	sendChar=message[k];//Mevcut char değişkenini göndermek için ata
			reciveChar=0;//Alınan charı 0la
			SystickCounter=0;//Counterı 0la
			SysTick_Init(16000); //Timerı set et(interrupt ile birlikte)
			while(SystickCounter!=16)//Char gönderilene kadar bekle
			{
				
				
				
			}
			NVIC_ST_CTRL_R = 0; //timerı kapat
			Nokia5110_SetCursor(k+1, 0);
			Nokia5110_OutString(&reciveChar);//Alınan karakteri ekrana yaz
			j=0;
			i=0;
			k++;
			
			
		}
		
		
		
		
}
}
