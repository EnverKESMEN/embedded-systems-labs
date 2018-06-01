// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#include "Nokia5110.h"
#include <stdlib.h>

//*****************************************************************************
//
// GPIO registers (PORTA)
//
//*****************************************************************************
#define GPIO_PORTA_DATA_BITS_R  ((volatile unsigned long *)0x40004000)
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_IS_R         (*((volatile unsigned long *)0x40004404))
#define GPIO_PORTA_IBE_R        (*((volatile unsigned long *)0x40004408))
#define GPIO_PORTA_IEV_R        (*((volatile unsigned long *)0x4000440C))
#define GPIO_PORTA_IM_R         (*((volatile unsigned long *)0x40004410))
#define GPIO_PORTA_RIS_R        (*((volatile unsigned long *)0x40004414))
#define GPIO_PORTA_MIS_R        (*((volatile unsigned long *)0x40004418))
#define GPIO_PORTA_ICR_R        (*((volatile unsigned long *)0x4000441C))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DR2R_R       (*((volatile unsigned long *)0x40004500))
#define GPIO_PORTA_DR4R_R       (*((volatile unsigned long *)0x40004504))
#define GPIO_PORTA_DR8R_R       (*((volatile unsigned long *)0x40004508))
#define GPIO_PORTA_ODR_R        (*((volatile unsigned long *)0x4000450C))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_PDR_R        (*((volatile unsigned long *)0x40004514))
#define GPIO_PORTA_SLR_R        (*((volatile unsigned long *)0x40004518))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
#define GPIO_PORTA_ADCCTL_R     (*((volatile unsigned long *)0x40004530))
#define GPIO_PORTA_DMACTL_R     (*((volatile unsigned long *)0x40004534))
	
//*****************************************************************************
//
// GPIO registers (PORTE)
//
//*****************************************************************************
#define GPIO_PORTE_DATA_BITS_R  ((volatile unsigned long *)0x40024000)
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_IS_R         (*((volatile unsigned long *)0x40024404))
#define GPIO_PORTE_IBE_R        (*((volatile unsigned long *)0x40024408))
#define GPIO_PORTE_IEV_R        (*((volatile unsigned long *)0x4002440C))
#define GPIO_PORTE_IM_R         (*((volatile unsigned long *)0x40024410))
#define GPIO_PORTE_RIS_R        (*((volatile unsigned long *)0x40024414))
#define GPIO_PORTE_MIS_R        (*((volatile unsigned long *)0x40024418))
#define GPIO_PORTE_ICR_R        (*((volatile unsigned long *)0x4002441C))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DR2R_R       (*((volatile unsigned long *)0x40024500))
#define GPIO_PORTE_DR4R_R       (*((volatile unsigned long *)0x40024504))
#define GPIO_PORTE_DR8R_R       (*((volatile unsigned long *)0x40024508))
#define GPIO_PORTE_ODR_R        (*((volatile unsigned long *)0x4002450C))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_PDR_R        (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_SLR_R        (*((volatile unsigned long *)0x40024518))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_ADCCTL_R     (*((volatile unsigned long *)0x40024530))
#define GPIO_PORTE_DMACTL_R     (*((volatile unsigned long *)0x40024534))



//*****************************************************************************
//
// ADC registers (ADC0)
//
//*****************************************************************************
#define ADC0_ACTSS_R            (*((volatile unsigned long *)0x40038000))
#define ADC0_RIS_R              (*((volatile unsigned long *)0x40038004))
#define ADC0_IM_R               (*((volatile unsigned long *)0x40038008))
#define ADC0_ISC_R              (*((volatile unsigned long *)0x4003800C))
#define ADC0_OSTAT_R            (*((volatile unsigned long *)0x40038010))
#define ADC0_EMUX_R             (*((volatile unsigned long *)0x40038014))
#define ADC0_USTAT_R            (*((volatile unsigned long *)0x40038018))
#define ADC0_TSSEL_R            (*((volatile unsigned long *)0x4003801C))
#define ADC0_SSPRI_R            (*((volatile unsigned long *)0x40038020))
#define ADC0_SPC_R              (*((volatile unsigned long *)0x40038024))
#define ADC0_PSSI_R             (*((volatile unsigned long *)0x40038028))
#define ADC0_SAC_R              (*((volatile unsigned long *)0x40038030))
#define ADC0_DCISC_R            (*((volatile unsigned long *)0x40038034))
#define ADC0_CTL_R              (*((volatile unsigned long *)0x40038038))
#define ADC0_SSMUX0_R           (*((volatile unsigned long *)0x40038040))
#define ADC0_SSCTL0_R           (*((volatile unsigned long *)0x40038044))
#define ADC0_SSFIFO0_R          (*((volatile unsigned long *)0x40038048))
#define ADC0_SSFSTAT0_R         (*((volatile unsigned long *)0x4003804C))
#define ADC0_SSOP0_R            (*((volatile unsigned long *)0x40038050))
#define ADC0_SSDC0_R            (*((volatile unsigned long *)0x40038054))
#define ADC0_SSMUX1_R           (*((volatile unsigned long *)0x40038060))
#define ADC0_SSCTL1_R           (*((volatile unsigned long *)0x40038064))
#define ADC0_SSFIFO1_R          (*((volatile unsigned long *)0x40038068))
#define ADC0_SSFSTAT1_R         (*((volatile unsigned long *)0x4003806C))
#define ADC0_SSOP1_R            (*((volatile unsigned long *)0x40038070))
#define ADC0_SSDC1_R            (*((volatile unsigned long *)0x40038074))
#define ADC0_SSMUX2_R           (*((volatile unsigned long *)0x40038080))
#define ADC0_SSCTL2_R           (*((volatile unsigned long *)0x40038084))
#define ADC0_SSFIFO2_R          (*((volatile unsigned long *)0x40038088))
#define ADC0_SSFSTAT2_R         (*((volatile unsigned long *)0x4003808C))
#define ADC0_SSOP2_R            (*((volatile unsigned long *)0x40038090))
#define ADC0_SSDC2_R            (*((volatile unsigned long *)0x40038094))
#define ADC0_SSMUX3_R           (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R           (*((volatile unsigned long *)0x400380A4))
#define ADC0_SSFIFO3_R          (*((volatile unsigned long *)0x400380A8))
#define ADC0_SSFSTAT3_R         (*((volatile unsigned long *)0x400380AC))
#define ADC0_SSOP3_R            (*((volatile unsigned long *)0x400380B0))
#define ADC0_SSDC3_R            (*((volatile unsigned long *)0x400380B4))
#define ADC0_DCRIC_R            (*((volatile unsigned long *)0x40038D00))
#define ADC0_DCCTL0_R           (*((volatile unsigned long *)0x40038E00))
#define ADC0_DCCTL1_R           (*((volatile unsigned long *)0x40038E04))
#define ADC0_DCCTL2_R           (*((volatile unsigned long *)0x40038E08))
#define ADC0_DCCTL3_R           (*((volatile unsigned long *)0x40038E0C))
#define ADC0_DCCTL4_R           (*((volatile unsigned long *)0x40038E10))
#define ADC0_DCCTL5_R           (*((volatile unsigned long *)0x40038E14))
#define ADC0_DCCTL6_R           (*((volatile unsigned long *)0x40038E18))
#define ADC0_DCCTL7_R           (*((volatile unsigned long *)0x40038E1C))
#define ADC0_DCCMP0_R           (*((volatile unsigned long *)0x40038E40))
#define ADC0_DCCMP1_R           (*((volatile unsigned long *)0x40038E44))
#define ADC0_DCCMP2_R           (*((volatile unsigned long *)0x40038E48))
#define ADC0_DCCMP3_R           (*((volatile unsigned long *)0x40038E4C))
#define ADC0_DCCMP4_R           (*((volatile unsigned long *)0x40038E50))
#define ADC0_DCCMP5_R           (*((volatile unsigned long *)0x40038E54))
#define ADC0_DCCMP6_R           (*((volatile unsigned long *)0x40038E58))
#define ADC0_DCCMP7_R           (*((volatile unsigned long *)0x40038E5C))
#define ADC0_PP_R               (*((volatile unsigned long *)0x40038FC0))
#define ADC0_PC_R               (*((volatile unsigned long *)0x40038FC4))
#define ADC0_CC_R               (*((volatile unsigned long *)0x40038FC8))


#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))
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
long a=0;
long pwm=0;
long led=0;
long result=0;


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
	
	SYSCTL_RCGC2_R |= 0x00000001; // activate port A
	GPIO_PORTA_AMSEL_R = 0xFF;    // no analog
  GPIO_PORTA_PCTL_R &= ~0xFFFFFFFF; // regular function
  GPIO_PORTA_DIR_R |= 0xFF;       // make PA 2,3,4,5,6,7 output.
  GPIO_PORTA_AFSEL_R &= ~0xFF;   // disable alt funct 
  GPIO_PORTA_DEN_R |= 0xFF;      // enable digital I/O 
	
 
}



unsigned long ADC0_InSeq3(void){unsigned long result;
   ADC0_PSSI_R = 0x0008;
   while((ADC0_RIS_R&0x08)==0){};
	 result = ADC0_SSFIFO3_R&0xFFF;
	 ADC0_ISC_R = 0x0008;
	 return result;
}

void ADC0_InitSWTriggerSeq3_Ch9(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;   // 1) activate clock for Port E
	delay = SYSCTL_RCGC2_R;					//    allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;			// 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;			// 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;			// 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;			// 5) enable analog function on PE2
  SYSCTL_RCGC0_R |= 0x00010000;		// 6) activate ADC0
  delay = SYSCTL_RCGC2_R;         
  SYSCTL_RCGC0_R &= ~0x00000300;  // 7) configure for 125K
  ADC0_SSPRI_R = 0x0123; 				 // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008; // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;// 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;// 11) clear SS3 field
  ADC0_SSMUX3_R += 9; //    set channel Ain9 (PE4)
  ADC0_SSCTL3_R = 0x0006; // 12) no TS0 D0, yes IE0 END0
  ADC0_ACTSS_R |= 0x0008; // 13) enable sample sequencer 3
}	

void led1_light()
{
	GPIO_PORTA_DATA_R = 0x4;
}



void led2_light()
{
	GPIO_PORTA_DATA_R = 0xE;
}


void led3_light()
{
	GPIO_PORTA_DATA_R = 0x1E;
}


void led4_light()
{
	GPIO_PORTA_DATA_R = 0x3E;
}
void led5_light()
{
	GPIO_PORTA_DATA_R = 0x7E;
}


void led6_light()
{
	GPIO_PORTA_DATA_R = 0xFE;
}



void SysTick_Handler(void){
  a++;//PWM hesaplamak için sürekli 100e kadar sayıyor.
	if(a>99)
	{
		a=0;
	}
	

	result= ADC0_InSeq3();//Potansiyometreden gelen değer
	
	//Gelen değere göre ledler yakılıyor(her 0x2AA için bir led artan değer result değişkenine atanıyor.)
	if(result>0xFFC)
	{
		led6_light();
		led=7;
	}
	else if(result>0xD52)
	{
			 led5_light();
			 result = result - 0xD52;
		   led=6;
	}
	else if(result>0xAA8){
				led4_light();
		 result = result - 0xAA8;
				led=5;
	}
	else if(result>0x7FE){
		
			led3_light();
		  result = result - 0x7FE;
			led=4;
	}
	
	else if(result>0x554){
			led2_light();
		  result = result - 0x554;
		  led=3;
	}
	else if(result>0x2AA)
	{		
			led1_light();
		  result = result - 0x2AA;
			led=2;
	}	
	else
	{  
		  led=1;
	}
	
	//result değişkenine atanan değer yüze göre oranlanıyor.
	 pwm = (100 * result)/0x2AA;
	
	
	//100 e göre oranlanan değere göre PWM sağlanıyor.
if(pwm>a)
{
	
	   switch(led) {
      case 1 :
         GPIO_PORTA_DATA_R |=0x04;
         break;
      case 2 :
				 GPIO_PORTA_DATA_R |=0x08;
				 break;
      case 3 :
        GPIO_PORTA_DATA_R |=0x10;
         break;
      case 4 :
         GPIO_PORTA_DATA_R |=0x20;
         break;
      case 5 :
          GPIO_PORTA_DATA_R |=0x40;
				 break;
			case 6 :
          GPIO_PORTA_DATA_R |=0x80;
				 break;
			
       }

		}
else
{
	switch(led) {
      case 1 :
         GPIO_PORTA_DATA_R &= ~0x04;
         break;
      case 2 :
				 GPIO_PORTA_DATA_R  &= ~0x08;
				 break;
      case 3 :
          GPIO_PORTA_DATA_R  &= ~0x10;
         break;
      case 4 :
          GPIO_PORTA_DATA_R  &= ~0x20;
         break;
      case 5 :
          GPIO_PORTA_DATA_R  &= ~0x40;
				 break;
			case 6 :
          GPIO_PORTA_DATA_R  &= ~0x80;
				 break;
			
       }
}
	
}
	
	



int main(void){ // running at 16 MHz
  	Port_Init(); //portA kurulumu
ADC0_InitSWTriggerSeq3_Ch9();
	SysTick_Init(1600);
  while(1){  
			WaitForInterrupt();
			
		}
		
		
		
		
}

