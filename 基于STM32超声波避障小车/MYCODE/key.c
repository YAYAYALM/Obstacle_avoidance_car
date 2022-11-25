#include "key.h"

/*****************************************
引脚说明：

KEY0连接在PA0
按键未按下，PA0为高电平
按键 按下，PA0为低电平
KEY1连接在PE2
按键未按下，PA0为高电平
按键 按下，PA0为低电平
KEY2连接在PE3
按键未按下，PA0为高电平
按键 按下，PA0为低电平
KEY3连接在PE4
按键未按下，PA0为高电平
按键 按下，PA0为低电平
*****************************************/

void Key_Init(void)
{
	//GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//输入模式
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//上拉
	
	//打开GPIOA组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//打开GPIOE组时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//初始化GPIOA（引脚0）寄存器
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_0;		//引脚0
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//初始化GPIOE（引脚2）寄存器
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_2;		//引脚2
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//初始化GPIOE（引脚3）寄存器
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_3;		//引脚3
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//初始化GPIOE（引脚4）寄存器
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_4;		//引脚4
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}
