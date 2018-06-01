// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
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

// 2. Declarations Section
//   Global Variables

int j;
int WhichLed=999999998;   //mavi ledin kodu 0 oldugu icin %=0 verecek bir sayi ile basladi
int Switch;         			//0 veya 1 olmasi ledlerin yanma sirasini belirliyor
int IsItFirst;      			//LED yanarken sadece  butona ilk basmayi degerlendirmek icin

//   Function Prototypes
void PortF_Init(void);
void Delay(void);
void EnableInterrupts(void);
void TurnOnLed(int x);


int main(void)
{    	
  PortF_Init();        // Call initialization of port PF4 PF2    
  EnableInterrupts();  // The grader uses interrupts
  while(1)
		{
			if(Switch==0x00)//Switch 0 ise WhichLed degiskenini arttiriyor degilse azaltiyor
				WhichLed++;
			else
				WhichLed--;
	
			
			IsItFirst=0x01;		 	//Ledin rengi degisecegi icin sifirliyoruz
			TurnOnLed(WhichLed);//Siranin hangi ledde oldugu belirlendkten sonra fonksiyone o ledin numarasi gonderiliyor  
				
		}			
}
		
	//Gelen input numarisina gore ledleri yakiyor
void TurnOnLed(int x)
{
		x%=0x3;     							  //Ledler 0,1,2 seklinde numaralandigi icin 3e gore modu alniyor			
		switch(x) 
		{		
		
   case 0:											//gelen sayi 0 ise mavi ledi 0.5 saniye yakiyor.
      GPIO_PORTF_DATA_R = 0x04; //mavi ledi yak
	    for(j=0;j<50;j++)					//0.01x50=0.5 saniye boyunca oyalaniyor ve butonu kontrol ediyor
				Delay();
      break; 
	
   case 1  :										//gelen sayi 1 ise kirmizi ledi 1 saniye yakiyor.
			GPIO_PORTF_DATA_R = 0x02; //kirmizi ledi yak
			for(j=0;j<100;j++)				//0.01x100=1 saniye boyunca oyalaniyor ve butonu kontrol ediyor
				Delay();
      break; 
	 
	 case 2  :										//gelen sayi 2 ise yesil ledi 1.5 saniye yakiyor.
      GPIO_PORTF_DATA_R = 0x08; //yesil ledi yak
			for(j=0;j<150;j++)				//0.01x150=1.5 saniye boyunca oyalaniyor ve butonu kontrol ediyor
				Delay();
      break; 	 
		}
}	

//Her cagrildiginda butona basili olup olmadigini kontrol ediyor ve 0.01 saniye gecikme olusturuyor
void Delay(void)
	{unsigned long volatile time;
  time = 14550;  // 0.01sec
	if ( IsItFirst && (GPIO_PORTF_DATA_R&0x10) == 0x00)//butona basili ve o renk led de ilk basma ise Switch degiskenin degerini ters ceviriyor
		{
			Switch = !Switch;
			IsItFirst=0x00;
		}
		
  while(time){
		time--;
		
  }
}
		
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}



