#include "StdTypes.h"
#include "Utils.h"
#include "MemMap.h"
#define  F_CPU  8000000
#include <util/delay.h>
#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "ADC.h"
#include "Move.h"
#include "LCD.h"
#include "Stepper.h"
#include "KeyPad.h"
#include "Sensor.h"
#include "EX_Interrupt.h"
#include "NVM.h"
#include "NVM.h"


#define  NVM_STATUS_LOCATION    0x20
#define  NVM_PASSWORD_LOCATION1 0x21
#define  NVM_PASSWORD_LOCATION2 0x22
#define  NVM_PASSWORD_LOCATION3 0x23
#define  NVM_PASSWORD_LOCATION4 0x24
#define  MAX_TRIES 3

u8 arr[4];

int main(void)
{
	DIO_Init();
	ADC_Init(VREF_VCC,ADC_SCALER_64);
	u8 value=NOTPRESSED;
	u8 flag=0,i;
	u8 tries=MAX_TRIES;
	KEYPad_Init();
	LCD_Init();
	LCD_setcursorString(0,0,"Welcome MR Saif ");
	LCD_setcursorString(1,0,"In The Safe  :) ");
	_delay_ms(1600);
	LCD_Clear();
	if (NVM_Read(NVM_STATUS_LOCATION)==NOTPRESSED)
	{
		
		LCD_WriteString("Specify The pass:");
		for (i=0;i<=3;i++)
		{
			do
			{
				value=KEYPad_GetKey();
			}while (value==NOTPRESSED);
			
		LCD_setcursor(1,6+i);
			LCD_WriteChar(value);
			_delay_ms(300);
			LCD_setcursor(1,6+i);
			LCD_WriteChar('.');
			_delay_ms(300);
			NVM_Write(NVM_PASSWORD_LOCATION1+i,value);
		}
		NVM_Write(NVM_STATUS_LOCATION,0x00);
	}
	
	
	
	while(flag==0)
	{
		arr[0]=arr[1]=arr[2]=arr[3]=NOTPRESSED;
		LCD_Clear();
		LCD_WriteString("Enter The Pass");
		for (i=0;i<=3;i++)
		{
			do
			{
				arr[i]=KEYPad_GetKey();
			}while (arr[i]==NOTPRESSED);
			LCD_setcursor(1,6+i);
			LCD_WriteChar(arr[i]);
			_delay_ms(300);
			LCD_setcursor(1,6+i);
			LCD_WriteChar('.');
			_delay_ms(300);
		}
		
		if(NVM_Read(NVM_PASSWORD_LOCATION1)==arr[0] &&  NVM_Read(NVM_PASSWORD_LOCATION2)==arr[1] && NVM_Read(NVM_PASSWORD_LOCATION3)==arr[2] && NVM_Read(NVM_PASSWORD_LOCATION4)==arr[3])
		{
			LCD_Clear();
			LCD_WriteString("Right Password");
			LCD_setcursor(1,0);
			LCD_WriteString("U Are In . ");
			_delay_ms(1500);
			LCD_Clear();
			LCD_WriteString("MOTOR ON");
			flag=1;
		 while(1){
			 STEPPER_UniPolar_CW();
		     STEPPER_UniPolar_CW();
		     STEPPER_UniPolar_CW();
		     STEPPER_UniPolar_CW();
			 STEPPER_UniPolar_CW();
			 STEPPER_UniPolar_CW();
			 STEPPER_UniPolar_CW();
			 STEPPER_UniPolar_CW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
			 STEPPER_UniPolar_CCW();
		 }	
		}
		
		else
		{
			tries=tries-1;
			if (tries>0)
			{
				LCD_Clear();
				LCD_WriteString("wrong password");
				_delay_ms(1500);
				LCD_Clear();
				LCD_WriteString("tries left:");
				LCD_WriteChar(tries+48);
				_delay_ms(1500);
				
			}
			else
			{
				LCD_Clear();
				LCD_WriteString("wrong password");
				LCD_setcursor(1,1);
				LCD_WriteString("safe closed");
				flag=1;
			}
		}
	}
}


