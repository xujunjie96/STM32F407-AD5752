/*
										 _ooOoo_
										o8888888o
										88" . "88
										(| -_- |)
										O\  =  /O
								 ____/`---'\____
							 .'  \\|     |//  `.
							/  \\|||  :  |||//  \
						 /  _||||| -:- |||||-  \
						 |   | \\\  -  /// |   |
						 | \_|  ''\---/''  |   |
						 \  .-\__  `-`  ___/-. /
					 ___`. .'  /--.--\  `. . __
				."" '<  `.___\_<|>_/___.'  >'"".
			 | | :  `- \`.;`\ _ /`;.`/ - ` : | |
			 \  \ `-.   \_ __\ /__ _/   .-` /  /
	======`-.____`-.___\_____/___.-`____.-'======
										 `=---='
	^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
					 佛祖保佑       永无BUG
 */


#include "sys.h"
#include "delay.h"
#include "AD5752.h"

/*接线 PF7 -> SYNC ;  PF8 -> SCLK ;  PF9 -> SDIN ; PF10 -> LDAC ; PF11 -> RST ; PF12 -> SDOUT ;*/

/********************************************************************************
		STM32与AD5752连接的IO初始化
********************************************************************************/
void AD5752_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF7,F8,F9,F10,F11初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIOF12

	SET_RST();
	delay_us(10);
	CLR_RST();
	delay_us(10);
	SET_RST();		
	delay_us(10);	//上电复位
	
	CLR_LDAC();		//LDAC置低
	
}


/********************************************************************************
通过SPI端口写入AD5752的功能
********************************************************************************/

void WriteToAD5752Spi(long int *RegisterData)
{

	
	long int ValueToWrite = *RegisterData;
	int i;
	
	// SPI start

	SET_SYNC();
	delay_us(5);
	CLR_SYNC();	 //bring CS low
	delay_us(1);
	
	//写控制字	
	for(i=0; i<24; i++)
	{	
		SET_SCLK();
	  delay_us(5);
		if(0x800000 == (ValueToWrite & 0x800000))
		{
			SET_SDIN();	  //Send one to SDI pin
		}
		else
		{
			CLR_SDIN();	  //Send zero to SDI pin
		}

		delay_us(5);
		CLR_SCLK();
		delay_us(5);
		
		ValueToWrite <<= 1;	//Rotate data
		delay_us(5);

	}
		
   // SPI ends
	
	SET_SYNC();
    delay_us(10);

}


/********************************************************************************
通过SPI端口从AD5752读取的功能
********************************************************************************/


void ReadFromAD5752Spi(long int *RegisterData)
{

	
	unsigned	int	    i = 0;
	unsigned	int  	iTemp = 0;
	unsigned	long  	RotateData = 0;
	unsigned	long  	Noop = Nop;

	// SPI start

	SET_SYNC();
	delay_us(20);
	CLR_SYNC();	 //bring SYNC low
	delay_us(2);

	for(i=0; i<24; i++)
	{
		SET_SCLK();
		delay_us(5);
	if(0x800000 == (Noop & 0x800000))
	{
		SET_SDIN();	  //Send one to SDI pin
	}
	else
	{
		CLR_SDIN();	  //Send zero to SDI pin
	}

		CLR_SCLK();
		
		delay_us(5);
			 
		iTemp = SDOUT;			//读SDOUT
		 
		 RotateData <<= 1;
		
		if(iTemp==1)	//SDout为1
		{
	
			RotateData |= 1; 	//RotateData加上1 ，SDout为0不变
			
		}
		
		Noop <<= 1;
		
		delay_us(5);		
	}
	 					
   // SPI ends

	SET_SYNC();
 
    delay_us(20); 
	*RegisterData=RotateData;

}



//---------------------
//配置AD5752
//---------------------

void ConfigAD5752(void)
{

int i;
long int *p;
long int ins[2] = {0, 0};

ins[0] = Power_Control_Register | PowerUp_ALL;
ins[1] = Output_Range_Select_Register | Range5_Select | DAC_Channel_ALL;

p = ins;

for(i=0; i<2; i++)
{ 
  WriteToAD5752Spi(p);
  delay_us(200);
  p++;}
}


