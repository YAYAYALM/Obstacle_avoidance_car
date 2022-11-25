#include "key.h"

/*****************************************
����˵����

KEY0������PA0
����δ���£�PA0Ϊ�ߵ�ƽ
���� ���£�PA0Ϊ�͵�ƽ
KEY1������PE2
����δ���£�PA0Ϊ�ߵ�ƽ
���� ���£�PA0Ϊ�͵�ƽ
KEY2������PE3
����δ���£�PA0Ϊ�ߵ�ƽ
���� ���£�PA0Ϊ�͵�ƽ
KEY3������PE4
����δ���£�PA0Ϊ�ߵ�ƽ
���� ���£�PA0Ϊ�͵�ƽ
*****************************************/

void Key_Init(void)
{
	//GPIO�ṹ��
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_IN;		//����ģʽ
	GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;		//����
	
	//��GPIOA��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//��GPIOE��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//��ʼ��GPIOA������0���Ĵ���
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_0;		//����0
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//��ʼ��GPIOE������2���Ĵ���
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_2;		//����2
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//��ʼ��GPIOE������3���Ĵ���
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_3;		//����3
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//��ʼ��GPIOE������4���Ĵ���
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_4;		//����4
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}
