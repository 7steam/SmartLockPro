#include <stdio.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <string.h>


void send(void *pvParam);
void re(void *pvParam);


void Semaphores();
void PS_GetImage(void *pvParam);
void PS_GenChar1();
void PS_GenChar2();
void PS_RegModel();
void PS_StoreChar();
void PS_Search( );
void PS_Match(void *pvParam);

int uart_buffer_size = (1024 * 2);
QueueHandle_t uart_queue;
 uart_port_t uart_num = UART_NUM_1;
 uint8_t data[128];
uint8_t data2[128];
uint8_t data3[128];
uint8_t data4[128];
uint8_t data5[128];
uint8_t data6[128];

int length = 0;
int sin;




SemaphoreHandle_t xSemAB, xSemBC1,xSemBC2,xSemCA,xSemCD;

SemaphoreHandle_t xUartMutex;




void app_main(void)
{
    

sin=1;

uart_config_t uart_config = {
    .baud_rate = 57600,         //波特率
    .data_bits = UART_DATA_8_BITS,  //数据位数
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,   //停止位
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    
};

 uart_param_config(UART_NUM_1, &uart_config);
uart_set_pin(UART_NUM_1, 4, 5, 18, 19);

uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0);
 





memset(data2, 0XFF, sizeof(data2)); 
memset(data3, 0XFF, sizeof(data3));
memset(data4, 0XFF, sizeof(data4));
uart_flush_input(uart_num);
Semaphores();
xTaskCreate(PS_GetImage,"uart_1",10000,NULL,2,NULL );




}








void Semaphores(){
 
   xUartMutex = xSemaphoreCreateMutex();
  

 
}














//探测手指，探测到后录入指纹图像存于ImageBuffer，返回确认码
void PS_GetImage(void *pvParam){
   
    

  uint8_t send_data[12]={0xEF,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x01,0x00,0x05};
   while(1)
   
   {
   
    
     xSemaphoreGive(xUartMutex);
   
   
   xSemaphoreTake(xUartMutex, portMAX_DELAY);
   
      uart_write_bytes(uart_num, (const char*)send_data, 12);

      
   
   
   length = uart_read_bytes(uart_num, data, 12, 100);
   
  
  if(data[9]==0x00)
   {printf("录入成功\n");
     printf("data[9]: 0x%02X\n", data[9]);
    
     if(data2[9]!=0x00){
    xSemaphoreGive(xUartMutex);
    PS_GenChar1( );

     if(sin==1&&data2[9]==0x00){
      xSemaphoreGive(xUartMutex);
      PS_Search( );
    }

    continue;}
   
   
    if(sin==0&&data2[9]==0x00&&data3[9]!=0x00)
    {
      xSemaphoreGive(xUartMutex);
      PS_GenChar2( );
    continue;}
    }
         
   if(data[9]==0x01)
   {printf("错误\n");
          }

   if(data[9]==0x02)
   {printf("无手指\n");
          }

   if(data[9]==0x03)
   {printf("录入不成功\n");
          } 



    if(sin==0&&data2[9]==0x00&&data3[9]==0x00){
           xSemaphoreGive(xUartMutex);
           PS_RegModel( );
    }

    if(sin==0&&data4[9]==0x00){
       xSemaphoreGive(xUartMutex);
          PS_StoreChar( );
    }

   
         xSemaphoreGive(xUartMutex);
     vTaskDelay(100 / portTICK_PERIOD_MS);    
   }
}


//将ImageBuffer中的原始图像生成指纹特征文件存于CharBuffer1和CharBuffer2
void PS_GenChar1(){


      // while(1){
   

   
  xSemaphoreTake(xUartMutex, portMAX_DELAY);
 uint8_t Gen_data1[13]={0xEF,0x01,
    0xff,0xff,0xff,0xff,
    0x01,
    0x00,0x04,
    0x02,
    0x01,
       0x00,0x00};

        uint16_t checksum = 0;
            for(int i=6;i<11;i++){
              checksum+= Gen_data1[i];
            }
            Gen_data1[11]=(checksum>>8)&0xFF;
             Gen_data1[12]=checksum&0XFF;




            uart_write_bytes(uart_num, (const char*)Gen_data1, 13);
              vTaskDelay(50/ portTICK_PERIOD_MS);
            length = uart_read_bytes(uart_num, data2, 12, 100);
           
            if (length <= 0) {
               printf("UART read failed! Length: %d\n", length);
}
            printf("第一次%02X\n",data2[9]);


        
            xSemaphoreGive(xUartMutex);
         
     
}


void PS_GenChar2(void *pvParam){

    

xSemaphoreTake(xUartMutex, portMAX_DELAY);
uint8_t Gen_data2[13]={0xEF,0x01,
                        0xff,0xff,0xff,0xff,
                        0x01,
                        0x00,0x04,
                        0x02,
                        0x02,
       0x00,0x00};

        int sum=0;
            for(int i=6;i<13;i++){
              sum+= Gen_data2[i];
            }
            Gen_data2[11]=sum>>8;
             Gen_data2[12]=sum&0XFF;

            uart_write_bytes(uart_num, (const char*)Gen_data2, 13);
              vTaskDelay(50/ portTICK_PERIOD_MS);
            length = uart_read_bytes(uart_num, data3, 12, 100);
           
             if (length <= 0) {
               printf("UART read failed! Length: %d\n", length);
}
            printf("第二次%02X\n",data3[9]);
               
           xSemaphoreGive(xUartMutex);
             
       }  
//}















//精确对比CharBuffer1和CharBuffer2中的特征文件
void PS_Match(void *pvParam){
       uint8_t Match_data[12]={0xEF,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x03,0x07};
       while(1){
              uart_write_bytes(uart_num, (const char*)Match_data, 12);
               vTaskDelay(50/ portTICK_PERIOD_MS);
            length = uart_read_bytes(uart_num, data, 14, 100);
            printf("%02X\n",data[9]);
             vTaskDelay(10 / portTICK_PERIOD_MS);
       }
}


//以CharBuffer1和CharBuffer2中的特征文件搜索整个或部分指纹库。若搜索到则返回页码
void PS_Search(void *pvParam){
    
  xSemaphoreTake(xUartMutex, portMAX_DELAY);
  uint8_t Search_data[17]={0xEF,0x01,
                              0xff,0xff,0xff,0xff,
                              0x01,
                              0x00,0x08,
                              0x04,
                              0x01,
                              0x00,0x00,   //starpage
                              0x00,0xFF,   //pageNum
                              0x00,0x00};

            int sum=0;
            for(int i=6;i<17;i++){
              sum+= Search_data[i];
            }
            Search_data[15]=sum>>8;
             Search_data[16]=sum&0XFF;

     //while(1){
      uart_write_bytes(uart_num, (const char*)Search_data, 17);
              
            length = uart_read_bytes(uart_num, data6, 16, 100);
 if (length <= 0) {
               printf("UART read failed! Length: %d\n", length);
}

    printf("匹配%02X\n",data6[9]);
      data2[9]=0XEE;
             xSemaphoreGive(xUartMutex);
           
}

//将CharBuffer1和CharBuffer2中的特征文件合并并生成模板，结果存于CharBuffer1和CharBuffer2
void PS_RegModel(void *pvParam){
  
 xSemaphoreTake(xUartMutex, portMAX_DELAY);
 
    uint8_t RegModel_data[12]={0xEF,0x01,
                               0xff,0xff,0xff,0xff,
                               0x01,
                               0x00,0x03,
                               0x05,
                               0x00,0x09};

     // while(1){
      uart_write_bytes(uart_num, (const char*)RegModel_data, 12);
               vTaskDelay(50/ portTICK_PERIOD_MS);
            length = uart_read_bytes(uart_num, data4, 12, 100);
           if (length <= 0) {
               printf("UART read failed! Length: %d\n", length);
}
 
            printf("合并%02X\n",data4[9]);
           // printf("第一次%02X\n",data2[9]);
            xSemaphoreGive(xUartMutex);
                          
}



//将CharBuffer1和CharBuffer2中的模板文件存到PageID号flash数据库位置
void PS_StoreChar(void *pvParam){

  xSemaphoreTake(xUartMutex, portMAX_DELAY);
 
      uint8_t StoreChar_data[15]={0xEF,0X01,
                                  0xff,0xff,0xff,0xff,
                                  0x01,
                                  0x00,0x06,
                                  0x06,
                                  0x02,
                                  0x00,0x01,
                                  0x00,0x00};

            int sum=0;
            for(int i=6;i<15;i++){
              sum+= StoreChar_data[i];
            }
            StoreChar_data[13]=sum>>8;
             StoreChar_data[14]=sum&0XFF;                       


      uart_write_bytes(uart_num, (const char*)StoreChar_data, 15);
               vTaskDelay(50/ portTICK_PERIOD_MS);
            length = uart_read_bytes(uart_num, data5, 12, 100);
            if (length <= 0) {
               printf("UART read failed! Length: %d\n", length);
}
             printf("存储%02X\n",data5[9]);
           xSemaphoreGive(xUartMutex);
          
}



//将flash数据库中指定ID号的指纹模板读入到模板缓冲区CharBuffer1或CharBuffer2
void PS_LoadChar(void *pvParam){
       uint8_t LoadChar_data[15]={0xEF,0x01,
                                  0xff,0xff,0xff,0xff,
                                  0x01,
                                  0x00,0x06,
                                  0x07,
                                  0x01,
                                  0x00,0x02,
                                  0x00,0x00};

              int sum=0;
            for(int i=6;i<15;i++){
              sum+= LoadChar_data[i];
            }
            LoadChar_data[13]=sum>>8;
            LoadChar_data[14]=sum&0XFF;  
           
      while(1){
      uart_write_bytes(uart_num, (const char*)LoadChar_data, 15);
               vTaskDelay(50/ portTICK_PERIOD_MS);
            length = uart_read_bytes(uart_num, data, 12, 100);
             vTaskDelay(10 / portTICK_PERIOD_MS);
     }                     

}