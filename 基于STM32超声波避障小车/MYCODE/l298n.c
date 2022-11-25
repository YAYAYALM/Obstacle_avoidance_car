#include "l298n.h"


/*****************************************
引脚说明：

LED0连接在PF9,低电平灯亮；高电平，灯灭
TIM2_CH3(TIM2 -- APB1 32位  84MHZ)
TIM2_CH4(TIM2 -- APB1 32位  84MHZ)

*****************************************/


void L298n_Ctrl1_Init(void)
{
	TIM_OCInitTypeDef			TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseInitStruct;
	
	//GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//（2）使能定时器2和相关IO口时钟。
	//使能定时器4时钟：
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//使能GPIOA时钟：
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF;		//复用模式
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	//输出推挽
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_25MHz; //速度
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//上拉
	
	//（3）初始化IO口为复用功能输出。
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_14;		//引脚2	
	GPIO_Init(GPIOD, &GPIO_InitStruct);	
	
	//（3）初始化IO口为复用功能输出。
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_15;		//引脚3	
	GPIO_Init(GPIOD, &GPIO_InitStruct);	

	//（4）GPIOA2/3复用映射到定时器2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4); 
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (84-1); 	//84分频 84MHZ/84 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);	//ARR   计1000,在1MHZ，PWM周期1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //分频因子
	//2、初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode		= TIM_OCMode_PWM1;	//PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity	= TIM_OCPolarity_High;//输出极性电平 ，这里选择高电平
	TIM_OCInitStruct.TIM_Pulse 		= 0; //CCR1 = 0;
	TIM_OCInitStruct.TIM_OutputState= TIM_OutputState_Enable; //通道使能
	//（6）初始化输出比较参数 OC3 -- 通道3	OC4 -- 通道4
	TIM_OC3Init(TIM4, &TIM_OCInitStruct);
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);
	
	//（7）使能预装载寄存器  OC3 -- 通道3	OC4 -- 通道4
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	//（8）使能自动重装载的预装载寄存器允许位	
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	//（9）使能定时器。
	TIM_Cmd(TIM4, ENABLE);
}


void L298n_Ctrl2_Init(void)
{
	TIM_OCInitTypeDef					TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseInitStruct;
	
	//GPIO结构体
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//（2）使能定时器2和相关IO口时钟。
	//使能定时器2时钟：
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//使能GPIOA时钟：
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF;		//复用模式
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	//输出推挽
	GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_25MHz; //速度
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//上拉
	
	//（3）初始化IO口为复用功能输出。
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13;		//引脚2	
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
	
	//（3）初始化IO口为复用功能输出。
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_14;		//引脚3	
	GPIO_Init(GPIOE, &GPIO_InitStruct);	

	//（4）GPIOA2/3复用映射到定时器2
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 
	
	TIM_TimeBaseInitStruct.TIM_Prescaler	= (168-1); 	//168分频 168MHZ/168 = 1MHZ
	TIM_TimeBaseInitStruct.TIM_Period		= (1000-1);	//ARR   计1000,在1MHZ，PWM周期1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode	= TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision= TIM_CKD_DIV1; //分频因子
	//2、初始化定时器，配置ARR,PSC。
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode		= TIM_OCMode_PWM2;	//PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity	= TIM_OCPolarity_Low;//输出极性电平 ，这里选择高电平
	TIM_OCInitStruct.TIM_Pulse 		= 0; //CCR1 = 0;
	TIM_OCInitStruct.TIM_OutputState= TIM_OutputState_Enable; //通道使能
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//（6）初始化输出比较参数 OC3 -- 通道3	OC4 -- 通道4
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	//（7）使能预装载寄存器  OC3 -- 通道3	OC4 -- 通道4
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	//（8）使能自动重装载的预装载寄存器允许位	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	//MOE 主输出使能,高级定时器必须开启这个
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	//（9）使能定时器。
	TIM_Cmd(TIM1, ENABLE);
}


void Car_Go(void)
{
	//左轮
	TIM_SetCompare4(TIM1, 0);
	TIM_SetCompare3(TIM1, 300);
	//右轮
	TIM_SetCompare4(TIM4, 0);
	TIM_SetCompare3(TIM4, 300);
}

void Car_Stop(void)
{
	//左轮
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 0);
	//右轮
	TIM_SetCompare3(TIM4, 0);
	TIM_SetCompare4(TIM4, 0);
}

void Car_Back(void)
{
	//左轮
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 300);
	//右轮
	TIM_SetCompare3(TIM4, 0);
	TIM_SetCompare4(TIM4, 300);
}

void Car_Left(void)
{
	//左轮
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 300);
	//右轮
	TIM_SetCompare4(TIM4, 0);
	TIM_SetCompare3(TIM4, 300);
}

void Car_BigLeft(void)
{
	//左轮
	TIM_SetCompare3(TIM1, 0);
	TIM_SetCompare4(TIM1, 400);
	//右轮
	TIM_SetCompare4(TIM4, 0);
	TIM_SetCompare3(TIM4, 400);
}

void Car_Right(void)
{
	//左轮
	TIM_SetCompare4(TIM1, 0);
	TIM_SetCompare3(TIM1, 300);
	//右轮
	TIM_SetCompare3(TIM4, 0);
	TIM_SetCompare4(TIM4, 300);
	
}

void Car_BigRight(void)
{
	//左轮
	TIM_SetCompare4(TIM1, 0);
	TIM_SetCompare3(TIM1, 400);
	//右轮
	TIM_SetCompare3(TIM4, 0);
	TIM_SetCompare4(TIM4, 400);
	
}
