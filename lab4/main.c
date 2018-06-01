// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses


#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_DR8R_R     	(*((volatile unsigned long *)0x40004508))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_IS_R         (*((volatile unsigned long *)0x40004404))
#define GPIO_PORTA_IBE_R        (*((volatile unsigned long *)0x40004408))
#define GPIO_PORTA_IEV_R        (*((volatile unsigned long *)0x4000440C))
#define GPIO_PORTA_ICR_R        (*((volatile unsigned long *)0x4000441C))
#define GPIO_PORTA_IM_R         (*((volatile unsigned long *)0x40004410))
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



volatile unsigned long Counts=0;//Timer her yar1m saniyede bir kesme uygulayacak sekilde kuruldu. bu degisken ile led yanarken kac timer kesmesi oldugu hesaplaniyor
volatile unsigned long Switch=0;//Ledlerin yanma yonunu belirleyen degisken
volatile unsigned long TempPortA_Data; //porta daki veriyi kaydetmek icin kullandigimiz degisken
volatile unsigned long WhichLed=999998;//Hangi ledin yanacagina karar veren degisken

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
//Gelen degiskene göre hangi ledin yanacagina karar veriyor
void ChangeLed(unsigned long x)
{
 
	x%=0x3;
	switch(x) 
		{
		
			case 0:
				GPIO_PORTA_DATA_R = 0x10;//PA4'e bagli ledi yakiyor
				Counts = 2;//Toplamda 3 timer kesmesi suresi boyunca yanik kalmasi icin 2'ye set ediliyor
			break;
			
			case 1:
				GPIO_PORTA_DATA_R = 0x20;//PA5'e bagli ledi yakiyor
				Counts=1;//Toplamda 2 timer kesmesi suresi boyunca yanik kalmasi icin 1'ye set ediliyor
			break;
			
			case 2:
				GPIO_PORTA_DATA_R = 0x40;//PA6'ya bagli ledi yakiyor
				Counts=0;//bir timer kesmesi boyunca yanik kalmasi icin 0'a set ediliyor.
			break;
		}
		TempPortA_Data = 0xFF;//Led degistigi icin hafizda tuttugumuz porta verisini resetliyoruz
}





void PortA_Init(void)
{
	
	SYSCTL_RCGC2_R |= 0x00000001; // activate port A 
	GPIO_PORTA_AMSEL_R = 0x78;    // no analog
  GPIO_PORTA_PCTL_R &= ~0x0FFFF000; // regular function
  GPIO_PORTA_DIR_R &= ~0x08;       // make pa4,5,6 output 
	GPIO_PORTA_DIR_R |= 0x70;				//pa3 input	   		
  GPIO_PORTA_DR8R_R |= ~0x78;    // disable alt funtion PA 3,4,5,6 +++++++++++++
  GPIO_PORTA_AFSEL_R &= ~0x78;  // disable alt funct 
  GPIO_PORTA_DEN_R |= 0x78;     // enable digital I/O 
	GPIO_PORTA_IS_R &= ~0x08;    //PA3 edge sensetive
	GPIO_PORTA_IBE_R&= ~0x08;		 //PA3 is not both edges
	GPIO_PORTA_IEV_R |= 0x08;   //PA3 rising edge event
	GPIO_PORTA_IM_R |= 0x08; //PA3 rising edge event
	GPIO_PORTA_ICR_R = 0x08;			//PA3 clear flag
	GPIO_PORTA_IM_R |= 0x08;    	//arm interrupt on PA3
	
	NVIC_EN0_R = 0x01;  //enable interrupts on portA
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
	NVIC_PRI0_R =  (NVIC_PRI0_R & 0xFFFFFF0F)|0x20;
 
}


void GPIOPortA_Handler(void){ 

	GPIO_PORTA_ICR_R = 0x08;//PA3 clear flag
 
	if(TempPortA_Data != GPIO_PORTA_DATA_R)																				//tempte tutulan data su andaki dataya esit degil ise bu renkte butona ilkkez basildigini gosteriyor
	{
		Switch = !Switch;																														//ledlerin yanma yonunu degistir
	  TempPortA_Data = GPIO_PORTA_DATA_R; 																			// su anki datayi tempe kaydet
	}
	 
}


//SysTick timer interruptu her 0.5 saniyede bir kez aktif oluyor
void SysTick_Handler(void){

	if(Counts == 0)//o anki ledde yanmasi gereken sure tamamlandi ise isliyor
	{
		if(Switch==0x00)	//Switch 0 ise WhichLed degiskenini arttiriyor degilse azaltiyor
				WhichLed++;
		else
				WhichLed--;
		
		ChangeLed(WhichLed);//Ledi degistir.
	}
	else //sure tamamlanmadi ise bir azaltarak 0.5 saniye daha bekliyor
		Counts--;
	
}



int main(void){ // running at 16 MHz
  PortA_Init(); //portA kurulumu
  SysTick_Init(8000000);        // initialize SysTick timer, every 0,5 s

  EnableInterrupts();      // enable after everything initialized
  while(1){                   
		WaitForInterrupt();
  }
}

