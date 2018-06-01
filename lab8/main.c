
#include "tm4c123gh6pm.h"





const unsigned char SineWave[32] = {8,10,11,12,14,15,15,16,16,16,15,15,14,12,11,10,8,6,5,4,2,1,1,0,0,0,1,1,2,4,5,6}; //http://www.daycounter.com/Calculators/Sine-Generator-Calculator2.phtml
unsigned char Index=0;// Index varies from 0 to 15
int flag=1;
// **************DAC_Init*********************
// Initialize 3-bit DAC
// Input: none
// Output: none
void DAC_Init(void){unsigned long volatile delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   // activate port B
	delay = SYSCTL_RCGC2_R;               // allow time to finish activating
	GPIO_PORTB_AMSEL_R &= ~0x17;         // no analog
	GPIO_PORTB_PCTL_R &= ~0x000F0FFF;    // regular GPIO function
	GPIO_PORTB_DIR_R |= 0x17;						// make PB2-0 out
	GPIO_PORTB_AFSEL_R &= ~0x17; 			// disable alt funct on PB2-0
	GPIO_PORTB_DEN_R |= 0x17;				 // enable digital I/O on PB2-0
}

void EnableInterrupts(void);	
void WaitForInterrupt(void); 

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void Sound_Init(unsigned long period){
	
	NVIC_ST_CTRL_R = 0; // disable SysTick during setup
	NVIC_ST_RELOAD_R = period-1;// reload value
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	NVIC_ST_CTRL_R = 0x0007; // enable SysTick with core clock and interrupts
}

// **************DAC_Out*********************
// output to DAC
// Input: 3-bit data, 0 to 7
// Output: none
void DAC_Out(unsigned long data){
	GPIO_PORTB_DATA_R = data;
}
// the sound frequency will be (interrupt frequency)/(size of the table)
void SysTick_Handler(void){
	Index = (Index+1)&0x1F;    // 4,5,6,7,7,7,6,5,4, one value ea3,2,1,1,1,2,3,...  
	DAC_Out(SineWave[Index]);  // outputch interrupt
}



void PortA_Init(void)
{
	
	SYSCTL_RCGC2_R |= 0x00000001; // activate port A 
	GPIO_PORTA_AMSEL_R = 0xFC;    // no analog
  GPIO_PORTA_PCTL_R &= ~0x0FFFFF00; // regular function
	GPIO_PORTA_DIR_R &= ~0xFC;				//pa2-7 input	   		
  GPIO_PORTA_DR8R_R |= ~0xFC;    // disable alt funtion PA 3,4,5,6 +++++++++++++
  GPIO_PORTA_AFSEL_R &= ~0xFC;  // disable alt funct 
  GPIO_PORTA_DEN_R |= 0xFC;     // enable digital I/O 
	GPIO_PORTA_IS_R &= ~0xFC;    //PA2-7 edge sensetive
	GPIO_PORTA_IBE_R |= 0xFC;		 //PA2-7 is  both edges
	GPIO_PORTA_IEV_R |= 0xFC;   //PA2-7 rising edge event
	GPIO_PORTA_IM_R |= 0xFC; //PA2-7 rising edge event
	GPIO_PORTA_ICR_R = 0xFC;			//PA2-7 clear flag
	GPIO_PORTA_IM_R |= 0xFC;    	//arm interrupt on PA2-7
	
	NVIC_EN0_R = 0x01;  //enable interrupts on portA
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
	NVIC_PRI0_R =  (NVIC_PRI0_R & 0xFFFFFF0F)|0x20;
 
}

void GPIOPortA_Handler(void){ 

	GPIO_PORTA_ICR_R = 0xFC;//PA27 clear flag
	if(GPIO_PORTA_DATA_R==0)
		Sound_Init(0);
	
	if(GPIO_PORTA_DATA_R == 0x04)
	{	
			Sound_Init(1923); //DO 262Hz ise 16.000.000/262/32+16=1923
		
	}
	if(GPIO_PORTA_DATA_R == 0x08)
	{	
			Sound_Init(1710);//RE 294Hz ise 16.000.000/294/32+10=1710
		
	}
	if(GPIO_PORTA_DATA_R == 0x10)
	{
			Sound_Init(3046);//Mi 330Hz
	}
	if(GPIO_PORTA_DATA_R == 0x20)
	{
			Sound_Init(2880);//Fa 349Hz
	}

if(GPIO_PORTA_DATA_R == 0x40)
	{
			Sound_Init(2720);//Sol 392Hz
	}
if(GPIO_PORTA_DATA_R == 0x80)
	{
			Sound_Init(1142);//La 440Hz
	}

	
}
int main(void){
	
	PortA_Init();
	DAC_Init();
			
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1
	EnableInterrupts();
	while(1){
		WaitForInterrupt();
	}
}


