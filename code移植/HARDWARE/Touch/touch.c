#include "touch.h"
#include "gt9147.h"
#include "usart.h"
#include "delay.h"
#include "ILI93xx.h"
_m_tp_dev tp_dev=
{
	TP_Init,
	NULL,//TP_Scan,
	NULL,//TP_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};

//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
u8 TP_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(lcddev.id==0X5510)				//4.3����ݴ�����
	{
		if(GT9147_Init()==0)			//��GT9147
		{ 
			tp_dev.scan=GT9147_Scan;	//ɨ�躯��ָ��GT9147������ɨ��
		}
		tp_dev.touchtype|=0X80;			//������ 
		tp_dev.touchtype|=lcddev.dir&0X01;//������������ 
		return 0;
	}
	  

		//ע��,ʱ��ʹ��֮��,��GPIO�Ĳ�������Ч
		//��������֮ǰ,����ʹ��ʱ��.����ʵ��������������
    	 	
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PB,PF�˿�ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PF�˿�ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PF�˿�ʱ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // PB1�˿�����
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 	GPIO_Init(GPIOA, &GPIO_InitStructure);//B1�������
	 	GPIO_SetBits(GPIOA,GPIO_Pin_5);//����
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // PB2�˿�����
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	 	GPIO_Init(GPIOA, &GPIO_InitStructure);//B2��������
	 	GPIO_SetBits(GPIOA,GPIO_Pin_6);//����		
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 // F9��PF11�˿�����
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 	GPIO_Init(GPIOA, &GPIO_InitStructure);//PF9,PF11�������
	 	GPIO_SetBits(GPIOA, GPIO_Pin_7);//����
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // F9��PF11�˿�����
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 	GPIO_Init(GPIOC, &GPIO_InitStructure);//PF9,PF11�������
	 	GPIO_SetBits(GPIOC, GPIO_Pin_4);//����
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // PF10�˿�����
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	 	GPIO_Init(GPIOB, &GPIO_InitStructure);//PF10��������
	 	GPIO_SetBits(GPIOB,GPIO_Pin_6);//����			
		
//		TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//��һ�ζ�ȡ��ʼ��	 
//		//AT24CXX_Init();			//��ʼ��24CXX
//		if(TP_Get_Adjdata())return 0;//�Ѿ�У׼
//		else			  		//δУ׼?
//		{ 										    
//			LCD_Clear(WHITE);	//����
//			TP_Adjust();  		//��ĻУ׼  
//		}			
//		TP_Get_Adjdata();	
//	
	return 1; 									 
}
