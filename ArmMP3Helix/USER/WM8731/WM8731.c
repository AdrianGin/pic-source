/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			WM8731.C
** Descriptions:		WM8731函数
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2010-11-17
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "WM8731.h"
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_10	 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOB->BRR  = GPIO_Pin_10  /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOB->BSRR = GPIO_Pin_11	 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOB->BRR  = GPIO_Pin_11	 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read      GPIOB->IDR  & GPIO_Pin_10	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIOB->IDR  & GPIO_Pin_11	 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */


/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : EEPROM管脚配置
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  /* Configure I2C2 pins: PB10->SCL and PB11->SDA */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*******************************************************************************
* Function Name  : I2C_delay
* Description    : 延迟时间
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_delay(void)
{	
   uint8_t i=5; /* 这里可以优化速度,经测试最低到5还能写入 */
   while(i) 
   { 
     i--; 
   } 
}

/*******************************************************************************
* Function Name  : I2C_Start
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static FunctionalState I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return DISABLE;	/* SDA线为低电平则总线忙,退出 */
	SDA_L;
	I2C_delay();
	if(SDA_read) return DISABLE;	/* SDA线为高电平则总线出错,退出 */
	SDA_L;
	I2C_delay();
	return ENABLE;
}

/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : None
* Input          : None
* Output         : None
* Return         : 返回为:=1有ACK,=0无ACK
* Attention		 : None
*******************************************************************************/
static FunctionalState I2C_WaitAck(void) 	
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

 /*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : 数据从高位到低位
* Input          : - SendByte: 发送的数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}

/*******************************************************************************
* Function Name  : WM8731_Send
* Description    : 写数据
* Input          : - SendByte: 待写入数据
*           	   - WriteAddress: 待写入地址
*                  - DeviceAddress: 器件类型
* Output         : None
* Return         : 返回为:=1成功写入,=0失败
* Attention		 : None
*******************************************************************************/           
static FunctionalState WM8731_Send(uint8_t WriteAddress, uint8_t SendByte , uint8_t DeviceAddress)
{		
    if(!I2C_Start())return DISABLE;
    I2C_SendByte(DeviceAddress & 0xFF);        /* 设置器件地址 */ 
    if(!I2C_WaitAck()){I2C_Stop(); return DISABLE;}
    I2C_SendByte( WriteAddress );     
    I2C_WaitAck();	
    I2C_SendByte(SendByte);		    
    I2C_WaitAck();   
    I2C_Stop(); 
	/* 注意：因为这里要等待写完，可以采用查询或延时方式(10ms)	*/
    /* Systick_Delay_1ms(10); */
    return ENABLE;
}									 

/*******************************************************************************
* Function Name  : codec_send
* Description    : None
* Input          : - s_data: 待写入数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/   
void codec_send(uint16_t s_data)
{
  //while( !WM8731_Send( (s_data >> 8) & 0xFF ,s_data & 0xFF ,  ADDR_WM8731S) );
}

/*******************************************************************************
* Function Name  : WM8731_Init
* Description    : WM8731初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/   
void WM8731_Init(void)
{
   I2C_Configuration();
   codec_send( RESET_REGISTER ); /* reset */
   codec_send( LEFT_HEADPHONE_OUT  | (1<<8) | (1<<7) | (100<<0) ); /* headphone out	*/
   codec_send( RIGHT_HEADPHONE_OUT | (1<<8) | (1<<7) | (100<<0) );
   codec_send( ANALOGUE_AUDIO_PATH_CONTROL | DAC_SEL | MUTEMIC );
   codec_send( DIGITAL_AUDIO_PATH_CONTRL | (2<<1) ); /* 44.1kHz	*/
   codec_send( POWER_DOWN_COTROL | (0<<4) | (0<<3) | (1<<2) | (1<<1) | (1<<0));	/* powerup */
   codec_send( SAMPLING_CONTROL | NORMAL_MODE | BOSR_NORMAL_256FS );
   codec_send( ACTIVE_CONTROL | ACTIVE );
}

/*******************************************************************************
* Function Name  : WM8731_Vol
* Description    : WM8731调节音量
* Input          : - vol: 47-127  47静音 127最大声音
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/  
void WM8731_Vol(uint8_t vol)
{
   codec_send(LEFT_HEADPHONE_OUT  | (1<<8) | (1<<7) | (vol<<0));
   codec_send(RIGHT_HEADPHONE_OUT | (1<<8) | (1<<7) | (vol<<0));

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
