/*********************************************************************************************************
*
* File                : LCD.c
* Hardware Environment:
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : sasa_xiao
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD.h" 
#include "HzLib.h"
#include "AsciiLib.h"

/* Private define ------------------------------------------------------------*/
/* 使用总线方式时定义地址 */
/* 挂在不同的BANK,使用不同地址线时请自行换算地址 */
//#define LCD_REG              (*((volatile unsigned short *) 0x6C000000)) /* RS = 0 */
//#define LCD_RAM              (*((volatile unsigned short *) 0x6C000002)) /* RS = 1 */
#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */


/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Code;

/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8347D    10 /* 0x0047 */
#define  HX8347A    11 /* 0x0047 */
#define  LGDP4535   12 /* 0x4535 */
#define  SSD2119    13 /* 3.5 LCD 0x9919 */

/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

	/* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
	 PD.10(D15), PD.11(A16), PD.14(D0), PD.15(D1) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* Set PE.02(A23), PE.05(A21), PE.06(A22) as alternate function push pull */
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	    */

	/* Set PF.00(A0 (RS)) as alternate function push pull */
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure); */

	/* Set PG.12(NE4), PG.13(A24) as alternate function push pull - CE3(LCD /CS) */
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  	GPIO_ResetBits(GPIOD,GPIO_Pin_13);	     	//使能139

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  	GPIO_SetBits(GPIOD, GPIO_Pin_12);		//取消 LCD RESET。
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	/* FSMC读速度设置 */
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 5;//5;  /* 地址建立时间  */
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 10;//5;	   /* 数据建立时间  */
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	/* FSMC写速度设置 */
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 15;//1;   /* 地址建立时间  */
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 15;//1;	   /* 数据建立时间  */
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* Enable FSMC Bank4_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
	/* Configure the LCD Control pins --------------------------------------------*/
	LCD_CtrlLinesConfig();

	/* Configure the FSMC Parallel interface -------------------------------------*/
	LCD_FSMCConfig();
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : controller command
* Input          : - cmd: writes command.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
//static __inline void LCD_WriteCommand(uint16_t cmd)
//{
//	/* Write cmd */
//	LCD_REG = cmd;
//}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - data: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
//static __inline void LCD_WriteData(uint16_t data)
//{
	/* Write 16-bit data */
//	LCD_RAM = data;
//}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
* Attention		 : None
*******************************************************************************/
//static __inline uint16_t LCD_ReadData(void)
//{
	/* Read 16-bit data */
//	return LCD_RAM;
//}

/*******************************************************************************
* Function Name  : SSD1963_SSD1963_GPIO_WR
* Description    : Set a GPIO pin to state high(1) or low(0)
* Input          : - pin: LCD_RESET or LCD_SPENA or LCD_SPCLK  or LCD_SPDAT
*                  - state: 0 for low and 1 for high
* Output         : None
* Return         : None
* Attention		 : Set the GPIO pin an output prior using this function
*******************************************************************************/
/*
static void SSD1963_GPIO_WR(uint8_t pin, uint8_t state)
{
	static uint8_t _gpioStatus = 0;

	if( state )
	{
		_gpioStatus = _gpioStatus | pin;
	}
	else
	{
		_gpioStatus = _gpioStatus & ( ~pin );
	}

	LCD_WriteCommand(0xBA);
	LCD_WriteData(_gpioStatus);
}
*/
/*******************************************************************************
* Function Name  : SSD1963_SPI_Write
* Description    : SPI Write
* Input          : - byte: data
* Output         : None
* Return         : None
* Attention		 : GPIO pins for the SPI port set all output prior to
*                  using this function
*******************************************************************************/
/*
static void SSD1963_SPI_Write(uint8_t byte)
{
	uint8_t bit_ctr;

	for( bit_ctr = 0; bit_ctr < 8; bit_ctr++ )
	{
		if( byte & 0x80 )
		{
			SSD1963_GPIO_WR(LCD_SPDAT,1);
		}
		else
		{
			SSD1963_GPIO_WR(LCD_SPDAT,0);
		}
		SSD1963_GPIO_WR(LCD_SPCLK,0);
		SSD1963_GPIO_WR(LCD_SPCLK,1);
		byte = ( byte << 1 );
	}
}
*/
/*******************************************************************************
* Function Name  : SSD1963_SPI_WriteReg
* Description    : write reg
* Input          : - reg:
*                  - cmd:
* Output         : None
* Return         : None
* Attention		 : GPIO pins for the SPI port set all output prior to
*                  using this function
*******************************************************************************/
/*
static void SSD1963_SPI_WriteReg(uint8_t reg, uint16_t cmd)
{
	SSD1963_GPIO_WR(LCD_SPENA,0);
	cmd = ( (reg << 10) | (1 << 9) | cmd );
	SSD1963_SPI_Write( ( (uint8_t) (cmd)>>8) );
	SSD1963_SPI_Write( ( (uint8_t) cmd) );
	SSD1963_GPIO_WR(LCD_SPENA,1);
}
*/
/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - cnt: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void delay_ms(uint16_t ms)
{
	uint16_t i,j;
	for( i = 0; i < ms; i++ )
	{
		for( j = 0; j < 1141; j++ );
	}
}

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : SSD1963 Resets
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
	uint16_t DeviceCode;
	uint16_t i;

void LCD_Initializtion(void)
{

	LCD_Configuration();
	/* Set MN(multipliers) of PLL, VCO = crystal freq * (N+1) */
	/* PLL freq = VCO/M with 250MHz < VCO < 800MHz */
	/* The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq */

	//while(1)
	{
	DeviceCode = 0x0123;
	DeviceCode = LCD_ReadReg(0x0000);		/* 读取屏ID	*/
	//DeviceCode = 0x0123;
	}

	/* 不同屏驱动IC 初始化不同 */
	if( DeviceCode == 0x9325 || DeviceCode == 0x9328 )
	{
		LCD_Code = ILI9325;
		LCD_WriteReg(0x00e7,0x0010);
		LCD_WriteReg(0x0000,0x0001);  	/* start internal osc */
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700); 	/* power on sequence */
		LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3) ); 	/* importance */
		LCD_WriteReg(0x0004,0x0000);
		LCD_WriteReg(0x0008,0x0207);
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000a,0x0000); 	/* display setting */
		LCD_WriteReg(0x000c,0x0001);	/* display setting */
		LCD_WriteReg(0x000d,0x0000);
		LCD_WriteReg(0x000f,0x0000);
		/* Power On sequence */
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0007);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0010,0x1590);
		LCD_WriteReg(0x0011,0x0227);
		delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0012,0x009c);
		delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0013,0x1900);
		LCD_WriteReg(0x0029,0x0023);
		LCD_WriteReg(0x002b,0x000e);
		delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0020,0x0000);
		LCD_WriteReg(0x0021,0x0000);
		delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0707);
		LCD_WriteReg(0x0032,0x0006);
		LCD_WriteReg(0x0035,0x0704);
		LCD_WriteReg(0x0036,0x1f04);
		LCD_WriteReg(0x0037,0x0004);
		LCD_WriteReg(0x0038,0x0000);
		LCD_WriteReg(0x0039,0x0706);
		LCD_WriteReg(0x003c,0x0701);
		LCD_WriteReg(0x003d,0x000f);
		delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00ef);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013f);
		LCD_WriteReg(0x0060,0xa700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x006a,0x0000);
		LCD_WriteReg(0x0080,0x0000);
		LCD_WriteReg(0x0081,0x0000);
		LCD_WriteReg(0x0082,0x0000);
		LCD_WriteReg(0x0083,0x0000);
		LCD_WriteReg(0x0084,0x0000);
		LCD_WriteReg(0x0085,0x0000);

		LCD_WriteReg(0x0090,0x0010);
		LCD_WriteReg(0x0092,0x0000);
		LCD_WriteReg(0x0093,0x0003);
		LCD_WriteReg(0x0095,0x0110);
		LCD_WriteReg(0x0097,0x0000);
		LCD_WriteReg(0x0098,0x0000);
		/* display on sequence */
		LCD_WriteReg(0x0007,0x0133);

		LCD_WriteReg(0x0020,0x0000);  /* 行首址0 */
		LCD_WriteReg(0x0021,0x0000);  /* 列首址0 */
	}
	else if( DeviceCode == 0x9320 || DeviceCode == 0x9300 )
	{
	    LCD_Code = ILI9320;
	    LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	/* Driver Output Contral */
		LCD_WriteReg(0x02,0x0700);	/* LCD Driver Waveform Contral */
		LCD_WriteReg(0x03,0x1018);	/* Entry Mode Set */

		LCD_WriteReg(0x04,0x0000);	/* Scalling Contral */
	    LCD_WriteReg(0x08,0x0202);	/* Display Contral */
		LCD_WriteReg(0x09,0x0000);	/* Display Contral 3.(0x0000) */
		LCD_WriteReg(0x0a,0x0000);	/* Frame Cycle Contal.(0x0000) */
	    LCD_WriteReg(0x0c,(1<<0));	/* Extern Display Interface Contral */
		LCD_WriteReg(0x0d,0x0000);	/* Frame Maker Position */
		LCD_WriteReg(0x0f,0x0000);	/* Extern Display Interface Contral 2. */

	    delay_ms(100);  /* delay 100 ms */
		LCD_WriteReg(0x07,0x0101);	/* Display Contral */
	    delay_ms(100);  /* delay 100 ms */
	
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	/* Power Control 1.(0x16b0)	*/
		LCD_WriteReg(0x11,0x0007);								/* Power Control 2 */
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				/* Power Control 3.(0x0138)	*/
		LCD_WriteReg(0x13,0x0b00);								/* Power Control 4 */
		LCD_WriteReg(0x29,0x0000);								/* Power Control 7 */

		LCD_WriteReg(0x2b,(1<<14)|(1<<4));

		LCD_WriteReg(0x50,0);       /* Set X Start */
		LCD_WriteReg(0x51,239);	    /* Set X End */
		LCD_WriteReg(0x52,0);	    /* Set Y Start */
		LCD_WriteReg(0x53,319);	    /* Set Y End */

		LCD_WriteReg(0x60,0x2700);	/* Driver Output Control */
		LCD_WriteReg(0x61,0x0001);	/* Driver Output Control */
		LCD_WriteReg(0x6a,0x0000);	/* Vertical Srcoll Control */

		LCD_WriteReg(0x80,0x0000);	/* Display Position? Partial Display 1 */
		LCD_WriteReg(0x81,0x0000);	/* RAM Address Start? Partial Display 1 */
		LCD_WriteReg(0x82,0x0000);	/* RAM Address End-Partial Display 1 */
		LCD_WriteReg(0x83,0x0000);	/* Displsy Position? Partial Display 2 */
		LCD_WriteReg(0x84,0x0000);	/* RAM Address Start? Partial Display 2 */
		LCD_WriteReg(0x85,0x0000);	/* RAM Address End? Partial Display 2 */
		
	    LCD_WriteReg(0x90,(0<<7)|(16<<0));	/* Frame Cycle Contral.(0x0013)	*/
		LCD_WriteReg(0x92,0x0000);	/* Panel Interface Contral 2.(0x0000) */
		LCD_WriteReg(0x93,0x0001);	/* Panel Interface Contral 3. */
	    LCD_WriteReg(0x95,0x0110);	/* Frame Cycle Contral.(0x0110)	*/
		LCD_WriteReg(0x97,(0<<8));
		LCD_WriteReg(0x98,0x0000);	/* Frame Cycle Contral */
	
	    LCD_WriteReg(0x07,0x0173);
	}
	else if( DeviceCode == 0x9331 )
	{
	    LCD_Code = ILI9331;
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100);   /* set SS and SM bit */
		LCD_WriteReg(0x0002, 0x0200);   /* set 1 line inversion */
		LCD_WriteReg(0x0003, 0x1030);   /* set GRAM write direction and BGR=1 */
		LCD_WriteReg(0x0008, 0x0202);   /* set the back porch and front porch */
	    LCD_WriteReg(0x0009, 0x0000);   /* set non-display area refresh cycle ISC[3:0] */
		LCD_WriteReg(0x000A, 0x0000);   /* FMARK function */
		LCD_WriteReg(0x000C, 0x0000);   /* RGB interface setting */
		LCD_WriteReg(0x000D, 0x0000);   /* Frame marker Position */
		LCD_WriteReg(0x000F, 0x0000);   /* RGB interface polarity */
		/* Power On sequence */
		LCD_WriteReg(0x0010, 0x0000);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
		LCD_WriteReg(0x0011, 0x0007);   /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x0012, 0x0000);   /* VREG1OUT voltage	*/
		LCD_WriteReg(0x0013, 0x0000);   /* VDV[4:0] for VCOM amplitude */
	    delay_ms(200);  /* delay 200 ms */
		LCD_WriteReg(0x0010, 0x1690);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
		LCD_WriteReg(0x0011, 0x0227);   /* DC1[2:0], DC0[2:0], VC[2:0] */
	    delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0012, 0x000C);   /* Internal reference voltage= Vci	*/
	    delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0013, 0x0800);   /* Set VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x0029, 0x0011);   /* Set VCM[5:0] for VCOMH */
		LCD_WriteReg(0x002B, 0x000B);   /* Set Frame Rate */
	    delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0020, 0x0000);   /* GRAM horizontal Address */
		LCD_WriteReg(0x0021, 0x0000);   /* GRAM Vertical Address */
		/* Adjust the Gamma Curve */
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		/* Set GRAM area */
		LCD_WriteReg(0x0050, 0x0000);   /* Horizontal GRAM Start Address */
		LCD_WriteReg(0x0051, 0x00EF);   /* Horizontal GRAM End Address */
		LCD_WriteReg(0x0052, 0x0000);   /* Vertical GRAM Start Address */
		LCD_WriteReg(0x0053, 0x013F);   /* Vertical GRAM Start Address */
		LCD_WriteReg(0x0060, 0x2700);   /* Gate Scan Line */
		LCD_WriteReg(0x0061, 0x0001);   /*  NDL,VLE, REV */
		LCD_WriteReg(0x006A, 0x0000);   /* set scrolling line */
		/* Partial Display Control */
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		/* Panel Control */
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007,0x0021);
	    delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0007,0x0061);
	    delay_ms(50);  /* delay 50 ms */
		LCD_WriteReg(0x0007,0x0133);    /* 262K color and display ON */
	}
	else if( DeviceCode == 0x9919 )
	{
	    LCD_Code = SSD2119;
	    /* POWER ON &RESET DISPLAY OFF */
		LCD_WriteReg(0x28,0x0006);
		LCD_WriteReg(0x00,0x0001);
		LCD_WriteReg(0x10,0x0000);
		LCD_WriteReg(0x01,0x72ef);
		LCD_WriteReg(0x02,0x0600);
		LCD_WriteReg(0x03,0x6a38);
		LCD_WriteReg(0x11,0x6874);
		LCD_WriteReg(0x0f,0x0000);    /* RAM WRITE DATA MASK */
		LCD_WriteReg(0x0b,0x5308);    /* RAM WRITE DATA MASK */
		LCD_WriteReg(0x0c,0x0003);
		LCD_WriteReg(0x0d,0x000a);
		LCD_WriteReg(0x0e,0x2e00);
		LCD_WriteReg(0x1e,0x00be);
		LCD_WriteReg(0x25,0x8000);
		LCD_WriteReg(0x26,0x7800);
		LCD_WriteReg(0x27,0x0078);
		LCD_WriteReg(0x4e,0x0000);
		LCD_WriteReg(0x4f,0x0000);
		LCD_WriteReg(0x12,0x08d9);
		/* Adjust the Gamma Curve */
		LCD_WriteReg(0x30,0x0000);
		LCD_WriteReg(0x31,0x0104);
		LCD_WriteReg(0x32,0x0100);
	    LCD_WriteReg(0x33,0x0305);
	    LCD_WriteReg(0x34,0x0505);
		LCD_WriteReg(0x35,0x0305);
	    LCD_WriteReg(0x36,0x0707);
	    LCD_WriteReg(0x37,0x0300);
		LCD_WriteReg(0x3a,0x1200);
		LCD_WriteReg(0x3b,0x0800);
	    LCD_WriteReg(0x07,0x0033);
	}
	else if( DeviceCode == 0x1505 || DeviceCode == 0x0505 )
	{
	    LCD_Code = R61505U;
	   /* second release on 3/5  ,luminance is acceptable,water wave appear during camera preview */
	    LCD_WriteReg(0x0007,0x0000);
	    delay_ms(50);  /* delay 50 ms */
	    LCD_WriteReg(0x0012,0x011C);    /* why need to set several times?	*/
	    LCD_WriteReg(0x00A4,0x0001);    /* NVM */
	    LCD_WriteReg(0x0008,0x000F);
	    LCD_WriteReg(0x000A,0x0008);
	    LCD_WriteReg(0x000D,0x0008);
	    /* GAMMA CONTROL */
	    LCD_WriteReg(0x0030,0x0707);
	    LCD_WriteReg(0x0031,0x0007);
	    LCD_WriteReg(0x0032,0x0603);
	    LCD_WriteReg(0x0033,0x0700);
	    LCD_WriteReg(0x0034,0x0202);
	    LCD_WriteReg(0x0035,0x0002);
	    LCD_WriteReg(0x0036,0x1F0F);
	    LCD_WriteReg(0x0037,0x0707);
	    LCD_WriteReg(0x0038,0x0000);
	    LCD_WriteReg(0x0039,0x0000);
	    LCD_WriteReg(0x003A,0x0707);
	    LCD_WriteReg(0x003B,0x0000);
	    LCD_WriteReg(0x003C,0x0007);
	    LCD_WriteReg(0x003D,0x0000);
	    delay_ms(50);  /* delay 50 ms */
	    LCD_WriteReg(0x0007,0x0001);
	    LCD_WriteReg(0x0017,0x0001);    /* Power supply startup enable */
	    delay_ms(50);  /* delay 50 ms */
	    /* power control */
	    LCD_WriteReg(0x0010,0x17A0);
	    LCD_WriteReg(0x0011,0x0217);    /* reference voltage VC[2:0]   Vciout = 1.00*Vcivl */
	    LCD_WriteReg(0x0012,0x011E);    /* Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?	*/
	    LCD_WriteReg(0x0013,0x0F00);    /* VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl */
	    LCD_WriteReg(0x002A,0x0000);
	    LCD_WriteReg(0x0029,0x000A);    /* Vcomh = VCM1[4:0]*Vreg1out    gate source voltage?? */
	    LCD_WriteReg(0x0012,0x013E);    /* power supply on */
	    /* Coordinates Control */
	    LCD_WriteReg(0x0050,0x0000);
	    LCD_WriteReg(0x0051,0x00EF);
	    LCD_WriteReg(0x0052,0x0000);
	    LCD_WriteReg(0x0053,0x013F);
	    /* Pannel Image Control */
	    LCD_WriteReg(0x0060,0x2700);
	    LCD_WriteReg(0x0061,0x0001);
	    LCD_WriteReg(0x006A,0x0000);
	    LCD_WriteReg(0x0080,0x0000);
	    /* Partial Image Control */
	    LCD_WriteReg(0x0081,0x0000);
	    LCD_WriteReg(0x0082,0x0000);
	    LCD_WriteReg(0x0083,0x0000);
	    LCD_WriteReg(0x0084,0x0000);
	    LCD_WriteReg(0x0085,0x0000);
	    /* Panel Interface Control */
	    LCD_WriteReg(0x0090,0x0013);      /* frenqucy */
	    LCD_WriteReg(0x0092,0x0300);
	    LCD_WriteReg(0x0093,0x0005);
	    LCD_WriteReg(0x0095,0x0000);
	    LCD_WriteReg(0x0097,0x0000);
	    LCD_WriteReg(0x0098,0x0000);

	    LCD_WriteReg(0x0001,0x0100);
	    LCD_WriteReg(0x0002,0x0700);
	    LCD_WriteReg(0x0003,0x1030);
	    LCD_WriteReg(0x0004,0x0000);
	    LCD_WriteReg(0x000C,0x0000);
	    LCD_WriteReg(0x000F,0x0000);
	    LCD_WriteReg(0x0020,0x0000);
	    LCD_WriteReg(0x0021,0x0000);
	    LCD_WriteReg(0x0007,0x0021);
	    delay_ms(200);  /* delay 200 ms */
	    LCD_WriteReg(0x0007,0x0061);
	    delay_ms(200);  /* delay 200 ms */
	    LCD_WriteReg(0x0007,0x0173);
	}
	else if( DeviceCode == 0x8989 )
	{
	    LCD_Code = SSD1289;
	    LCD_WriteReg(0x0000,0x0001);    delay_ms(50);   /* 打开晶振 */
	    LCD_WriteReg(0x0003,0xA8A4);    delay_ms(50);
	    LCD_WriteReg(0x000C,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x000D,0x080C);    delay_ms(50);
	    LCD_WriteReg(0x000E,0x2B00);    delay_ms(50);
	    LCD_WriteReg(0x001E,0x00B0);    delay_ms(50);
	    LCD_WriteReg(0x0001,0x2B3F);    delay_ms(50);   /* 驱动输出控制320*240 0x2B3F */
	    LCD_WriteReg(0x0002,0x0600);    delay_ms(50);
	    LCD_WriteReg(0x0010,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0011,0x6070);    delay_ms(50);   /* 定义数据格式 16位色 横屏 0x6070 */
	    LCD_WriteReg(0x0005,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0006,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0016,0xEF1C);    delay_ms(50);
	    LCD_WriteReg(0x0017,0x0003);    delay_ms(50);
	    LCD_WriteReg(0x0007,0x0133);    delay_ms(50);
	    LCD_WriteReg(0x000B,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x000F,0x0000);    delay_ms(50);   /* 扫描开始地址 */
	    LCD_WriteReg(0x0041,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0042,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0048,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0049,0x013F);    delay_ms(50);
	    LCD_WriteReg(0x004A,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x004B,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0044,0xEF00);    delay_ms(50);
	    LCD_WriteReg(0x0045,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0046,0x013F);    delay_ms(50);
	    LCD_WriteReg(0x0030,0x0707);    delay_ms(50);
	    LCD_WriteReg(0x0031,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0032,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0033,0x0502);    delay_ms(50);
	    LCD_WriteReg(0x0034,0x0507);    delay_ms(50);
	    LCD_WriteReg(0x0035,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0036,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0037,0x0502);    delay_ms(50);
	    LCD_WriteReg(0x003A,0x0302);    delay_ms(50);
	    LCD_WriteReg(0x003B,0x0302);    delay_ms(50);
	    LCD_WriteReg(0x0023,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0024,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0025,0x8000);    delay_ms(50);
	    LCD_WriteReg(0x004f,0);        /* 行首址0 */
	    LCD_WriteReg(0x004e,0);        /* 列首址0 */
	}
	else if( DeviceCode == 0x8999 )
	{
		LCD_Code = SSD1298;
		LCD_WriteReg(0x0028,0x0006);
		LCD_WriteReg(0x0000,0x0001);
		LCD_WriteReg(0x0003,0xaea4);    /* power control 1---line frequency and VHG,VGL voltage */
		LCD_WriteReg(0x000c,0x0004);    /* power control 2---VCIX2 output voltage */
		LCD_WriteReg(0x000d,0x000c);    /* power control 3---Vlcd63 voltage */
		LCD_WriteReg(0x000e,0x2800);    /* power control 4---VCOMA voltage VCOML=VCOMH*0.9475-VCOMA */
		LCD_WriteReg(0x001e,0x00b5);    /* POWER CONTROL 5---VCOMH voltage */
		LCD_WriteReg(0x0001,0x3b3f);
		LCD_WriteReg(0x0002,0x0600);
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x6830);
		LCD_WriteReg(0x0005,0x0000);
		LCD_WriteReg(0x0006,0x0000);
		LCD_WriteReg(0x0016,0xef1c);
		LCD_WriteReg(0x0007,0x0033);    /* Display control 1 */
		/* when GON=1 and DTE=0,all gate outputs become VGL */
		/* when GON=1 and DTE=0,all gate outputs become VGH */
		/* non-selected gate wires become VGL */
		LCD_WriteReg(0x000b,0x0000);
		LCD_WriteReg(0x000f,0x0000);
		LCD_WriteReg(0x0041,0x0000);
		LCD_WriteReg(0x0042,0x0000);
		LCD_WriteReg(0x0048,0x0000);
		LCD_WriteReg(0x0049,0x013f);
		LCD_WriteReg(0x004a,0x0000);
		LCD_WriteReg(0x004b,0x0000);
		LCD_WriteReg(0x0044,0xef00);	/* Horizontal RAM start and end address */
		LCD_WriteReg(0x0045,0x0000);	/* Vretical RAM start address */
		LCD_WriteReg(0x0046,0x013f);	/* Vretical RAM end address */
		LCD_WriteReg(0x004e,0x0000);	/* set GDDRAM x address counter */
		LCD_WriteReg(0x004f,0x0000);    /* set GDDRAM y address counter */
		/* y control */
		LCD_WriteReg(0x0030,0x0707);
		LCD_WriteReg(0x0031,0x0202);
		LCD_WriteReg(0x0032,0x0204);
		LCD_WriteReg(0x0033,0x0502);
		LCD_WriteReg(0x0034,0x0507);
		LCD_WriteReg(0x0035,0x0204);
		LCD_WriteReg(0x0036,0x0204);
		LCD_WriteReg(0x0037,0x0502);
		LCD_WriteReg(0x003a,0x0302);
		LCD_WriteReg(0x003b,0x0302);
		LCD_WriteReg(0x0023,0x0000);
		LCD_WriteReg(0x0024,0x0000);
		LCD_WriteReg(0x0025,0x8000);
		LCD_WriteReg(0x0026,0x7000);
		LCD_WriteReg(0x0020,0xb0eb);
		LCD_WriteReg(0x0027,0x007c);
	}
	else if( DeviceCode == 0x5408 )
	{
		LCD_Code = SPFD5408B;

		LCD_WriteReg(0x0001,0x0100);	  /* Driver Output Contral Register */
		LCD_WriteReg(0x0002,0x0700);      /* LCD Driving Waveform Contral */
		LCD_WriteReg(0x0003,0x1030);	  /* Entry Mode设置 */

		LCD_WriteReg(0x0004,0x0000);	  /* Scalling Control register */
		LCD_WriteReg(0x0008,0x0207);	  /* Display Control 2 */
		LCD_WriteReg(0x0009,0x0000);	  /* Display Control 3 */
		LCD_WriteReg(0x000A,0x0000);	  /* Frame Cycle Control */
		LCD_WriteReg(0x000C,0x0000);	  /* External Display Interface Control 1 */
		LCD_WriteReg(0x000D,0x0000);      /* Frame Maker Position */
		LCD_WriteReg(0x000F,0x0000);	  /* External Display Interface Control 2 */
		delay_ms(50);
		LCD_WriteReg(0x0007,0x0101);	  /* Display Control */
		delay_ms(50);
		LCD_WriteReg(0x0010,0x16B0);      /* Power Control 1 */
		LCD_WriteReg(0x0011,0x0001);      /* Power Control 2 */
		LCD_WriteReg(0x0017,0x0001);      /* Power Control 3 */
		LCD_WriteReg(0x0012,0x0138);      /* Power Control 4 */
		LCD_WriteReg(0x0013,0x0800);      /* Power Control 5 */
		LCD_WriteReg(0x0029,0x0009);	  /* NVM read data 2 */
		LCD_WriteReg(0x002a,0x0009);	  /* NVM read data 3 */
		LCD_WriteReg(0x00a4,0x0000);
		LCD_WriteReg(0x0050,0x0000);	  /* 设置操作窗口的X轴开始列 */
		LCD_WriteReg(0x0051,0x00EF);	  /* 设置操作窗口的X轴结束列 */
		LCD_WriteReg(0x0052,0x0000);	  /* 设置操作窗口的Y轴开始行 */
		LCD_WriteReg(0x0053,0x013F);	  /* 设置操作窗口的Y轴结束行 */

		LCD_WriteReg(0x0060,0x2700);	  /* Driver Output Control */
										  /* 设置屏幕的点数以及扫描的起始行 */
		LCD_WriteReg(0x0061,0x0003);	  /* Driver Output Control */
		LCD_WriteReg(0x006A,0x0000);	  /* Vertical Scroll Control */

		LCD_WriteReg(0x0080,0x0000);	  /* Display Position – Partial Display 1 */
		LCD_WriteReg(0x0081,0x0000);	  /* RAM Address Start – Partial Display 1 */
		LCD_WriteReg(0x0082,0x0000);	  /* RAM address End - Partial Display 1 */
		LCD_WriteReg(0x0083,0x0000);	  /* Display Position – Partial Display 2 */
		LCD_WriteReg(0x0084,0x0000);	  /* RAM Address Start – Partial Display 2 */
		LCD_WriteReg(0x0085,0x0000);	  /* RAM address End – Partail Display2 */
		LCD_WriteReg(0x0090,0x0013);	  /* Frame Cycle Control */
		LCD_WriteReg(0x0092,0x0000); 	  /* Panel Interface Control 2 */
		LCD_WriteReg(0x0093,0x0003);	  /* Panel Interface control 3 */
		LCD_WriteReg(0x0095,0x0110);	  /* Frame Cycle Control */
		LCD_WriteReg(0x0007,0x0173);
	}
	else if( DeviceCode == 0x4531 )
	{
		LCD_Code = LGDP4531;
		/* Setup display */
		LCD_WriteReg(0x00,0x0001);
		LCD_WriteReg(0x10,0x0628);
		LCD_WriteReg(0x12,0x0006);
		LCD_WriteReg(0x13,0x0A32);
		LCD_WriteReg(0x11,0x0040);
		LCD_WriteReg(0x15,0x0050);
		LCD_WriteReg(0x12,0x0016);
		delay_ms(50);
		LCD_WriteReg(0x10,0x5660);
		delay_ms(50);
		LCD_WriteReg(0x13,0x2A4E);
		LCD_WriteReg(0x01,0x0100);
		LCD_WriteReg(0x02,0x0300);
		LCD_WriteReg(0x03,0x1030);
		LCD_WriteReg(0x08,0x0202);
		LCD_WriteReg(0x0A,0x0000);
		LCD_WriteReg(0x30,0x0000);
		LCD_WriteReg(0x31,0x0402);
		LCD_WriteReg(0x32,0x0106);
		LCD_WriteReg(0x33,0x0700);
		LCD_WriteReg(0x34,0x0104);
		LCD_WriteReg(0x35,0x0301);
		LCD_WriteReg(0x36,0x0707);
		LCD_WriteReg(0x37,0x0305);
		LCD_WriteReg(0x38,0x0208);
		LCD_WriteReg(0x39,0x0F0B);
		delay_ms(50);
		LCD_WriteReg(0x41,0x0002);
		LCD_WriteReg(0x60,0x2700);
		LCD_WriteReg(0x61,0x0001);
		LCD_WriteReg(0x90,0x0119);
		LCD_WriteReg(0x92,0x010A);
		LCD_WriteReg(0x93,0x0004);
		LCD_WriteReg(0xA0,0x0100);
		delay_ms(50);
		LCD_WriteReg(0x07,0x0133);
		delay_ms(50);
		LCD_WriteReg(0xA0,0x0000);
	}
	else if( DeviceCode == 0x4535 )
	{
		LCD_Code = LGDP4535;
		LCD_WriteReg(0x15, 0x0030); 	 /* Set the internal vcore voltage */
		LCD_WriteReg(0x9A, 0x0010); 	 /* Start internal OSC */
		LCD_WriteReg(0x11, 0x0020);	     /* set SS and SM bit */
		LCD_WriteReg(0x10, 0x3428);	     /* set 1 line inversion */
		LCD_WriteReg(0x12, 0x0002);	     /* set GRAM write direction and BGR=1 */
		LCD_WriteReg(0x13, 0x1038);	     /* Resize register */
		delay_ms(40);
		LCD_WriteReg(0x12, 0x0012);	     /* set the back porch and front porch */
		delay_ms(40);
		LCD_WriteReg(0x10, 0x3420);	     /* set non-display area refresh cycle ISC[3:0] */
		LCD_WriteReg(0x13, 0x3045);	     /* FMARK function */
		delay_ms(70);
		LCD_WriteReg(0x30, 0x0000);      /* RGB interface setting */
		LCD_WriteReg(0x31, 0x0402);	     /* Frame marker Position */
		LCD_WriteReg(0x32, 0x0307);      /* RGB interface polarity */
		LCD_WriteReg(0x33, 0x0304);      /* SAP, BT[3:0], AP, DSTB, SLP, STB */
		LCD_WriteReg(0x34, 0x0004);      /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x35, 0x0401);      /* VREG1OUT voltage */
		LCD_WriteReg(0x36, 0x0707);      /* VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x37, 0x0305);      /* SAP, BT[3:0], AP, DSTB, SLP, STB */
		LCD_WriteReg(0x38, 0x0610);      /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x39, 0x0610);      /* VREG1OUT voltage */
		LCD_WriteReg(0x01, 0x0100);      /* VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x02, 0x0300);      /* VCM[4:0] for VCOMH */
		LCD_WriteReg(0x03, 0x1030);      /* GRAM horizontal Address */
		LCD_WriteReg(0x08, 0x0808);      /* GRAM Vertical Address */
		LCD_WriteReg(0x0A, 0x0008);
		LCD_WriteReg(0x60, 0x2700);	     /* Gate Scan Line */
		LCD_WriteReg(0x61, 0x0001);	     /* NDL,VLE, REV */
		LCD_WriteReg(0x90, 0x013E);
		LCD_WriteReg(0x92, 0x0100);
		LCD_WriteReg(0x93, 0x0100);
		LCD_WriteReg(0xA0, 0x3000);
		LCD_WriteReg(0xA3, 0x0010);
		LCD_WriteReg(0x07, 0x0001);
		LCD_WriteReg(0x07, 0x0021);
		LCD_WriteReg(0x07, 0x0023);
		LCD_WriteReg(0x07, 0x0033);
		LCD_WriteReg(0x07, 0x0133);
	}
	else if( DeviceCode == 0x0047 )
	{
		LCD_Code = HX8347D;
		/* Start Initial Sequence */
		LCD_WriteReg(0xEA,0x00);
		LCD_WriteReg(0xEB,0x20);
		LCD_WriteReg(0xEC,0x0C);
		LCD_WriteReg(0xED,0xC4);
		LCD_WriteReg(0xE8,0x40);
		LCD_WriteReg(0xE9,0x38);
		LCD_WriteReg(0xF1,0x01);
		LCD_WriteReg(0xF2,0x10);
		LCD_WriteReg(0x27,0xA3);
		/* GAMMA SETTING */
		LCD_WriteReg(0x40,0x01);
		LCD_WriteReg(0x41,0x00);
		LCD_WriteReg(0x42,0x00);
		LCD_WriteReg(0x43,0x10);
		LCD_WriteReg(0x44,0x0E);
		LCD_WriteReg(0x45,0x24);
		LCD_WriteReg(0x46,0x04);
		LCD_WriteReg(0x47,0x50);
		LCD_WriteReg(0x48,0x02);
		LCD_WriteReg(0x49,0x13);
		LCD_WriteReg(0x4A,0x19);
		LCD_WriteReg(0x4B,0x19);
		LCD_WriteReg(0x4C,0x16);
		LCD_WriteReg(0x50,0x1B);
		LCD_WriteReg(0x51,0x31);
		LCD_WriteReg(0x52,0x2F);
		LCD_WriteReg(0x53,0x3F);
		LCD_WriteReg(0x54,0x3F);
		LCD_WriteReg(0x55,0x3E);
		LCD_WriteReg(0x56,0x2F);
		LCD_WriteReg(0x57,0x7B);
		LCD_WriteReg(0x58,0x09);
		LCD_WriteReg(0x59,0x06);
		LCD_WriteReg(0x5A,0x06);
		LCD_WriteReg(0x5B,0x0C);
		LCD_WriteReg(0x5C,0x1D);
		LCD_WriteReg(0x5D,0xCC);
		/* Power Voltage Setting */
		LCD_WriteReg(0x1B,0x18);
		LCD_WriteReg(0x1A,0x01);
		LCD_WriteReg(0x24,0x15);
		LCD_WriteReg(0x25,0x50);
		LCD_WriteReg(0x23,0x8B);
		LCD_WriteReg(0x18,0x36);
		LCD_WriteReg(0x19,0x01);
		LCD_WriteReg(0x01,0x00);
		LCD_WriteReg(0x1F,0x88);
		delay_ms(50);
		LCD_WriteReg(0x1F,0x80);
		delay_ms(50);
		LCD_WriteReg(0x1F,0x90);
		delay_ms(50);
		LCD_WriteReg(0x1F,0xD0);
		delay_ms(50);
		LCD_WriteReg(0x17,0x05);
		LCD_WriteReg(0x36,0x00);
		LCD_WriteReg(0x28,0x38);
		delay_ms(50);
		LCD_WriteReg(0x28,0x3C);
	}
	else if( DeviceCode == 0x7783 )
	{
		LCD_Code = ST7781;
		/* Start Initial Sequence */
		LCD_WriteReg(0x00FF,0x0001);
		LCD_WriteReg(0x00F3,0x0008);
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
		LCD_WriteReg(0x0003,0x1030);
		LCD_WriteReg(0x0008,0x0302);
		LCD_WriteReg(0x0008,0x0207);
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000);
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0005);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		delay_ms(50);
		LCD_WriteReg(0x0010,0x12B0);
		delay_ms(50);
		LCD_WriteReg(0x0011,0x0007);
		delay_ms(50);
		LCD_WriteReg(0x0012,0x008B);
		delay_ms(50);
		LCD_WriteReg(0x0013,0x1700);
		delay_ms(50);
		LCD_WriteReg(0x0029,0x0022);
		LCD_WriteReg(0x0030,0x0000);
		LCD_WriteReg(0x0031,0x0707);
		LCD_WriteReg(0x0032,0x0505);
		LCD_WriteReg(0x0035,0x0107);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0000);
		LCD_WriteReg(0x0038,0x0202);
		LCD_WriteReg(0x0039,0x0106);
		LCD_WriteReg(0x003C,0x0202);
		LCD_WriteReg(0x003D,0x0408);
		delay_ms(50);
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013F);
		LCD_WriteReg(0x0060,0xA700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x0090,0x0033);
		LCD_WriteReg(0x002B,0x000B);
		LCD_WriteReg(0x0007,0x0133);
	}
	else	/* special ID */
	{
		DeviceCode = 0x0123;
		DeviceCode = LCD_ReadReg(0x67);
		if( DeviceCode == 0x0047 )
		{
	        LCD_Code = HX8347A;
	        LCD_WriteReg(0x0042,0x0008);
	        /* Gamma setting */
		    LCD_WriteReg(0x0046,0x00B4);
		    LCD_WriteReg(0x0047,0x0043);
		    LCD_WriteReg(0x0048,0x0013);
		    LCD_WriteReg(0x0049,0x0047);
		    LCD_WriteReg(0x004A,0x0014);
		    LCD_WriteReg(0x004B,0x0036);
		    LCD_WriteReg(0x004C,0x0003);
		    LCD_WriteReg(0x004D,0x0046);
		    LCD_WriteReg(0x004E,0x0005);
		    LCD_WriteReg(0x004F,0x0010);
		    LCD_WriteReg(0x0050,0x0008);
		    LCD_WriteReg(0x0051,0x000a);
		    /* Window Setting */
		    LCD_WriteReg(0x0002,0x0000);
		    LCD_WriteReg(0x0003,0x0000);
		    LCD_WriteReg(0x0004,0x0000);
		    LCD_WriteReg(0x0005,0x00EF);
		    LCD_WriteReg(0x0006,0x0000);
		    LCD_WriteReg(0x0007,0x0000);
		    LCD_WriteReg(0x0008,0x0001);
		    LCD_WriteReg(0x0009,0x003F);
		    delay_ms(10);
		    LCD_WriteReg(0x0001,0x0006);
		    LCD_WriteReg(0x0016,0x00C8);
		    LCD_WriteReg(0x0023,0x0095);
		    LCD_WriteReg(0x0024,0x0095);
		    LCD_WriteReg(0x0025,0x00FF);
		    LCD_WriteReg(0x0027,0x0002);
		    LCD_WriteReg(0x0028,0x0002);
		    LCD_WriteReg(0x0029,0x0002);
		    LCD_WriteReg(0x002A,0x0002);
		    LCD_WriteReg(0x002C,0x0002);
		    LCD_WriteReg(0x002D,0x0002);
		    LCD_WriteReg(0x003A,0x0001);
		    LCD_WriteReg(0x003B,0x0001);
		    LCD_WriteReg(0x003C,0x00F0);
		    LCD_WriteReg(0x003D,0x0000);
		    delay_ms(20);
		    LCD_WriteReg(0x0035,0x0038);
		    LCD_WriteReg(0x0036,0x0078);
		    LCD_WriteReg(0x003E,0x0038);
		    LCD_WriteReg(0x0040,0x000F);
		    LCD_WriteReg(0x0041,0x00F0);
		    LCD_WriteReg(0x0038,0x0000);
		    /* Power Setting */
		    LCD_WriteReg(0x0019,0x0049);
		    LCD_WriteReg(0x0093,0x000A);
		    delay_ms(10);
		    LCD_WriteReg(0x0020,0x0020);
		    LCD_WriteReg(0x001D,0x0003);
		    LCD_WriteReg(0x001E,0x0000);
		    LCD_WriteReg(0x001F,0x0009);
		    LCD_WriteReg(0x0044,0x0053);
		    LCD_WriteReg(0x0045,0x0010);
		    delay_ms(10);
		    LCD_WriteReg(0x001C,0x0004);
		    delay_ms(20);
		    LCD_WriteReg(0x0043,0x0080);
		    delay_ms(5);
		    LCD_WriteReg(0x001B,0x000a);
		    delay_ms(40);
		    LCD_WriteReg(0x001B,0x0012);
		    delay_ms(40);
		    /* Display On Setting */
		    LCD_WriteReg(0x0090,0x007F);
		    LCD_WriteReg(0x0026,0x0004);
		    delay_ms(40);
		    LCD_WriteReg(0x0026,0x0024);
		    LCD_WriteReg(0x0026,0x002C);
		    delay_ms(40);
		    LCD_WriteReg(0x0070,0x0008);
		    LCD_WriteReg(0x0026,0x003C);
		    LCD_WriteReg(0x0057,0x0002);
		    LCD_WriteReg(0x0055,0x0000);
		    LCD_WriteReg(0x0057,0x0000);
		}
	}
    delay_ms(50);   /* delay 50 ms */

}

/******************************************************************************
* Function Name  : LCD_SetArea
* Description    : Sets Area.
* Input          : - start_x: start column
*                  - start_y: start row
*				   - end_x: end column
*				   - end_y: end row
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/*
static __inline void LCD_SetArea(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
	LCD_WriteCommand(CMD_SET_COLUMN);
	LCD_WriteData(start_x>>8);
	LCD_WriteData(start_x);
	LCD_WriteData(end_x>>8);
	LCD_WriteData(end_x);

	LCD_WriteCommand(CMD_SET_PAGE);
	LCD_WriteData(start_y>>8);
	LCD_WriteData(start_y);
	LCD_WriteData(end_y>>8);
	LCD_WriteData(end_y);
}
*/
/*******************************************************************************
* Function Name  : LCD_SetBacklight
* Description    : This function makes use of PWM feature of ssd1963 to adjust
*				   the backlight intensity.
* Input          : - intensity: intensity from
*                               0x00 (total backlight shutdown, PWM pin pull-down to VSS)
                                0xff (99% pull-up, 255/256 pull-up to VDD)
* Output         : None
* Return         : None
* Attention		 : The base frequency of PWM set to around 300Hz with PLL set to 120MHz.
*                  This parameter is hardware dependent.
*                  Backlight circuit with shutdown pin connected to PWM output of ssd1963.
*******************************************************************************/
//void LCD_SetBacklight(uint8_t intensity)
//{
//	LCD_WriteCommand(0xBE);		    /* Set PWM configuration for backlight control */
//	LCD_WriteData(0x0E);			/* PWMF[7:0] = 14, PWM base freq = PLL/(256*(1+14))/256 = */
//								    /* 122Hz for a PLL freq = 120MHz */
//	LCD_WriteData(intensity);		/* Set duty cycle, from 0x00 (total pull-down) to 0xFF */
//								    /* (99% pull-up , 255/256) */
//	LCD_WriteData(0x01);			/* PWM enabled and controlled by host (mcu) */
//	LCD_WriteData(0x00);
//	LCD_WriteData(0x00);
//	LCD_WriteData(0x00);
//}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
/*
void LCD_Clear(uint16_t Color)
{
	uint32_t index;

	LCD_SetArea(0,0,DISP_HOR_RESOLUTION -1 ,DISP_VER_RESOLUTION -1 );
	LCD_WriteCommand(CMD_WR_MEMSTART);
	for( index=0; index < DISP_HOR_RESOLUTION * DISP_VER_RESOLUTION; index++ )
	{
		LCD_WriteData(Color);
	}
}
*/
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;

	if( LCD_Code == HX8347D || LCD_Code == HX8347A )
	{
		LCD_WriteReg(0x02,0x00);
		LCD_WriteReg(0x03,0x00);

		LCD_WriteReg(0x04,0x00);
		LCD_WriteReg(0x05,0xEF);

		LCD_WriteReg(0x06,0x00);
		LCD_WriteReg(0x07,0x00);

		LCD_WriteReg(0x08,0x01);
		LCD_WriteReg(0x09,0x3F);
	}
	else
	{
		LCD_SetCursor(0,0);
	}
//	Clr_Cs;
	LCD_WriteIndex(0x0022);
	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}
//	Set_Cs;
}

static void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{

	  #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

		uint16_t temp;
		Ypos = ( MAX_Y - 1 ) - Ypos;
		temp = Ypos;
		Ypos = Xpos;
		Xpos = temp;

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

		Ypos = ( MAX_Y - 1 ) - Ypos;

	#endif
  switch( LCD_Code )
  {
     default:		 /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );
          LCD_WriteReg(0x0021, Ypos );
	      break;

     case SSD1298: 	 /* 0x8999 */
     case SSD1289:   /* 0x8989 */
	      LCD_WriteReg(0x004e, Xpos );
        LCD_WriteReg(0x004f, Ypos );
	      break;

     case HX8347A: 	 /* 0x0047 */
     case HX8347D: 	 /* 0x0047 */
	      LCD_WriteReg(0x02, Xpos>>8 );
	      LCD_WriteReg(0x03, Xpos );

	      LCD_WriteReg(0x06, Ypos>>8 );
	      LCD_WriteReg(0x07, Ypos );

	      break;
     case SSD2119:	 /* 3.5 LCD 0x9919 */
	      break;
  }
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* Input          : - color: BRG 颜色值
* Output         : None
* Return         : RGB 颜色值
* Attention		 : 内部函数调用
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;

	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;

	rgb =  (b<<11) + (g<<5) + (r<<0);

	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : 获取指定座标的颜色值
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
//uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
//{
//	LCD_SetArea(Xpos,Ypos,Xpos,Ypos);
//	LCD_WriteCommand(CMD_RD_MEMSTART);
//	return LCD_ReadData();
//}
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
	uint16_t dummy;

	LCD_SetCursor(Xpos,Ypos);
//	Clr_Cs;
	LCD_WriteIndex(0x0022);

	switch( LCD_Code )
	{
		case ST7781:
		case LGDP4531:
		case LGDP4535:
		case SSD1289:
		case SSD1298:
             dummy = LCD_ReadData();   /* 空读一次 */
             dummy = LCD_ReadData();
//		     Set_Cs;
 		     return  dummy;
	    case HX8347A:
	    case HX8347D:
             {
		        uint8_t red,green,blue;
		        red = LCD_ReadData()>>3;
                green = LCD_ReadData()>>3;
                blue = LCD_ReadData()>>2;
                dummy = ( green << 11 ) | (blue << 5 ) | red;
		     }
//		     Set_Cs;
	         return  dummy;

        default:	/* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x9919 */
             dummy = LCD_ReadData();   /* 空读一次 */
             dummy = LCD_ReadData();
//		     Set_Cs;
 		     return  LCD_BGR2RGB( dummy );
	}
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 在指定座标画点
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}



/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x1: A点行座标
*                  - y1: A点列座标
*				   - x2: B点行座标
*				   - y2: B点列座标
*				   - color: 线颜色
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
    short dx,dy;      /* 定义X Y轴上增加的变量值 */
    short temp;       /* 起点 终点大小比较 交换数据时的中间变量 */

    if( x0 > x1 )     /* X轴上起点大于终点 交换数据 */
    {
	    temp = x1;
		x1 = x0;
		x0 = temp;
    }
    if( y0 > y1 )     /* Y轴上起点大于终点 交换数据 */
    {
		temp = y1;
		y1 = y0;
		y0 = temp;
    }

	dx = x1-x0;       /* X轴方向上的增量 */
	dy = y1-y0;       /* Y轴方向上的增量 */

    if( dx == 0 )     /* X轴上没有增量 画垂直线 */
    {
        do
        {
            LCD_SetPoint(x0, y0, color);   /* 逐点显示 描垂直线 */
            y0++;
        }
        while( y1 >= y0 );
		return;
    }
    if( dy == 0 )     /* Y轴上没有增量 画水平直线 */
    {
        do
        {
            LCD_SetPoint(x0, y0, color);   /* 逐点显示 描水平线 */
            x0++;
        }
        while( x1 >= x0 );
		return;
    }
	/* 布兰森汉姆(Bresenham)算法画线 */
    if( dx > dy )                         /* 靠近X轴 */
    {
	    temp = 2 * dy - dx;               /* 计算下个点的位置 */
        while( x0 != x1 )
        {
	        LCD_SetPoint(x0,y0,color);    /* 画起点 */
	        x0++;                         /* X轴上加1 */
	        if( temp > 0 )                /* 判断下下个点的位置 */
	        {
	            y0++;                     /* 为右上相邻点，即（x0+1,y0+1） */
	            temp += 2 * dy - 2 * dx;
	 	    }
            else
            {
			    temp += 2 * dy;           /* 判断下下个点的位置 */
			}
        }
        LCD_SetPoint(x0,y0,color);
    }
    else
    {
	    temp = 2 * dx - dy;                      /* 靠近Y轴 */
        while( y0 != y1 )
        {
	 	    LCD_SetPoint(x0,y0,color);
            y0++;
            if( temp > 0 )
            {
                x0++;
                temp+=2*dy-2*dx;
            }
            else
			{
                temp += 2 * dy;
			}
        }
        LCD_SetPoint(x0,y0,color);
	}
} 

/******************************************************************************
* Function Name  : PutChar
* Description    : 将Lcd屏上任意位置显示一个字符
* Input          : - Xpos: 水平坐标 
*                  - Ypos: 垂直坐标  
*				   - ASCI: 显示的字符
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);  /* 取字模数据 */
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );  /* 字符颜色 */
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );  /* 背景颜色 */
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标 
*				   - str: 字符串
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        }
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}


/******************************************************************************
* Function Name  : PutChinese
* Description    : 将Lcd屏上任意位置显示一个中文字
* Input          : - Xpos: 水平坐标 
*                  - Ypos: 垂直坐标  
*				   - str: 显示的中文字
*				   - Color: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor)
{
	uint8_t i,j;
	uint8_t buffer[32];
	uint16_t tmp_char=0;

	GetGBKCode(buffer,str);  /* 取字模数据 */

	for ( i = 0; i < 16; i++ )
	{
		tmp_char = buffer[i*2];
		tmp_char = ( tmp_char << 8 );
		tmp_char |= buffer[2*i+1];
		for (j = 0; j < 16; j++ )
		{
		    if ( (tmp_char >> 15-j ) & 0x01 == 0x01 )
	        {
		        LCD_SetPoint(Xpos+j,Ypos+i,Color);  /* 字符颜色 */
	        }
	        else
	        {
	            LCD_SetPoint(Xpos+j,Ypos+i,bkColor);  /* 背景颜色 */
	        }
	    }
	}
}

/******************************************************************************
* Function Name  : GUI_Chinese
* Description    : 在指定座标显示字符串
* Input          : - Xpos: 行座标
*                  - Ypos: 列座标 
*				   - str: 字符串
*				   - charColor: 字符颜色   
*				   - bkColor: 背景颜色 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
	do
	{
		PutChinese(Xpos,Ypos,str++,Color,bkColor);
		str++;
		if ( Xpos < MAX_X - 16 )
		{
			Xpos += 16;
		}
		else if ( Ypos < MAX_Y - 16 )
		{
			Xpos = 0;
			Ypos += 16;
		}
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while(*str!=0);
}  

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : LCD控制器寄存器地址
* Input          : - index: 寄存器地址
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteIndex(uint16_t index)
{
	LCD_REG	= index;
/*
	Clr_Rs;
	Set_nRd;

	GPIOE->ODR = index;	 // GPIO_Write(GPIOE,index);

	Clr_nWr;
	Set_nWr;
*/
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : LCD寄存器数据
* Input          : - index: 寄存器数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteData(uint16_t data)
{
	LCD_RAM = data;
/*
	Set_Rs;

	GPIOE->ODR = data;	 //GPIO_Write(GPIOE,data);

	Clr_nWr;
	Set_nWr;
*/
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 读取控制器数据
* Input          : None
* Output         : None
* Return         : 返回读取到的数据
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadData(void)
{
//	uint16_t value;

//	Set_Rs;
//	Set_nWr;
//	Clr_nRd;

/*
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   */
/*
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
*/
//	GPIOE->CRH = 0x44444444;
//	GPIOE->CRL = 0x44444444;
/*
    value = GPIO_ReadInputData(GPIOE);
    value = GPIO_ReadInputData(GPIOE);
*/
//    value = GPIOE->IDR;
//    value = GPIOE->IDR;

/*
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   */
/*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
*/
//    GPIOE->CRH = 0x33333333;
//    GPIOE->CRL = 0x33333333;

//    Set_nRd;

//    return value;
	return LCD_RAM;
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
//	Clr_Cs;
	LCD_WriteIndex(LCD_Reg);
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);
//	Set_Cs;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
//	uint16_t LCD_RAM;

	/* Write 16-bit Index (then Read Reg) */
//	Clr_Cs;
	LCD_WriteIndex(LCD_Reg);
//	delay_ms(1);  /* delay 5 ms */

	/* Read 16-bit Reg */
//	LCD_RAM = LCD_ReadData();
//	Set_Cs;
//	return LCD_RAM;
	return LCD_ReadData();
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


///****************************************Copyright (c)**************************************************
//**
//**                                 http://www.powermcu.com
//**
//**--------------File Info-------------------------------------------------------------------------------
//** File name:			GLCD.c
//** Descriptions:		STM32 FSMC TFT操作函数库
//**
//**------------------------------------------------------------------------------------------------------
//** Created by:			poweravr
//** Created date:		2010-11-7
//** Version:				1.0
//** Descriptions:		The original version
//**
//**------------------------------------------------------------------------------------------------------
//** Modified by:
//** Modified date:
//** Version:
//** Descriptions:
//********************************************************************************************************/
//
///* Includes ------------------------------------------------------------------*/
//#include "GLCD.h"
//#include "HzLib.h"
//#include "AsciiLib.h"
//#include <math.h>
//
///* Private variables ---------------------------------------------------------*/
//static uint16_t DeviceCode;
//static uint16_t TimerPeriod = 0;
//static uint16_t Channel2Pulse = 1000;
//
///* Private define ------------------------------------------------------------*/
///* 使用总线方式时定义地址 */
///* 挂在不同的BANK,使用不同地址线时请自行换算地址 */
//#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
//#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */
//
//
///*******************************************************************************
//* Function Name  : LCD_CtrlLinesConfig
//* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
//                   Push-Pull mode.
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static void LCD_CtrlLinesConfig(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//
//  /* Enable FSMC, GPIOD, GPIOE and AFIO clocks */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
//
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
//
//  /* PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9),
//     PE.13(D10), PE.14(D11), PE.15(D12) */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
//                                 GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
//                                 GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//  /* PD.00(D2), PD.01(D3), PD.04(RD), PD.5(WR), PD.7(CS), PD.8(D13), PD.9(D14),
//     PD.10(D15), PD.11(RS) PD.14(D0) PD.15(D1) */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |
//                                 GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
//                                 GPIO_Pin_14 | GPIO_Pin_15;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//}
//
///*******************************************************************************
//* Function Name  : LCD_FSMCConfig
//* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static void LCD_FSMCConfig(void)
//{
//  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
//  FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
//  /* FSMC读速度设置 */
//  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 5;  /* 地址建立时间  */
//  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
//  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 5;	   /* 数据建立时间  */
//  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
//  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
//  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
//  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */
//
//  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
//  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
//  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
//  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
//  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
//  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
//  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
//  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
////  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
////  /* FSMC写速度设置 */
//  FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 15;   /* 地址建立时间  */
//  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
//  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 15;	   /* 数据建立时间  */
//  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
//  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
//  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
//  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	/* FSMC 访问模式 */
//  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
////
//  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
//
//  /* Enable FSMC Bank1_SRAM Bank */
//  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
//}
//
///*******************************************************************************
//* Function Name  : LCD_Configuration
//* Description    : Configure the LCD Control pins and FSMC Parallel interface
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static void LCD_Configuration(void)
//{
// /* Configure the LCD Control pins --------------------------------------------*/
//  LCD_CtrlLinesConfig();
//
///* Configure the FSMC Parallel interface -------------------------------------*/
//  LCD_FSMCConfig();
//}
//
///*******************************************************************************
//* Function Name  : LCD_WriteReg
//* Description    : Writes to the selected LCD register.
//* Input          : - LCD_Reg: address of the selected register.
//*                  - LCD_RegValue: value to write to the selected register.
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static __inline void LCD_WriteReg(uint8_t LCD_Reg,uint16_t LCD_RegValue)
//{
//  /* Write 16-bit Index, then Write Reg */
//  LCD_REG = LCD_Reg;
//  /* Write 16-bit Reg */
//  LCD_RAM = LCD_RegValue;
//}
//
///*******************************************************************************
//* Function Name  : LCD_WriteReg
//* Description    : Reads the selected LCD Register.
//* Input          : None
//* Output         : None
//* Return         : LCD Register Value.
//* Attention		 : None
//*******************************************************************************/
//static __inline uint16_t LCD_ReadReg(uint8_t LCD_Reg)
//{
//  /* Write 16-bit Index (then Read Reg) */
//  LCD_REG = LCD_Reg;
//  /* Read 16-bit Reg */
//  return (LCD_RAM);
//}
//
///*******************************************************************************
//* Function Name  : LCD_WriteRAM_Prepare
//* Description    : Prepare to write to the LCD RAM.
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static __inline void LCD_WriteRAM_Prepare(void)
//{
//  LCD_REG = R34;
//}
//
///*******************************************************************************
//* Function Name  : LCD_WriteRAM
//* Description    : Writes to the LCD RAM.
//* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static __inline void LCD_WriteRAM(uint16_t RGB_Code)
//{
//  /* Write 16-bit GRAM Reg */
//  LCD_RAM = RGB_Code;
//}
//
///*******************************************************************************
//* Function Name  : LCD_ReadRAM
//* Description    : Reads the LCD RAM.
//* Input          : None
//* Output         : None
//* Return         : LCD RAM Value.
//* Attention		 : None
//*******************************************************************************/
//static __inline uint16_t LCD_ReadRAM(void)
//{
//  volatile uint16_t dummy;
//  /* Write 16-bit Index (then Read Reg) */
//  LCD_REG = R34; /* Select GRAM Reg */
//  /* Read 16-bit Reg */
//  dummy = LCD_RAM;
//
//  return LCD_RAM;
//}
//
///*******************************************************************************
//* Function Name  : LCD_SetCursor
//* Description    : Sets the cursor position.
//* Input          : - Xpos: specifies the X position.
//*                  - Ypos: specifies the Y position.
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static void LCD_SetCursor(uint16_t Xpos,uint16_t Ypos)
//{
//  if(DeviceCode==0x8989)
//  {
//    LCD_WriteReg(0x004e,Xpos); /* Row */
//    LCD_WriteReg(0x004f,Ypos); /* Line */
//  }
//  else if(DeviceCode==0x9919)
//  {
//    LCD_WriteReg(0x004e,Xpos); /* Row */
//    LCD_WriteReg(0x004f,Ypos); /* Line */
//  }
//  else
//  {
//    LCD_WriteReg(0x0020,Xpos); /* Row */
//    LCD_WriteReg(0x0021,Ypos); /* Line */
//  }
//}
//
///*******************************************************************************
//* Function Name  : LCD_Delay
//* Description    : Delay Time
//* Input          : - nCount: Delay Time
//* Output         : None
//* Return         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//static void LCD_Delay(uint16_t nCount)
//{
// uint16_t TimingDelay;
//
// delay_ms(nCount*10);
//
//// while(nCount--)
////   {
////    for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
////   }
//}
//
///*******************************************************************************
//* Function Name  : LCD_Initializtion
//* Description    : Initialize TFT Controller.
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void LCD_Initializtion()
//{
//  LCD_Configuration();
//  LCD_Delay(5);  /* delay 50 ms */
//  DeviceCode = 0x8989;
//  //DeviceCode = LCD_ReadReg(0x0000);
//  if(DeviceCode==0x9325 || DeviceCode==0x9328)	/* 不同屏驱动IC 初始化不同 */
//  {
//    LCD_WriteReg(0x00e7,0x0010);
//    LCD_WriteReg(0x0000,0x0001);  	/* start internal osc */
//    LCD_WriteReg(0x0001,0x0100);
//    LCD_WriteReg(0x0002,0x0700); 	/* power on sequence */
//	LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3) ); 	/* importance */
//    LCD_WriteReg(0x0004,0x0000);
//    LCD_WriteReg(0x0008,0x0207);
//    LCD_WriteReg(0x0009,0x0000);
//    LCD_WriteReg(0x000a,0x0000); 	/* display setting */
//    LCD_WriteReg(0x000c,0x0001);	/* display setting */
//    LCD_WriteReg(0x000d,0x0000);
//    LCD_WriteReg(0x000f,0x0000);
//    /* Power On sequence */
//    LCD_WriteReg(0x0010,0x0000);
//    LCD_WriteReg(0x0011,0x0007);
//    LCD_WriteReg(0x0012,0x0000);
//    LCD_WriteReg(0x0013,0x0000);
//    LCD_Delay(5);  /* delay 50 ms */
//    LCD_WriteReg(0x0010,0x1590);
//    LCD_WriteReg(0x0011,0x0227);
//    LCD_Delay(5);  /* delay 50 ms */
//    LCD_WriteReg(0x0012,0x009c);
//    LCD_Delay(5);  /* delay 50 ms */
//    LCD_WriteReg(0x0013,0x1900);
//    LCD_WriteReg(0x0029,0x0023);
//    LCD_WriteReg(0x002b,0x000e);
//    LCD_Delay(5);  /* delay 50 ms */
//    LCD_WriteReg(0x0020,0x0000);
//    LCD_WriteReg(0x0021,0x0000);
//    LCD_Delay(5);  /* delay 50 ms */
//    LCD_WriteReg(0x0030,0x0007);
//    LCD_WriteReg(0x0031,0x0707);
//    LCD_WriteReg(0x0032,0x0006);
//    LCD_WriteReg(0x0035,0x0704);
//    LCD_WriteReg(0x0036,0x1f04);
//    LCD_WriteReg(0x0037,0x0004);
//    LCD_WriteReg(0x0038,0x0000);
//    LCD_WriteReg(0x0039,0x0706);
//    LCD_WriteReg(0x003c,0x0701);
//    LCD_WriteReg(0x003d,0x000f);
//    LCD_Delay(5);  /* delay 50 ms */
//    LCD_WriteReg(0x0050,0x0000);
//    LCD_WriteReg(0x0051,0x00ef);
//    LCD_WriteReg(0x0052,0x0000);
//    LCD_WriteReg(0x0053,0x013f);
//    LCD_WriteReg(0x0060,0xa700);
//    LCD_WriteReg(0x0061,0x0001);
//    LCD_WriteReg(0x006a,0x0000);
//    LCD_WriteReg(0x0080,0x0000);
//    LCD_WriteReg(0x0081,0x0000);
//    LCD_WriteReg(0x0082,0x0000);
//    LCD_WriteReg(0x0083,0x0000);
//    LCD_WriteReg(0x0084,0x0000);
//    LCD_WriteReg(0x0085,0x0000);
//
//    LCD_WriteReg(0x0090,0x0010);
//    LCD_WriteReg(0x0092,0x0000);
//    LCD_WriteReg(0x0093,0x0003);
//    LCD_WriteReg(0x0095,0x0110);
//    LCD_WriteReg(0x0097,0x0000);
//    LCD_WriteReg(0x0098,0x0000);
//    /* display on sequence */
//    LCD_WriteReg(0x0007,0x0133);
//
//    LCD_WriteReg(0x0020,0x0000);  /* 行首址0 */
//    LCD_WriteReg(0x0021,0x0000);  /* 列首址0 */
//  }
//  else if(DeviceCode==0x9320 || DeviceCode==0x9300)
//  {
//    LCD_WriteReg(0x00,0x0000);
//	LCD_WriteReg(0x01,0x0100);	/* Driver Output Contral */
//	LCD_WriteReg(0x02,0x0700);	/* LCD Driver Waveform Contral */
//	LCD_WriteReg(0x03,0x1018);	/* Entry Mode Set */
//
//	LCD_WriteReg(0x04,0x0000);	/* Scalling Contral */
//    LCD_WriteReg(0x08,0x0202);	/* Display Contral */
//	LCD_WriteReg(0x09,0x0000);	/* Display Contral 3.(0x0000) */
//	LCD_WriteReg(0x0a,0x0000);	/* Frame Cycle Contal.(0x0000) */
//    LCD_WriteReg(0x0c,(1<<0));	/* Extern Display Interface Contral */
//	LCD_WriteReg(0x0d,0x0000);	/* Frame Maker Position */
//	LCD_WriteReg(0x0f,0x0000);	/* Extern Display Interface Contral 2. */
//
//    LCD_Delay(10);  /* delay 100 ms */
//	LCD_WriteReg(0x07,0x0101);	/* Display Contral */
//    LCD_Delay(10);  /* delay 100 ms */
//
//	LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	/* Power Control 1.(0x16b0)	*/
//	LCD_WriteReg(0x11,0x0007);								/* Power Control 2 */
//	LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				/* Power Control 3.(0x0138)	*/
//	LCD_WriteReg(0x13,0x0b00);								/* Power Control 4 */
//	LCD_WriteReg(0x29,0x0000);								/* Power Control 7 */
//
//	LCD_WriteReg(0x2b,(1<<14)|(1<<4));
//
//	LCD_WriteReg(0x50,0);       /* Set X Start */
//	LCD_WriteReg(0x51,239);	    /* Set X End */
//	LCD_WriteReg(0x52,0);	    /* Set Y Start */
//	LCD_WriteReg(0x53,319);	    /* Set Y End */
//
//	LCD_WriteReg(0x60,0x2700);	/* Driver Output Control */
//	LCD_WriteReg(0x61,0x0001);	/* Driver Output Control */
//	LCD_WriteReg(0x6a,0x0000);	/* Vertical Srcoll Control */
//
//	LCD_WriteReg(0x80,0x0000);	/* Display Position? Partial Display 1 */
//	LCD_WriteReg(0x81,0x0000);	/* RAM Address Start? Partial Display 1 */
//	LCD_WriteReg(0x82,0x0000);	/* RAM Address End-Partial Display 1 */
//	LCD_WriteReg(0x83,0x0000);	/* Displsy Position? Partial Display 2 */
//	LCD_WriteReg(0x84,0x0000);	/* RAM Address Start? Partial Display 2 */
//	LCD_WriteReg(0x85,0x0000);	/* RAM Address End? Partial Display 2 */
//
//    LCD_WriteReg(0x90,(0<<7)|(16<<0));	/* Frame Cycle Contral.(0x0013)	*/
//	LCD_WriteReg(0x92,0x0000);	/* Panel Interface Contral 2.(0x0000) */
//	LCD_WriteReg(0x93,0x0001);	/* Panel Interface Contral 3. */
//    LCD_WriteReg(0x95,0x0110);	/* Frame Cycle Contral.(0x0110)	*/
//	LCD_WriteReg(0x97,(0<<8));
//	LCD_WriteReg(0x98,0x0000);	/* Frame Cycle Contral */
//
//    LCD_WriteReg(0x07,0x0173);
//  }
//  else if(DeviceCode==0x9331)
//  {
//	LCD_WriteReg(0x00E7, 0x1014);
//	LCD_WriteReg(0x0001, 0x0100);   /* set SS and SM bit */
//	LCD_WriteReg(0x0002, 0x0200);   /* set 1 line inversion */
//	LCD_WriteReg(0x0003, 0x1030);   /* set GRAM write direction and BGR=1 */
//	LCD_WriteReg(0x0008, 0x0202);   /* set the back porch and front porch */
//    LCD_WriteReg(0x0009, 0x0000);   /* set non-display area refresh cycle ISC[3:0] */
//	LCD_WriteReg(0x000A, 0x0000);   /* FMARK function */
//	LCD_WriteReg(0x000C, 0x0000);   /* RGB interface setting */
//	LCD_WriteReg(0x000D, 0x0000);   /* Frame marker Position */
//	LCD_WriteReg(0x000F, 0x0000);   /* RGB interface polarity */
//	/* Power On sequence */
//	LCD_WriteReg(0x0010, 0x0000);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
//	LCD_WriteReg(0x0011, 0x0007);   /* DC1[2:0], DC0[2:0], VC[2:0] */
//	LCD_WriteReg(0x0012, 0x0000);   /* VREG1OUT voltage	*/
//	LCD_WriteReg(0x0013, 0x0000);   /* VDV[4:0] for VCOM amplitude */
//    LCD_Delay(20);                  /* delay 200 ms */
//	LCD_WriteReg(0x0010, 0x1690);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
//	LCD_WriteReg(0x0011, 0x0227);   /* DC1[2:0], DC0[2:0], VC[2:0] */
//    LCD_Delay(5);                   /* delay 50 ms */
//	LCD_WriteReg(0x0012, 0x000C);   /* Internal reference voltage= Vci	*/
//    LCD_Delay(5);                    /* delay 50 ms */
//	LCD_WriteReg(0x0013, 0x0800);   /* Set VDV[4:0] for VCOM amplitude */
//	LCD_WriteReg(0x0029, 0x0011);   /* Set VCM[5:0] for VCOMH */
//	LCD_WriteReg(0x002B, 0x000B);   /* Set Frame Rate */
//    LCD_Delay(5);                   /* delay 50 ms */
//	LCD_WriteReg(0x0020, 0x0000);   /* GRAM horizontal Address */
//	LCD_WriteReg(0x0021, 0x0000);   /* GRAM Vertical Address */
//	/* Adjust the Gamma Curve */
//	LCD_WriteReg(0x0030, 0x0000);
//	LCD_WriteReg(0x0031, 0x0106);
//	LCD_WriteReg(0x0032, 0x0000);
//	LCD_WriteReg(0x0035, 0x0204);
//	LCD_WriteReg(0x0036, 0x160A);
//	LCD_WriteReg(0x0037, 0x0707);
//	LCD_WriteReg(0x0038, 0x0106);
//	LCD_WriteReg(0x0039, 0x0707);
//	LCD_WriteReg(0x003C, 0x0402);
//	LCD_WriteReg(0x003D, 0x0C0F);
//	/* Set GRAM area */
//	LCD_WriteReg(0x0050, 0x0000);   /* Horizontal GRAM Start Address */
//	LCD_WriteReg(0x0051, 0x00EF);   /* Horizontal GRAM End Address */
//	LCD_WriteReg(0x0052, 0x0000);   /* Vertical GRAM Start Address */
//	LCD_WriteReg(0x0053, 0x013F);   /* Vertical GRAM Start Address */
//	LCD_WriteReg(0x0060, 0x2700);   /* Gate Scan Line */
//	LCD_WriteReg(0x0061, 0x0001);   /*  NDL,VLE, REV */
//	LCD_WriteReg(0x006A, 0x0000);   /* set scrolling line */
//	/* Partial Display Control */
//	LCD_WriteReg(0x0080, 0x0000);
//	LCD_WriteReg(0x0081, 0x0000);
//	LCD_WriteReg(0x0082, 0x0000);
//	LCD_WriteReg(0x0083, 0x0000);
//	LCD_WriteReg(0x0084, 0x0000);
//	LCD_WriteReg(0x0085, 0x0000);
//	/* Panel Control */
//	LCD_WriteReg(0x0090, 0x0010);
//	LCD_WriteReg(0x0092, 0x0600);
//	LCD_WriteReg(0x0007,0x0021);
//    LCD_Delay(5);                   /* delay 50 ms */
//	LCD_WriteReg(0x0007,0x0061);
//    LCD_Delay(5);                   /* delay 50 ms */
//	LCD_WriteReg(0x0007,0x0133);    /* 262K color and display ON */
//    LCD_Delay(5);                   /* delay 50 ms */
//  }
//  else if(DeviceCode==0x9919)
//  {
//    /* POWER ON &RESET DISPLAY OFF */
//	LCD_WriteReg(0x28,0x0006);
//	LCD_WriteReg(0x00,0x0001);
//	LCD_WriteReg(0x10,0x0000);
//	LCD_WriteReg(0x01,0x72ef);
//	LCD_WriteReg(0x02,0x0600);
//	LCD_WriteReg(0x03,0x6a38);
//	LCD_WriteReg(0x11,0x6874);
//	LCD_WriteReg(0x0f,0x0000);    /* RAM WRITE DATA MASK */
//	LCD_WriteReg(0x0b,0x5308);    /* RAM WRITE DATA MASK */
//	LCD_WriteReg(0x0c,0x0003);
//	LCD_WriteReg(0x0d,0x000a);
//	LCD_WriteReg(0x0e,0x2e00);
//	LCD_WriteReg(0x1e,0x00be);
//	LCD_WriteReg(0x25,0x8000);
//	LCD_WriteReg(0x26,0x7800);
//	LCD_WriteReg(0x27,0x0078);
//	LCD_WriteReg(0x4e,0x0000);
//	LCD_WriteReg(0x4f,0x0000);
//	LCD_WriteReg(0x12,0x08d9);
//	/* Adjust the Gamma Curve */
//	LCD_WriteReg(0x30,0x0000);
//	LCD_WriteReg(0x31,0x0104);
//	LCD_WriteReg(0x32,0x0100);
//    LCD_WriteReg(0x33,0x0305);
//    LCD_WriteReg(0x34,0x0505);
//	LCD_WriteReg(0x35,0x0305);
//    LCD_WriteReg(0x36,0x0707);
//    LCD_WriteReg(0x37,0x0300);
//	LCD_WriteReg(0x3a,0x1200);
//	LCD_WriteReg(0x3b,0x0800);
//    LCD_WriteReg(0x07,0x0033);
//  }
//  else if(DeviceCode==0x1505)
//  {
//    /* second release on 3/5  ,luminance is acceptable,water wave appear during camera preview */
//    LCD_WriteReg(0x0007,0x0000);
//    LCD_Delay(5);                   /* delay 50 ms */
//    LCD_WriteReg(0x0012,0x011C);    /* why need to set several times?	*/
//    LCD_WriteReg(0x00A4,0x0001);    /* NVM */
//    LCD_WriteReg(0x0008,0x000F);
//    LCD_WriteReg(0x000A,0x0008);
//    LCD_WriteReg(0x000D,0x0008);
//    /* GAMMA CONTROL */
//    LCD_WriteReg(0x0030,0x0707);
//    LCD_WriteReg(0x0031,0x0007);
//    LCD_WriteReg(0x0032,0x0603);
//    LCD_WriteReg(0x0033,0x0700);
//    LCD_WriteReg(0x0034,0x0202);
//    LCD_WriteReg(0x0035,0x0002);
//    LCD_WriteReg(0x0036,0x1F0F);
//    LCD_WriteReg(0x0037,0x0707);
//    LCD_WriteReg(0x0038,0x0000);
//    LCD_WriteReg(0x0039,0x0000);
//    LCD_WriteReg(0x003A,0x0707);
//    LCD_WriteReg(0x003B,0x0000);
//    LCD_WriteReg(0x003C,0x0007);
//    LCD_WriteReg(0x003D,0x0000);
//    LCD_Delay(5);                   /* delay 50 ms */
//    LCD_WriteReg(0x0007,0x0001);
//    LCD_WriteReg(0x0017,0x0001);    /* Power supply startup enable */
//    LCD_Delay(5);                   /* delay 50 ms */
//    /* power control */
//    LCD_WriteReg(0x0010,0x17A0);
//    LCD_WriteReg(0x0011,0x0217);    /* reference voltage VC[2:0]   Vciout = 1.00*Vcivl */
//    LCD_WriteReg(0x0012,0x011E);    /* Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?	*/
//    LCD_WriteReg(0x0013,0x0F00);    /* VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl */
//    LCD_WriteReg(0x002A,0x0000);
//    LCD_WriteReg(0x0029,0x000A);    /* Vcomh = VCM1[4:0]*Vreg1out    gate source voltage?? */
//    LCD_WriteReg(0x0012,0x013E);    /* power supply on */
//    /* Coordinates Control */
//    LCD_WriteReg(0x0050,0x0000);
//    LCD_WriteReg(0x0051,0x00EF);
//    LCD_WriteReg(0x0052,0x0000);
//    LCD_WriteReg(0x0053,0x013F);
//    /* Pannel Image Control */
//    LCD_WriteReg(0x0060,0x2700);
//    LCD_WriteReg(0x0061,0x0001);
//    LCD_WriteReg(0x006A,0x0000);
//    LCD_WriteReg(0x0080,0x0000);
//    /* Partial Image Control */
//    LCD_WriteReg(0x0081,0x0000);
//    LCD_WriteReg(0x0082,0x0000);
//    LCD_WriteReg(0x0083,0x0000);
//    LCD_WriteReg(0x0084,0x0000);
//    LCD_WriteReg(0x0085,0x0000);
//    /* Panel Interface Control */
//    LCD_WriteReg(0x0090,0x0013);      /* frenqucy */
//    LCD_WriteReg(0x0092,0x0300);
//    LCD_WriteReg(0x0093,0x0005);
//    LCD_WriteReg(0x0095,0x0000);
//    LCD_WriteReg(0x0097,0x0000);
//    LCD_WriteReg(0x0098,0x0000);
//
//    LCD_WriteReg(0x0001,0x0100);
//    LCD_WriteReg(0x0002,0x0700);
//    LCD_WriteReg(0x0003,0x1030);
//    LCD_WriteReg(0x0004,0x0000);
//    LCD_WriteReg(0x000C,0x0000);
//    LCD_WriteReg(0x000F,0x0000);
//    LCD_WriteReg(0x0020,0x0000);
//    LCD_WriteReg(0x0021,0x0000);
//    LCD_WriteReg(0x0007,0x0021);
//    LCD_Delay(20);                   /* delay 200 ms */
//    LCD_WriteReg(0x0007,0x0061);
//    LCD_Delay(20);                   /* delay 200 ms */
//    LCD_WriteReg(0x0007,0x0173);
//    LCD_Delay(20);                   /* delay 200 ms */
//  }
//  else if(DeviceCode==0x8989)
//  {
//    LCD_WriteReg(0x0000,0x0001);    LCD_Delay(5);   /* 打开晶振 */
//    LCD_WriteReg(0x0003,0xA8A4);    LCD_Delay(5);
//    LCD_WriteReg(0x000C,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x000D,0x080C);    LCD_Delay(5);
//    LCD_WriteReg(0x000E,0x2B00);    LCD_Delay(5);
//    LCD_WriteReg(0x001E,0x00B0);    LCD_Delay(5);
//    LCD_WriteReg(0x0001,0x2B3F);    LCD_Delay(5);   /* 驱动输出控制320*240 0x2B3F */
//    LCD_WriteReg(0x0002,0x0600);    LCD_Delay(5);
//    LCD_WriteReg(0x0010,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0011,0x6070);    LCD_Delay(5);   /* 定义数据格式 16位色 横屏 0x6070 */
//    LCD_WriteReg(0x0005,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0006,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0016,0xEF1C);    LCD_Delay(5);
//    LCD_WriteReg(0x0017,0x0003);    LCD_Delay(5);
//    LCD_WriteReg(0x0007,0x0133);    LCD_Delay(5);
//    LCD_WriteReg(0x000B,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x000F,0x0000);    LCD_Delay(5);   /* 扫描开始地址 */
//    LCD_WriteReg(0x0041,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0042,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0048,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0049,0x013F);    LCD_Delay(5);
//    LCD_WriteReg(0x004A,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x004B,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0044,0xEF00);    LCD_Delay(5);
//    LCD_WriteReg(0x0045,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0046,0x013F);    LCD_Delay(5);
//    LCD_WriteReg(0x0030,0x0707);    LCD_Delay(5);
//    LCD_WriteReg(0x0031,0x0204);    LCD_Delay(5);
//    LCD_WriteReg(0x0032,0x0204);    LCD_Delay(5);
//    LCD_WriteReg(0x0033,0x0502);    LCD_Delay(5);
//    LCD_WriteReg(0x0034,0x0507);    LCD_Delay(5);
//    LCD_WriteReg(0x0035,0x0204);    LCD_Delay(5);
//    LCD_WriteReg(0x0036,0x0204);    LCD_Delay(5);
//    LCD_WriteReg(0x0037,0x0502);    LCD_Delay(5);
//    LCD_WriteReg(0x003A,0x0302);    LCD_Delay(5);
//    LCD_WriteReg(0x003B,0x0302);    LCD_Delay(5);
//    LCD_WriteReg(0x0023,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0024,0x0000);    LCD_Delay(5);
//    LCD_WriteReg(0x0025,0x8000);    LCD_Delay(5);
//    LCD_WriteReg(0x004f,0);        /* 行首址0 */
//    LCD_WriteReg(0x004e,0);        /* 列首址0 */
//  }
//  LCD_Delay(5);  /* delay 50 ms */
//}
//
///******************************************************************************
//* Function Name  : LCD_SetWindows
//* Description    : Sets Windows Area.
//* Input          : - StartX: Row Start Coordinate
//*                  - StartY: Line Start Coordinate
//*				   - xLong:
//*				   - yLong:
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void LCD_SetWindows(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong)
//{
//  LCD_SetCursor(xStart,yStart);
//  LCD_WriteReg(0x0050,xStart);         /* 水平GRAM起始位置 */
//  LCD_WriteReg(0x0051,xStart+xLong-1); /* 水平GRAM终止位置 */
//  LCD_WriteReg(0x0052,yStart);         /* 垂直GRAM起始位置 */
//  LCD_WriteReg(0x0053,yStart+yLong-1); /* 垂直GRAM终止位置 */
//}
//
///*******************************************************************************
//* Function Name  : LCD_Clear
//* Description    : 将屏幕填充成指定的颜色，如清屏，则填充 0xffff
//* Input          : - Color: Screen Color
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void LCD_Clear(uint16_t Color)
//{
//  uint32_t index=0;
//  LCD_SetCursor(0,0);
//  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
//  for(index=0;index<76800;index++)
//   {
//     LCD_RAM=Color;
//   }
//}
//
///******************************************************************************
//* Function Name  : LCD_GetPoint
//* Description    : 获取指定座标的颜色值
//* Input          : - Xpos: Row Coordinate
//*                  - Xpos: Line Coordinate
//* Output         : None
//* Return         : Screen Color
//* Attention		 : None
//*******************************************************************************/
//uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
//{
//  LCD_SetCursor(Xpos,Ypos);
//  if( DeviceCode==0x7783 || DeviceCode==0x4531 || DeviceCode==0x8989 )
//    return ( LCD_ReadRAM() );
//  else
//    return ( LCD_BGR2RGB(LCD_ReadRAM()) );
//}
//
///******************************************************************************
//* Function Name  : LCD_SetPoint
//* Description    : 在指定座标画点
//* Input          : - Xpos: Row Coordinate
//*                  - Ypos: Line Coordinate
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
//{
//  if ( ( Xpos > 239 ) ||( Ypos > 319 ) ) return;
//  LCD_SetCursor(Xpos,Ypos);
//  LCD_WriteRAM_Prepare();
//  LCD_WriteRAM(point);
//}
//
///******************************************************************************
//* Function Name  : LCD_DrawPicture
//* Description    : 在指定坐标范围显示一副图片
//* Input          : - StartX: Row Start Coordinate
//*                  - StartY: Line Start Coordinate
//*				   - EndX: Row End Coordinate
//*				   - EndY: Line End Coordinate
//* Output         : None
//* Return         : None
//* Attention		 : 图片取模格式为水平扫描，16位颜色模式
//*******************************************************************************/
//void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic)
//{
//  uint16_t  i;
//  LCD_SetCursor(StartX,StartY);
//  LCD_WriteRAM_Prepare();
//  for (i=0;i<(EndX*EndY);i++)
//  {
//      LCD_WriteRAM(*pic++);
//  }
//}
//
//
///******************************************************************************
//* Function Name  : LCD_DrawLine
//* Description    : 画一条直线
//* Input          : - x1: 行座标开始
//*                  - y1: 列座标开始
//*				   - x2: 行座标结束
//*				   - y2: 列座标结束
//*				   - bkColor: 背景颜色
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void LCD_DrawLine(int x1, int y1, int x2, int y2,uint16_t bkColor)
//{
//  int x,y,dx,dy,Dx,Dy,e,i;
//  Dx=x2-x1;
//  Dy=y2-y1;
//
//  dx=fabs(x2-x1);
//  dy=fabs(y2-y1);
//  x=x1;
//  y=y1;
//  if(dy>dx)
//  {
//    e=-dy;
//    for(i=0;i<dy;i++)
//    {
//      LCD_SetPoint(x,y,bkColor);
//      if(Dy>=0) y++;
//      else y--;
//      e+=2*dx;
//      if(e>=0)
//      {
//        if(Dx>=0) x++;
//        else x--;
//        e-=2*dy;
//      }
//    }
//  }
//  else
//  {
//    e=-dx;
//    for(i=0;i<dx;i++)
//    {
//      LCD_SetPoint(x,y,bkColor);
//      if(Dx>=0) x++;
//      else x--;
//      e+=2*dy;
//      if(e>=0)
//      {
//        if(Dy>=0) y++;
//        else y--;
//        e-=2*dx;
//      }
//    }
//  }
//}
//
//
//#if ASCII_LIB > 0
///******************************************************************************
//* Function Name  : PutChar
//* Description    : 将Lcd屏上任意位置显示一个字符
//* Input          : - Xpos: 水平坐标
//*                  - Ypos: 垂直坐标
//*				   - c: 显示的字符
//*				   - charColor: 字符颜色
//*				   - bkColor: 背景颜色
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void PutChar(unsigned short Xpos,unsigned short Ypos,unsigned char c,unsigned short charColor,unsigned short bkColor)
//{
//  unsigned short i=0;
//  unsigned short j=0;
//  unsigned char buffer[16];
//  unsigned char tmp_char=0;
//  GetASCIICode(buffer,c);  /* 取字模数据 */
//  for (i=0;i<16;i++)
//  {
//    tmp_char=buffer[i];
//    for (j=0;j<8;j++)
//    {
//      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
//        {
//          LCD_SetPoint(Xpos+j,Ypos+i,charColor);  /* 字符颜色 */
//        }
//        else
//        {
//          LCD_SetPoint(Xpos+j,Ypos+i,bkColor);  /* 背景颜色 */
//        }
//    }
//  }
//}
//
///******************************************************************************
//* Function Name  : GUI_Text
//* Description    : 在指定座标显示字符串
//* Input          : - Xpos: 行座标
//*                  - Ypos: 列座标
//*				   - str: 字符串
//*				   - charColor: 字符颜色
//*				   - bkColor: 背景颜色
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
//{
//uint8_t TempChar;
// do
//  {
//    TempChar=*str++;
//    PutChar(Xpos,Ypos,TempChar,Color,bkColor);
//    if (Xpos<232)
//    {
//      Xpos+=8;
//    }
//    else if (Ypos<304)
//    {
//      Xpos=0;
//      Ypos+=16;
//    }
//    else
//    {
//      Xpos=0;
//      Ypos=0;
//    }
//  }
//  while (*str!=0);
//}
//
//#endif
//
//#if HZ_LIB > 0
///******************************************************************************
//* Function Name  : PutChinese
//* Description    : 将Lcd屏上任意位置显示一个中文字
//* Input          : - Xpos: 水平坐标
//*                  - Ypos: 垂直坐标
//*				   - str: 显示的中文字
//*				   - Color: 字符颜色
//*				   - bkColor: 背景颜色
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor)
//{
//  uint8_t i,j;
//  uint8_t buffer[32];
//  uint16_t tmp_char=0;
//
//  GetGBKCode(buffer,str);  /* 取字模数据 */
//
//  for (i=0;i<16;i++)
//  {
//    tmp_char=buffer[i*2];
//	tmp_char=(tmp_char<<8);
//	tmp_char|=buffer[2*i+1];
//    for (j=0;j<16;j++)
//    {
//      if ( (tmp_char >> 15-j) & 0x01 == 0x01)
//        {
//          LCD_SetPoint(Xpos+j,Ypos+i,Color);  /* 字符颜色 */
//        }
//        else
//        {
//          LCD_SetPoint(Xpos+j,Ypos+i,bkColor);  /* 背景颜色 */
//        }
//    }
//  }
//}
//
///******************************************************************************
//* Function Name  : GUI_Chinese
//* Description    : 在指定座标显示字符串
//* Input          : - Xpos: 行座标
//*                  - Ypos: 列座标
//*				   - str: 字符串
//*				   - charColor: 字符颜色
//*				   - bkColor: 背景颜色
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
//{
//  do
//  {
//    PutChinese(Xpos,Ypos,str++,Color,bkColor);
//	str++;
//   if (Xpos<224)
//    {
//      Xpos+=16;
//    }
//    else if (Ypos<304)
//    {
//      Xpos=0;
//      Ypos+=16;
//    }
//    else
//    {
//      Xpos=0;
//      Ypos=0;
//    }
//  }
//  while(*str!=0);
//}
//
//#endif
//
///******************************************************************************
//* Function Name  : LCD_BGR2RGB
//* Description    : RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
//* Input          : - color: BRG 颜色值
//* Output         : None
//* Return         : RGB 颜色值
//* Attention		 : 内部函数调用
//*******************************************************************************/
//uint16_t LCD_BGR2RGB(uint16_t color)
//{
//  uint16_t  r, g, b, rgb;
//
//  b = ( color>>0 )  & 0x1f;
//  g = ( color>>5 )  & 0x3f;
//  r = ( color>>11 ) & 0x1f;
//
//  rgb =  (b<<11) + (g<<5) + (r<<0);
//
//  return( rgb );
//}
//
///******************************************************************************
//* Function Name  : LCD_BackLight_Init
//* Description    : LCD_BackLight Initializtion
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//void LCD_BackLight_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//
//  /*GPIOB Configuration:  PB5(TIM3 CH2) as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);	 /* 管脚重引射 */
//
//  /* -----------------------------------------------------------------------
//    TIM3CLK = 36 MHz, Prescaler = 35, TIM3 counter clock = 1 MHz
//    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
//    TIM3 Frequency = 1 KHz.
//    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100
//  ----------------------------------------------------------------------- */
//   /* 频率为 1MHz TIM3 counter clock = 1MHz */
//  TimerPeriod = (uint16_t) (SystemCoreClock / 1000000) - 1;
//
//  /* 输出频率=时钟/(ARR+1)，所以将输出一个 1Mhz/(999 + 1 )=1kHz 频率	 */
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;  /* 使用系统基础时钟 */
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//  TIM_TimeBaseStructure.TIM_Prescaler = TimerPeriod;  /* 时钟预分频 */
//  TIM_TimeBaseStructure.TIM_Period = 999;   /* TIM3 ARR Register */
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//
//  /* TIM_Pulse用来控制占空比，他实际影响TIM的CCR2寄存器，程序中可随时更改该寄存器的值，可随时更改占空比。占空比=(CCRx/ARR)*100。*/
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* 输出模式 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; /* 占空比参数 */
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//
//  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);    /* 使能TIM3在CCR2的预装储存器 */
//
//  TIM_ARRPreloadConfig(TIM3, ENABLE);  /* 使能 ARR装载 */
//  TIM_Cmd(TIM3, ENABLE);			   /* 使能TIM3 */
//
//}
///******************************************************************************
//* Function Name  : LCD_BackLight
//* Description    : 调整液晶背光
//* Input          : - percent: 背光亮度百分比
//* Output         : None
//* Return         : 返回1成功 返回0失败
//* Attention		 : None
//*******************************************************************************/
//FunctionalState LCD_BackLight( uint8_t percent)
//{
//
//  if( percent <= 100)
//  {
//    Channel2Pulse=percent*10;
//	LCD_BackLight_Init();
//	return ENABLE;
//  }
//  else
//    return DISABLE;
//}
//
///*********************************************************************************************************
//      END FILE
//*********************************************************************************************************/

