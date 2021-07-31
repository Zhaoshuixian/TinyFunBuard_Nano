
#include "w25qxx.h" 
#include "spi.h"
#include "delay.h"
#include "usart.h"

unsigned short W25QXX_TYPE=W25Q128;	    //�������õ��ⲿFlash��W25Q128��һ����16M�ֽڣ�
								                        //128������(һ��������16������)��
									                      //4096������(һ��������4K�ֽ�)									 
unsigned char W25QXX_BUFFER[4096];      //дW25Q128ʱ�õ������ݴ����м�Ĵ���

/*********************W25QXXƬѡ�ˣ��͵�ƽ��Ч ���Ƭѡ����Ҫ���ݾ����·�޸ģ���������PA4����***************************/
void W25Q_CS(unsigned char date)               
{	 
	if(date) GPIO_SetBits(GPIOA,GPIO_Pin_4);
	else     GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

/********************************************************************************************************			                                                           
*  ����: 	 ��ȡоƬID
*  �������: ��
*  ����ֵ��  оƬID��W25Q08-0XEF13��W25Q16-0XEF14��W25Q32-0XEF15��W25Q64-0XEF16��W25Q128-0XEF17��
********************************************************************************************************/
unsigned short int W25QXX_ReadID(void)
{
	unsigned short int id = 0;	  
	
	W25Q_CS(0);				    
	
	SPI1_ReadWriteByte(0x90);                //���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	
	id|=SPI1_ReadWriteByte(0xFF)<<8;  
	id|=SPI1_ReadWriteByte(0xFF);	 
	
	W25Q_CS(1); 				    
	
	return id;
}   

/********************************************************************************************************			                                                           
*  ����: 	 ��ʼ��SPI FLASH��IO��
*  �������: num����Ҫ��ʼ����SPI��
*  ����ֵ��  ��
*  ע����������оƬ��ͨ��SPI��ʽ��W25Q128����ͨ�ŵģ���������ʵ�����Ƕ�SPIx�ĳ�ʼ�����������ѡ�õ�SPI�ڽ��ж�Ӧ�ĳ�ʼ��
********************************************************************************************************/
void W25QXX_Init(unsigned char num)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTAʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // PA4 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);
 
	W25Q_CS(1);				//SPI FLASH��ѡ��
	
	/*ʹ��SPI1*/
	if(num == 1)
	{
		SPI1_Init();		   	//��ʼ��SPI
		SPI1_SetSpeed(SPI_BaudRatePrescaler_4);//����Ϊ18Mʱ��,����ģʽ��SPI1�ҿ���APB12�ϣ�APB2ʱ��Ϊ72M,��������Ҫ4��Ƶ
	}
	
	/*ʹ��SPI2*/
	else if(num == 2)
	{
		SPI2_Init();		   	//��ʼ��SPI
		SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//����Ϊ18Mʱ��,����ģʽ��SPI2�ҿ���APB1�ϣ�APB1ʱ��Ϊ36M,��������Ҫ2��Ƶ
	}
	
	W25QXX_TYPE=W25QXX_ReadID();//��ȡ�ⲿFLASH ID.  

}  

/********************************************************************************************************			                                                           
*  ����: 	 ��ȡW25QXX��״̬�Ĵ���
*  �������: num����W25QXX������SPI��
*  ����ֵ��  W25QXX��״ֵ̬��SPR RV TB BP2 BP1 BP0 WEL BUSY��SPR��              Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
															 TB��BP2��BP1��BP0��FLASH����д��������
															 WEL��              дʹ������
															 BUSY��             æ���λ(1,æ;0,����)
********************************************************************************************************/
unsigned char W25QXX_ReadSR(unsigned char num)   
{  
	unsigned char byte=0;   
	
	W25Q_CS(0);                            //ʹ������   
	
	if(num==1)                          //SPI1
	{
		SPI1_ReadWriteByte(W25X_ReadStatusReg); //���Ͷ�ȡ״̬�Ĵ�������    
		byte=SPI1_ReadWriteByte(0Xff);          //��ȡһ���ֽ�  
	}
	else if(num==2)                     //SPI2
	{
		SPI2_ReadWriteByte(W25X_ReadStatusReg);  //���Ͷ�ȡ״̬�Ĵ�������    
		byte=SPI2_ReadWriteByte(0Xff);          //��ȡһ���ֽ�
	}
	
	W25Q_CS(1);                            //ȡ��Ƭѡ     
	
	return byte;   
} 

/********************************************************************************************************			                                                           
*  ����: 	 дW25QXX״̬�Ĵ���
*  �������: sr����д���ֽ�
			 num����W25QXX������SPI��
*  ����ֵ��  ��
*  ע��ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
********************************************************************************************************/
void W25QXX_Write_SR(unsigned char sr,unsigned char num)   
{   
	W25Q_CS(0);                            //ʹ������   
	
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_WriteStatusReg);//����д״̬�Ĵ�������    
		SPI1_ReadWriteByte(sr);               	//д��һ���ֽ�
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_WriteStatusReg);//����д״̬�Ĵ�������    
		SPI2_ReadWriteByte(sr);               	//д��һ���ֽ�
	}
	
	W25Q_CS(1);                           //ȡ��Ƭѡ     	      
}   

/********************************************************************************************************			                                                           
*  ����: 	 W25QXXдʹ�ܡ���WEL��λ��
*  �������: num����W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Write_Enable(unsigned char num)   
{
	W25Q_CS(0);                          	//ʹ������   
    
	if(num==1)
		SPI1_ReadWriteByte(W25X_WriteEnable); 	//����дʹ��
	else if(num==2)
		SPI2_ReadWriteByte(W25X_WriteEnable); 	//����дʹ��
	
	W25Q_CS(1);                           	//ȡ��Ƭѡ     	      
} 

/********************************************************************************************************			                                                           
*  ����: 	 W25QXXд��������WEL���㡿
*  �������: num����W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Write_Disable(unsigned char num)   
{  
	W25Q_CS(0);                            //ʹ������   
    
	if(num==1) SPI1_ReadWriteByte(W25X_WriteDisable);  //����д��ָֹ�� 
	if(num==2) SPI2_ReadWriteByte(W25X_WriteDisable);  //����д��ָֹ��
	
	W25Q_CS(1);                            //ȡ��Ƭѡ     	      
} 		
  		    
/********************************************************************************************************			                                                           
*  ����: 	 ��ȡSPI FLASH����ָ����ַ��ʼ��ȡָ�����ȵ����ݡ�
*  �������: pBuffer��      ���ݴ洢��
			 ReadAddr��     ��ʼ��ȡ�ĵ�ַ(24bit)
		     NumByteToRead��Ҫ��ȡ���ֽ���(���65535)
			 num��          ��W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Read(unsigned char* pBuffer,unsigned int ReadAddr,unsigned short int NumByteToRead,unsigned char num)   
{ 
 	unsigned short int i;   										    
	
	W25Q_CS(0);                            	//ʹ������   
    
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_ReadData);         	//���Ͷ�ȡ����   
		SPI1_ReadWriteByte((unsigned char)((ReadAddr)>>16));  	//����24bit��ַ    
		SPI1_ReadWriteByte((unsigned char)((ReadAddr)>>8));   
		SPI1_ReadWriteByte((unsigned char)ReadAddr);   
		for(i=0;i<NumByteToRead;i++)
		{ 
			pBuffer[i]=SPI1_ReadWriteByte(0XFF);   	//ѭ������  
		}
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_ReadData);         	//���Ͷ�ȡ����   
		SPI2_ReadWriteByte((unsigned char)((ReadAddr)>>16));  	//����24bit��ַ    
		SPI2_ReadWriteByte((unsigned char)((ReadAddr)>>8));   
		SPI2_ReadWriteByte((unsigned char)ReadAddr);   
		for(i=0;i<NumByteToRead;i++)
		{ 
			pBuffer[i]=SPI2_ReadWriteByte(0XFF);   	//ѭ������  
		}
	}
	
	W25Q_CS(1);  				    	      
}  

/********************************************************************************************************			                                                           
*  ����: 	 ��һҳ��д��SPI FLASH����ָ����ַ��ʼд�����256�ֽڵ����ݡ�
*  �������: pBuffer��      ���ݴ洢��
			 WriteAddr��    ��ʼд��ĵ�ַ(24bit)
		     NumByteToRead��Ҫд����ֽ���(���256)��������Ӧ�ó�����ҳ��ʣ���ֽ�����
			 num��          ��W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Write_Page(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short int NumByteToWrite, unsigned char num)
{
 	unsigned short int i;  
    
	W25QXX_Write_Enable(spi_num);           //SET WEL 
	
	W25Q_CS(0);                           	//ʹ������   
    
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_PageProgram);      	//����дҳ����   
		SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>16)); 	//����24bit��ַ    
		SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>8));   
		SPI1_ReadWriteByte((unsigned char)WriteAddr);   
		
		for(i=0;i<NumByteToWrite;i++)
			SPI1_ReadWriteByte(pBuffer[i]);//ѭ��д�� 
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_PageProgram);      	//����дҳ����   
		SPI2_ReadWriteByte((unsigned char)((WriteAddr)>>16)); 	//����24bit��ַ    
		SPI2_ReadWriteByte((unsigned char)((WriteAddr)>>8));   
		SPI2_ReadWriteByte((unsigned char)WriteAddr);   
		
		for(i=0;i<NumByteToWrite;i++)
			SPI2_ReadWriteByte(pBuffer[i]);//ѭ��д�� 
	}
	
	W25Q_CS(1);                           	//ȡ��Ƭѡ 
	
	W25QXX_Wait_Busy();					   		//�ȴ�д�����
} 

/********************************************************************************************************			                                                           
*  ����: 	 �޼���дSPI FLASH����ָ����ַ��ʼд��ָ�����ȵ����ݣ�����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ�ܣ�д����ʱ�Զ���ҳ��
*  �������: pBuffer��       ���ݴ洢��
			 WriteAddr��     ��ʼд��ĵ�ַ(24bit)
		     NumByteToWrite��Ҫд����ֽ���(���256)��������Ӧ�ó�����ҳ��ʣ���ֽ�����
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Write_NoCheck(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short int NumByteToWrite)   
{ 			 		 
	unsigned short int pageremain;	   
	
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	
	if(NumByteToWrite<=pageremain) pageremain=NumByteToWrite;//������256���ֽ�
		
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain,spi_num);
		
		if(NumByteToWrite==pageremain) break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256) pageremain=256; //һ�ο���д��256���ֽ�
			else   pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	}	    
}

/********************************************************************************************************			                                                           
*  ����: 	 дSPI FLASH����ָ����ַ��ʼд��ָ�����ȵ����ݣ������ȼ����д��ռ��Ƿ�Ϊ0xff���������Ȳ�����д�롿
*  �������: pBuffer��       ���ݴ洢��
			 WriteAddr��     ��ʼд��ĵ�ַ(24bit)
		     NumByteToWrite��Ҫд����ֽ���(���65535) 
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Write(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short int NumByteToWrite)   
{ 
	unsigned int secpos;
	unsigned short int secoff;
	unsigned short int secremain;	   
 	unsigned short int i;      
   
	unsigned char* W25QXX_BUF;

	W25QXX_BUF=W25QXX_BUFFER;
 	
	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	
	if(NumByteToWrite<=secremain) secremain=NumByteToWrite;//������4096���ֽ�
		
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096,spi_num);//������������������
		
		for(i=0;i<secremain;i++)//У������
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)	break;//��Ҫ����  
		}
		if(i<secremain)//��Ҫ����
		{
			W25QXX_Erase_Sector(secpos,spi_num);		//�����������
			for(i=0;i<secremain;i++)	   		//����
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//д����������  

		}
		else 
		{
			W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������.
		}			
		
		if(NumByteToWrite==secremain) break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  				//ָ��ƫ��
			WriteAddr+=secremain;				//д��ַƫ��	   
		   	NumByteToWrite-=secremain;			//�ֽ����ݼ�
			
			if(NumByteToWrite>4096)
				secremain=4096;//��һ����������д����
			else
				secremain=NumByteToWrite;		//��һ����������д����
		}	 
	}	 
}

/********************************************************************************************************			                                                           
*  ����: 	 ��������оƬ������ʱ���Ƚϳ���
*  �������: num����W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Erase_Chip(unsigned char num)   
{                                   
    W25QXX_Write_Enable(spi_num);                 	 	//SET WEL 
    W25QXX_Wait_Busy();   
  	
	W25Q_CS(0);                            	//ʹ������   
    
	if(num==1) SPI1_ReadWriteByte(W25X_ChipErase);        	//����Ƭ�������� 
	if(num==2) SPI2_ReadWriteByte(W25X_ChipErase);        	//����Ƭ�������� 
	
	W25Q_CS(1);                            	//ȡ��Ƭѡ     	      
	
	W25QXX_Wait_Busy();   				   		//�ȴ�оƬ��������
}  

/********************************************************************************************************			                                                           
*  ����: 	 ����һ������������ʱ������Ҫ150ms��
*  �������: Dst_Addr��������ַ������ʵ���������á�
			 num��     ��W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Erase_Sector(unsigned int Dst_Addr,unsigned char num)   
{  
	//����falsh�������,������   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	
	Dst_Addr*=4096;
    W25QXX_Write_Enable(spi_num);                  	       //SET WEL 	 
    W25QXX_Wait_Busy();   
  	
	W25Q_CS(0);                            	         //ʹ������   
    
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_SectorErase);      	    //������������ָ�� 
		SPI1_ReadWriteByte((unsigned char)((Dst_Addr)>>16));  	   //����24bit��ַ    
		SPI1_ReadWriteByte((unsigned char)((Dst_Addr)>>8));   
		SPI1_ReadWriteByte((unsigned char)Dst_Addr); 
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_SectorErase);      	    //������������ָ�� 
		SPI2_ReadWriteByte((unsigned char)((Dst_Addr)>>16));  	   //����24bit��ַ    
		SPI2_ReadWriteByte((unsigned char)((Dst_Addr)>>8));   
		SPI2_ReadWriteByte((unsigned char)Dst_Addr); 
	}
	
	W25Q_CS(1);                            	     //ȡ��Ƭѡ     	      
    W25QXX_Wait_Busy();   				   		//�ȴ��������
} 

/********************************************************************************************************			                                                           
*  ����: 	 �ȴ�оƬ��ɲ���
*  �������: ��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(spi_num)&0x01)==0x01);  		// �ȴ�BUSYλ���
}

/********************************************************************************************************			                                                           
*  ����: 	 �������ģʽ
*  �������: num����W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_PowerDown(unsigned char num)   
{ 
  	W25Q_CS(0);                           	 	//ʹ������   
    
	if(num==1)
		SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������
	else if(num==2)
		SPI1_ReadWriteByte(W25X_PowerDown);        //���͵�������
	
	W25Q_CS(1);                           	  //ȡ��Ƭѡ     	      
    xDelay_us(3);                              //�ȴ�TPD  
}   

/********************************************************************************************************			                                                           
*  ����: 	 ����оƬ
*  �������: num����W25QXX������SPI��
*  ����ֵ��  ��
********************************************************************************************************/
void W25QXX_WAKEUP(unsigned char num)   
{  
  	W25Q_CS(0);                            	//ʹ������   
    
	if(num==1) SPI1_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB 
	if(num==2) SPI2_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB
	
	W25Q_CS(1);                            	//ȡ��Ƭѡ     	      
  xDelay_us(3);                            	//�ȴ�TRES1
}   

