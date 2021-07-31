
#include "w25qxx.h" 
#include "spi.h"
#include "delay.h"
#include "usart.h"

unsigned short W25QXX_TYPE=W25Q128;	    //开发板用的外部Flash是W25Q128，一共有16M字节，
								                        //128个块区(一个块区有16个扇区)，
									                      //4096个扇区(一个扇区有4K字节)									 
unsigned char W25QXX_BUFFER[4096];      //写W25Q128时用到的数据储存中间寄存器

/*********************W25QXX片选端，低电平有效 这个片选端需要根据具体电路修改，这里是用PA4控制***************************/
void W25Q_CS(unsigned char date)               
{	 
	if(date) GPIO_SetBits(GPIOA,GPIO_Pin_4);
	else     GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}

/********************************************************************************************************			                                                           
*  功能: 	 读取芯片ID
*  输入参数: 无
*  返回值：  芯片ID【W25Q08-0XEF13，W25Q16-0XEF14，W25Q32-0XEF15，W25Q64-0XEF16，W25Q128-0XEF17】
********************************************************************************************************/
unsigned short int W25QXX_ReadID(void)
{
	unsigned short int id = 0;	  
	
	W25Q_CS(0);				    
	
	SPI1_ReadWriteByte(0x90);                //发送读取ID命令	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	    
	SPI1_ReadWriteByte(0x00); 	 			   
	
	id|=SPI1_ReadWriteByte(0xFF)<<8;  
	id|=SPI1_ReadWriteByte(0xFF);	 
	
	W25Q_CS(1); 				    
	
	return id;
}   

/********************************************************************************************************			                                                           
*  功能: 	 初始化SPI FLASH的IO口
*  输入参数: num：需要初始化的SPI口
*  返回值：  无
*  注：这里由于芯片是通过SPI方式与W25Q128进行通信的，所以这里实际上是对SPIx的初始化，具体根据选用的SPI口进行对应的初始化
********************************************************************************************************/
void W25QXX_Init(unsigned char num)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTA时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  // PA4 推挽 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);
 
	W25Q_CS(1);				//SPI FLASH不选中
	
	/*使用SPI1*/
	if(num == 1)
	{
		SPI1_Init();		   	//初始化SPI
		SPI1_SetSpeed(SPI_BaudRatePrescaler_4);//设置为18M时钟,高速模式—SPI1挂靠在APB12上，APB2时钟为72M,所以这里要4分频
	}
	
	/*使用SPI2*/
	else if(num == 2)
	{
		SPI2_Init();		   	//初始化SPI
		SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//设置为18M时钟,高速模式—SPI2挂靠在APB1上，APB1时钟为36M,所以这里要2分频
	}
	
	W25QXX_TYPE=W25QXX_ReadID();//读取外部FLASH ID.  

}  

/********************************************************************************************************			                                                           
*  功能: 	 读取W25QXX的状态寄存器
*  输入参数: num：与W25QXX相连的SPI口
*  返回值：  W25QXX的状态值【SPR RV TB BP2 BP1 BP0 WEL BUSY】SPR：              默认0,状态寄存器保护位,配合WP使用
															 TB，BP2，BP1，BP0：FLASH区域写保护设置
															 WEL：              写使能锁定
															 BUSY：             忙标记位(1,忙;0,空闲)
********************************************************************************************************/
unsigned char W25QXX_ReadSR(unsigned char num)   
{  
	unsigned char byte=0;   
	
	W25Q_CS(0);                            //使能器件   
	
	if(num==1)                          //SPI1
	{
		SPI1_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器命令    
		byte=SPI1_ReadWriteByte(0Xff);          //读取一个字节  
	}
	else if(num==2)                     //SPI2
	{
		SPI2_ReadWriteByte(W25X_ReadStatusReg);  //发送读取状态寄存器命令    
		byte=SPI2_ReadWriteByte(0Xff);          //读取一个字节
	}
	
	W25Q_CS(1);                            //取消片选     
	
	return byte;   
} 

/********************************************************************************************************			                                                           
*  功能: 	 写W25QXX状态寄存器
*  输入参数: sr：待写入字节
			 num：与W25QXX相连的SPI口
*  返回值：  无
*  注：只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
********************************************************************************************************/
void W25QXX_Write_SR(unsigned char sr,unsigned char num)   
{   
	W25Q_CS(0);                            //使能器件   
	
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_WriteStatusReg);//发送写状态寄存器命令    
		SPI1_ReadWriteByte(sr);               	//写入一个字节
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_WriteStatusReg);//发送写状态寄存器命令    
		SPI2_ReadWriteByte(sr);               	//写入一个字节
	}
	
	W25Q_CS(1);                           //取消片选     	      
}   

/********************************************************************************************************			                                                           
*  功能: 	 W25QXX写使能【将WEL置位】
*  输入参数: num：与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_Write_Enable(unsigned char num)   
{
	W25Q_CS(0);                          	//使能器件   
    
	if(num==1)
		SPI1_ReadWriteByte(W25X_WriteEnable); 	//发送写使能
	else if(num==2)
		SPI2_ReadWriteByte(W25X_WriteEnable); 	//发送写使能
	
	W25Q_CS(1);                           	//取消片选     	      
} 

/********************************************************************************************************			                                                           
*  功能: 	 W25QXX写保护【将WEL清零】
*  输入参数: num：与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_Write_Disable(unsigned char num)   
{  
	W25Q_CS(0);                            //使能器件   
    
	if(num==1) SPI1_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令 
	if(num==2) SPI2_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令
	
	W25Q_CS(1);                            //取消片选     	      
} 		
  		    
/********************************************************************************************************			                                                           
*  功能: 	 读取SPI FLASH【在指定地址开始读取指定长度的数据】
*  输入参数: pBuffer：      数据存储区
			 ReadAddr：     开始读取的地址(24bit)
		     NumByteToRead：要读取的字节数(最大65535)
			 num：          与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_Read(unsigned char* pBuffer,unsigned int ReadAddr,unsigned short int NumByteToRead,unsigned char num)   
{ 
 	unsigned short int i;   										    
	
	W25Q_CS(0);                            	//使能器件   
    
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_ReadData);         	//发送读取命令   
		SPI1_ReadWriteByte((unsigned char)((ReadAddr)>>16));  	//发送24bit地址    
		SPI1_ReadWriteByte((unsigned char)((ReadAddr)>>8));   
		SPI1_ReadWriteByte((unsigned char)ReadAddr);   
		for(i=0;i<NumByteToRead;i++)
		{ 
			pBuffer[i]=SPI1_ReadWriteByte(0XFF);   	//循环读数  
		}
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_ReadData);         	//发送读取命令   
		SPI2_ReadWriteByte((unsigned char)((ReadAddr)>>16));  	//发送24bit地址    
		SPI2_ReadWriteByte((unsigned char)((ReadAddr)>>8));   
		SPI2_ReadWriteByte((unsigned char)ReadAddr);   
		for(i=0;i<NumByteToRead;i++)
		{ 
			pBuffer[i]=SPI2_ReadWriteByte(0XFF);   	//循环读数  
		}
	}
	
	W25Q_CS(1);  				    	      
}  

/********************************************************************************************************			                                                           
*  功能: 	 在一页内写入SPI FLASH【在指定地址开始写入最大256字节的数据】
*  输入参数: pBuffer：      数据存储区
			 WriteAddr：    开始写入的地址(24bit)
		     NumByteToRead：要写入的字节数(最大256)【该数不应该超过该页的剩余字节数】
			 num：          与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_Write_Page(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short int NumByteToWrite, unsigned char num)
{
 	unsigned short int i;  
    
	W25QXX_Write_Enable(spi_num);           //SET WEL 
	
	W25Q_CS(0);                           	//使能器件   
    
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_PageProgram);      	//发送写页命令   
		SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>16)); 	//发送24bit地址    
		SPI1_ReadWriteByte((unsigned char)((WriteAddr)>>8));   
		SPI1_ReadWriteByte((unsigned char)WriteAddr);   
		
		for(i=0;i<NumByteToWrite;i++)
			SPI1_ReadWriteByte(pBuffer[i]);//循环写数 
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_PageProgram);      	//发送写页命令   
		SPI2_ReadWriteByte((unsigned char)((WriteAddr)>>16)); 	//发送24bit地址    
		SPI2_ReadWriteByte((unsigned char)((WriteAddr)>>8));   
		SPI2_ReadWriteByte((unsigned char)WriteAddr);   
		
		for(i=0;i<NumByteToWrite;i++)
			SPI2_ReadWriteByte(pBuffer[i]);//循环写数 
	}
	
	W25Q_CS(1);                           	//取消片选 
	
	W25QXX_Wait_Busy();					   		//等待写入结束
} 

/********************************************************************************************************			                                                           
*  功能: 	 无检验写SPI FLASH【在指定地址开始写入指定长度的数据，必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败！写数据时自动换页】
*  输入参数: pBuffer：       数据存储区
			 WriteAddr：     开始写入的地址(24bit)
		     NumByteToWrite：要写入的字节数(最大256)【该数不应该超过该页的剩余字节数】
*  返回值：  无
********************************************************************************************************/
void W25QXX_Write_NoCheck(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short int NumByteToWrite)   
{ 			 		 
	unsigned short int pageremain;	   
	
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	
	if(NumByteToWrite<=pageremain) pageremain=NumByteToWrite;//不大于256个字节
		
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain,spi_num);
		
		if(NumByteToWrite==pageremain) break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	
			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256) pageremain=256; //一次可以写入256个字节
			else   pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}	    
}

/********************************************************************************************************			                                                           
*  功能: 	 写SPI FLASH【在指定地址开始写入指定长度的数据，函数先检验待写入空间是否为0xff，不是则先擦除再写入】
*  输入参数: pBuffer：       数据存储区
			 WriteAddr：     开始写入的地址(24bit)
		     NumByteToWrite：要写入的字节数(最大65535) 
*  返回值：  无
********************************************************************************************************/
void W25QXX_Write(unsigned char* pBuffer,unsigned int WriteAddr,unsigned short int NumByteToWrite)   
{ 
	unsigned int secpos;
	unsigned short int secoff;
	unsigned short int secremain;	   
 	unsigned short int i;      
   
	unsigned char* W25QXX_BUF;

	W25QXX_BUF=W25QXX_BUFFER;
 	
	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	
	if(NumByteToWrite<=secremain) secremain=NumByteToWrite;//不大于4096个字节
		
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096,4096,spi_num);//读出整个扇区的内容
		
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)	break;//需要擦除  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos,spi_num);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}
		else 
		{
			W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
		}			
		
		if(NumByteToWrite==secremain) break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			
			if(NumByteToWrite>4096)
				secremain=4096;//下一个扇区还是写不完
			else
				secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	}	 
}

/********************************************************************************************************			                                                           
*  功能: 	 擦除整个芯片【擦除时间会比较长】
*  输入参数: num：与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_Erase_Chip(unsigned char num)   
{                                   
    W25QXX_Write_Enable(spi_num);                 	 	//SET WEL 
    W25QXX_Wait_Busy();   
  	
	W25Q_CS(0);                            	//使能器件   
    
	if(num==1) SPI1_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令 
	if(num==2) SPI2_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令 
	
	W25Q_CS(1);                            	//取消片选     	      
	
	W25QXX_Wait_Busy();   				   		//等待芯片擦除结束
}  

/********************************************************************************************************			                                                           
*  功能: 	 擦除一个扇区【擦除时间最少要150ms】
*  输入参数: Dst_Addr：扇区地址【根据实际容量设置】
			 num：     与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_Erase_Sector(unsigned int Dst_Addr,unsigned char num)   
{  
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	
	Dst_Addr*=4096;
    W25QXX_Write_Enable(spi_num);                  	       //SET WEL 	 
    W25QXX_Wait_Busy();   
  	
	W25Q_CS(0);                            	         //使能器件   
    
	if(num==1)
	{
		SPI1_ReadWriteByte(W25X_SectorErase);      	    //发送扇区擦除指令 
		SPI1_ReadWriteByte((unsigned char)((Dst_Addr)>>16));  	   //发送24bit地址    
		SPI1_ReadWriteByte((unsigned char)((Dst_Addr)>>8));   
		SPI1_ReadWriteByte((unsigned char)Dst_Addr); 
	}
	else if(num==2)
	{
		SPI2_ReadWriteByte(W25X_SectorErase);      	    //发送扇区擦除指令 
		SPI2_ReadWriteByte((unsigned char)((Dst_Addr)>>16));  	   //发送24bit地址    
		SPI2_ReadWriteByte((unsigned char)((Dst_Addr)>>8));   
		SPI2_ReadWriteByte((unsigned char)Dst_Addr); 
	}
	
	W25Q_CS(1);                            	     //取消片选     	      
    W25QXX_Wait_Busy();   				   		//等待擦除完成
} 

/********************************************************************************************************			                                                           
*  功能: 	 等待芯片完成操作
*  输入参数: 无
*  返回值：  无
********************************************************************************************************/
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(spi_num)&0x01)==0x01);  		// 等待BUSY位清空
}

/********************************************************************************************************			                                                           
*  功能: 	 进入掉电模式
*  输入参数: num：与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_PowerDown(unsigned char num)   
{ 
  	W25Q_CS(0);                           	 	//使能器件   
    
	if(num==1)
		SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令
	else if(num==2)
		SPI1_ReadWriteByte(W25X_PowerDown);        //发送掉电命令
	
	W25Q_CS(1);                           	  //取消片选     	      
    xDelay_us(3);                              //等待TPD  
}   

/********************************************************************************************************			                                                           
*  功能: 	 唤醒芯片
*  输入参数: num：与W25QXX相连的SPI口
*  返回值：  无
********************************************************************************************************/
void W25QXX_WAKEUP(unsigned char num)   
{  
  	W25Q_CS(0);                            	//使能器件   
    
	if(num==1) SPI1_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB 
	if(num==2) SPI2_ReadWriteByte(W25X_ReleasePowerDown);	//  send W25X_PowerDown command 0xAB
	
	W25Q_CS(1);                            	//取消片选     	      
  xDelay_us(3);                            	//等待TRES1
}   

