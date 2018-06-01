// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define PD0											(*((volatile unsigned long *)0x40007004))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R   		  (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R  		  (*((volatile unsigned long *)0xE000E018))
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))

			
			
			
// 2. Declarations Section
//   Global Variables

int WhichLed=99999998;  //Hangi ledin yanacagina karar veren degisken 
int Switch;            //0 veya 1 olmasi ledlerin yanma sirasini belirliyor
int IsItFirst;        //LED yanarken sadece  butona ilk basmayi degerlendirmek icin
int j;
			


//   Function Prototypes
void PortF_Init(void);
void EnableInterrupts(void);
void TurnOnLed(int x);
//void PLL_Init(void);
void SysTick_Init(void);
void SysTick_Wait10ms(unsigned long delay);
void SysTick_Wait(unsigned long delay);


int main(void)
{    	
  PortF_Init();        // Call initialization of port PD0,PD1,PD2 and PD3 
  EnableInterrupts();  // The grader uses interrupts
	SysTick_Init();      //SysTick baslangic ayarlari
	//PLL_Init();					//PLL kurulumu

  while(1)
	{
  	IsItFirst = 1;  	//Renk degistigi icin buttonu aktif hale getiriyoruz
		if(Switch==0x00)	//Switch 0 ise WhichLed degiskenini arttiriyor degilse azaltiyor
				WhichLed++;
		else
				WhichLed--;

			TurnOnLed(WhichLed);
		}			
}
		
	
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}




// The delay parameter is in units of the 16 MHz core clock. (62.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}




//Gelen input numarisina gore ledleri yakiyor
void TurnOnLed(int x)
{
		x%=0x3;     							  //Ledler 0,1,2 seklinde numaralandigi icin 3e gore modu alniyor			
		switch(x) 
		{		
		
   case 0:											 //gelen sayi 0 ise mavi ledi 0.5 saniye yakiyor.
      GPIO_PORTD_DATA_R = 0x02;  //mavi ledi yak
	    for(j=0;j<50;j++){					//0.01x50=0.5 saniye   boyunca oyalaniyor ve butonu kontrol ediyor
				SysTick_Wait(160000); 
			  if ( IsItFirst && (PD0&0x01) == 0x01)//butona basili ve o renk led de ilk basma ise Switch degiskenin degerini ters ceviriyor
				{
					Switch = !Switch;
					IsItFirst=0x00;
				}
			}
      break; 
	
   case 1  :										//gelen sayi 1 ise kirmizi ledi 1 saniye yakiyor.
			GPIO_PORTD_DATA_R = 0x04; //kirmizi ledi yak
				for(j=0;j<100;j++){			//0.01x100=1.0 saniye boyunca oyalaniyor ve butonu kontrol ediyor
	      SysTick_Wait(160000); 
			  if ( IsItFirst && (PD0&0x01) == 0x01)//butona basili ve o renk led de ilk basma ise Switch degiskenin degerini ters ceviriyor
				{
					Switch = !Switch;
					IsItFirst=0x00;
				}
			}
      break; 
	 
	 case 2  :										//gelen sayi 2 ise yesil ledi 1.5 saniye yakiyor.
      GPIO_PORTD_DATA_R = 0x08; //yesil ledi yak
				for(j=0;j<150;j++){			//0.01x1150=1.5 saniye boyunca oyalaniyor ve butonu kontrol ediyor
				SysTick_Wait(160000); 
			  if ( IsItFirst && (PD0&0x01) == 0x01)//butona basili ve o renk led de ilk basma ise Switch degiskenin degerini ters ceviriyor
				{
					Switch = !Switch;
					IsItFirst=0x00;
				}
			}
      break; 	 
		}
}	
		
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x000000008;     // 1) D clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // 2) unlock PortD PD0        
  GPIO_PORTD_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTD_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R = 0x0E;          // 5) PD0 input, PD3,PD2,PD1 output   
  GPIO_PORTD_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTD_DEN_R = 0x1F;          // 7) enable digital pins PD4-PD0        
}



