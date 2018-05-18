
#ifndef __AD5752_H
#define __AD5752_H


//functions' prototype
void WriteToAD5752Spi(long int *RegisterData);
void ReadFromAD5752Spi(long int *RegisterData);
void ConfigAD5752(void);
void AD5752_Init(void);

//端口宏定义
/*接线 PF7 -> SYNC ;  PF8 -> SCLK ;  PF9 -> SDIN ; PF10 -> LDAC ; PF11 -> RST ; PF12 -> SDOUT ;*/
#define SET_SYNC()		PFout(7)=1	//PF7->SYNC
#define CLR_SYNC()		PFout(7)=0

#define SET_SCLK()	  PFout(8)=1	//PF8->SCLK
#define CLR_SCLK()		PFout(8)=0

#define	SET_SDIN()		PFout(9)=1	//PF9 -> SDIN
#define	CLR_SDIN()		PFout(9)=0

#define	SET_LDAC()		PFout(10)=1	//PF10 -> LDAC
#define	CLR_LDAC()		PFout(10)=0

#define	SET_RST()		PFout(11)=1	//PF11 -> RST
#define	CLR_RST()		PFout(11)=0

#define	SDOUT 		PFin(12)

#define Read  0x800000
#define Write 0x000000

//Channel Select
#define DAC_Channel_A    0x000000
#define DAC_Channel_B      0x020000
#define DAC_Channel_ALL  0x040000

//Register Select
#define DAC_Register                  0x000000
#define Output_Range_Select_Register  0x080000
#define Power_Control_Register        0x100000
#define Control_Register              0x180000

// Output Range Config
/*
Range1: 0~5V
Range2: 0~10V
Range3: 0~10.8V
Range4: -5~+5V
Range5: -10~+10V
Range6: -10.8~+10.8V
*/

#define Range1_Select 0x000000
#define Range2_Select 0x000001
#define Range3_Select 0x000002
#define Range4_Select 0x000003
#define Range5_Select 0x000004
#define Range6_Select 0x000005

// Power Control
#define PowerUp_ALL         0x000005	//

#define PowerUp_Channel_A   0x000001
#define PowerDown_Channel_A 0xFFFFFE

#define PowerUp_Channel_B   0x000002
#define PowerDown_Channel_B 0xFFFFFD

#define PowerUp_Channel_C   0x000004
#define PowerDown_Channel_C 0xFFFFFB

#define PowerUp_Channel_D   0x000008
#define PowerDown_Channel_D 0xFFFFF7

#define PowerUp_REF         0x000010
#define PowerDown_REF       0xFFFFEF

//General Config

#define Nop 0x180000 //for readback

//Control Register

#define Control        0x190000

#define TSD_Enable     0x000008
#define TSD_Disable    0x000000

#define Clamp_Enable   0x000004
#define TSD_Disable    0x000000
					  
#define Clear_Select0  0x000002
#define Clear_Select1  0x000000

#define SDO_Disable    0x000001
#define SDO_Enable     0x000000

//Clear

#define Clear 0x1C0000

//Load

#define Load 0x1D0000

#endif
