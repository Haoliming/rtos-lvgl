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

//触摸屏初始化  		    
//返回值:0,没有进行校准
//       1,进行过校准
u8 TP_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(lcddev.id==0X5510)				//4.3寸电容触摸屏
	{
		if(GT9147_Init()==0)			//是GT9147
		{ 
			tp_dev.scan=GT9147_Scan;	//扫描函数指向GT9147触摸屏扫描
		}
		tp_dev.touchtype|=0X80;			//电容屏 
		tp_dev.touchtype|=lcddev.dir&0X01;//横屏还是竖屏 
		return 0;
	}
	  

		//注意,时钟使能之后,对GPIO的操作才有效
		//所以上拉之前,必须使能时钟.才能实现真正的上拉输
    	 	
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PF端口时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PF端口时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PF端口时钟
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // PB1端口配置
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 	GPIO_Init(GPIOA, &GPIO_InitStructure);//B1推挽输出
	 	GPIO_SetBits(GPIOA,GPIO_Pin_5);//上拉
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // PB2端口配置
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	 	GPIO_Init(GPIOA, &GPIO_InitStructure);//B2上拉输入
	 	GPIO_SetBits(GPIOA,GPIO_Pin_6);//上拉		
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 // F9，PF11端口配置
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 	GPIO_Init(GPIOA, &GPIO_InitStructure);//PF9,PF11推挽输出
	 	GPIO_SetBits(GPIOA, GPIO_Pin_7);//上拉
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // F9，PF11端口配置
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 	GPIO_Init(GPIOC, &GPIO_InitStructure);//PF9,PF11推挽输出
	 	GPIO_SetBits(GPIOC, GPIO_Pin_4);//上拉
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 // PF10端口配置
	 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	 	GPIO_Init(GPIOB, &GPIO_InitStructure);//PF10上拉输入
	 	GPIO_SetBits(GPIOB,GPIO_Pin_6);//上拉			
		
//		TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//第一次读取初始化	 
//		//AT24CXX_Init();			//初始化24CXX
//		if(TP_Get_Adjdata())return 0;//已经校准
//		else			  		//未校准?
//		{ 										    
//			LCD_Clear(WHITE);	//清屏
//			TP_Adjust();  		//屏幕校准  
//		}			
//		TP_Get_Adjdata();	
//	
	return 1; 									 
}
