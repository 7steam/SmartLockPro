#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


#include "driver/i2c_master.h"
#include "driver/i2c.h"
#include "u8g2.h"
#include "esp_log.h"

//oled全局变量
esp_err_t err;
u8g2_t u8g2;
static const char *TAG = "I2C_DRIVER";

#define ADDR    0x3C
#define I2C_PORT I2C_NUM_0 
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define GPIO_NUM_4       4
#define GPIO_NUM_5       5

//u8g2回调函数声明
uint8_t u8g2__i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr); 
uint8_t u8g2_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr); 
uint8_t u8x8_byte_espidf_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void display(int x,int y,char z);


//AS608全局变量
void Semaphores();
void PS_GetImage(void *pvParam);
void PS_GenChar1();
void PS_GenChar2();
void PS_RegModel();
void PS_StoreChar();
void PS_Search( );
void PS_Match(void *pvParam);



void password();

int uart_buffer_size = (1024 * 2);
QueueHandle_t uart_queue;
 uart_port_t uart_num = UART_NUM_1;
 uint8_t data[128];
uint8_t data2[128];
uint8_t data3[128];
uint8_t data4[128];
uint8_t data5[128];
uint8_t data6[128];



const int Password[5]={1,3,5,6,7};
int passwod_into[5]={0,0,0,0,0};

int length = 0;
int sin;
int password_len;
int cross=0;
SemaphoreHandle_t xUartMutex;
int k1,k2,k3,k4;
//display的辅助数组
//char oled_str[2];








#define key_r1 19
#define key_r2 13
#define key_r3 0
#define key_r4 1

#define key_c1 2
#define key_c2 3
#define key_c3 10
#define key_c4 6


#define motor1 7
#define motor2 18
#define motor3 12
#define motor4 11

TaskHandle_t motor_1;
TaskHandle_t motor_2;
TaskHandle_t motor_3;
void key(void *pvParam );
void key_r(int *tem,int *point ,gpio_num_t gpio_num);


void motor_left(void *pvParam );
void motor_right(void *pvParam );
void motor_over(void *pvParam );



//int key[12]={1,2,3,4,9,10,11,12,13,14,15,16,17,18};
int singn[22];
   

void app_main(void)
{

//oled_i2c配置和初始化
esp_log_level_set(TAG, ESP_LOG_DEBUG);


     ESP_LOGI(TAG, "project start");
    i2c_config_t i2c_conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = GPIO_NUM_4,  
    .scl_io_num = GPIO_NUM_5, 
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 400000,  
};
  ESP_LOGI(TAG, "配置I2C参数"); 
  i2c_param_config(I2C_PORT, &i2c_conf);
 ESP_LOGI(TAG, "安装I2C驱动程序");
  esp_err_t ret=i2c_driver_install(I2C_PORT, i2c_conf.mode, 0, 0, 0);
if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C驱动安装失败: %d", ret);
        return;
    }

//u8g2初始化函数
ESP_LOGI(TAG, "初始化U8G2显示库");
 u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,U8G2_R0,
                                        u8x8_byte_espidf_hw_i2c,
                                        u8g2_gpio_and_delay_cb );

 vTaskDelay(pdMS_TO_TICKS(100));                                   
   
 //oled初始化显示
    ESP_LOGI(TAG, "Initialize the display");
    u8g2_InitDisplay(&u8g2);
    ESP_LOGI(TAG, "Init end");


    u8g2_SetPowerSave(&u8g2, 0);
    ESP_LOGI(TAG, "power setting end");
     u8g2_ClearBuffer(&u8g2);
     u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
    u8g2_DrawStr(&u8g2, 10, 15, "welcome use"); 
     u8g2_SendBuffer(&u8g2);
//AS608配置与初始化

 sin=1;
  password_len=0;

uart_config_t uart_config = {
    .baud_rate = 57600,         //波特率
    .data_bits = UART_DATA_8_BITS,  //数据位数
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,   //停止位
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    
};

 uart_param_config(UART_NUM_1, &uart_config);
uart_set_pin(UART_NUM_1, 8, 9, 18, 19);

uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0);
 





memset(data2, 0XFF, sizeof(data2)); 
memset(data3, 0XFF, sizeof(data3));
memset(data4, 0XFF, sizeof(data4));
uart_flush_input(uart_num);
Semaphores();













//电机gpio设置
gpio_config_t motor_conf;

   motor_conf.mode = GPIO_MODE_OUTPUT;
   motor_conf.pin_bit_mask = (1 << motor1) | (1 << motor2) | (1 << motor3) | (1 << motor4);
   esp_err_t r = gpio_config(&motor_conf);


//电机四根信号线全部输出高电平
   gpio_set_level(motor1, 1);
   gpio_set_level(motor2, 1);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor4, 1);


   //int singn[18];
   singn[6]=1;
   singn[7]=2;
   singn[8]=3;
   singn[9]=4;


   singn[10]=9;
   singn[11]=10;
   singn[12]=11;
   singn[13]=12;


   singn[14]=13;
   singn[15]=14;
   singn[16]=15;
   singn[17]=16;

   singn[18]=5;
   singn[19]=6;
   singn[20]=7;
   singn[21]=8;
   
   // void motor_left(void *pvParam );
   // void motor_right(void *pvParam );
   // void motor_over(void *pvParam );



    



   
   




   //TaskHandle_t motor_1;
    xTaskCreate(PS_GetImage,"uart_1",10000,NULL,1,NULL );
    
    xTaskCreate(key,"keybroad",10000,(void*) singn,1, NULL);
    xTaskCreate(motor_left,"motorl",10000,NULL,1, &motor_1);
    xTaskCreate(motor_right,"motorr",10000,NULL,1, &motor_2);
    xTaskCreate(motor_over,"motorro",10000,NULL,1, &motor_3);
    vTaskSuspend(motor_1);
    vTaskSuspend(motor_2);
     vTaskSuspend(motor_3);
    
    //vTaskResume(motor_1);
  
}






void motor_left( void *pvParam){
  while(1){
   gpio_set_level(motor1, 0);
  vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor1, 1);
   gpio_set_level(motor2, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor2, 1);
   gpio_set_level(motor3, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor4, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor4, 1);
   vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

   


void motor_right(void *pvParam ){
   while(1){
   gpio_set_level(motor4, 0);
  vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor4, 1);
   gpio_set_level(motor3, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor2, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor2, 1);
   gpio_set_level(motor1, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor1, 1);
   vTaskDelay(10 / portTICK_PERIOD_MS);
   }
}



void motor_over(void *pvParam  ){
  while(1){
   gpio_set_level(motor1,1);
   gpio_set_level(motor2,1);
   gpio_set_level(motor3,1);
   gpio_set_level(motor4,1);   
  vTaskDelay(10 / portTICK_PERIOD_MS);
}


}


//键盘对行扫描函数
void key_r(int *tem,int *point ,gpio_num_t gpio_num){
   gpio_config_t led_conf;
    memset(&led_conf,0,sizeof(led_conf));
   led_conf.mode = GPIO_MODE_OUTPUT;
   led_conf.pin_bit_mask = (1<<key_r1)|(1<<key_r2)|(1<<key_r3)|(1<<key_r4);
   led_conf.pull_down_en = 0; 
   esp_err_t r = gpio_config(&led_conf);
   

   gpio_config_t led_conf1;
   led_conf1.mode = GPIO_MODE_INPUT;
   led_conf1.pin_bit_mask = (1<<key_c1)|(1<<key_c2)|(1<<key_c3)|(1<<key_c4);
   led_conf1.pull_down_en = 1;
   esp_err_t m = gpio_config(&led_conf1);
  
     k1=0;
     k2=0;
     k3=0;
     k4=0;

   gpio_set_level(gpio_num, 1);
    *(tem+4)=gpio_get_level(key_c1);
    if(*(tem+4)==1)
    {
      printf("%d\n",*point);
     // if(reg==0){
      passwod_into[password_len]=*point;
     // }
     // if(reg==1){
       // Password[password_len]=*point;
    //  }
      
      printf("%d\n",password_len);
      password_len++;

      k1=1;
  
    }
    *(tem+4)=gpio_get_level(key_c2);
     if(*(tem+4)==1)
    {printf("%d\n",*(point+1));
      // if(reg==0){
       passwod_into[password_len]=*(point+1);
        // }

         // if(reg==1){
       // Password[password_len]=*point;
    //  }
       printf("%d\n",password_len);
      password_len++;
      k2=1;
    
    }  
    *(tem+4)=gpio_get_level(key_c3);
     if(*(tem+4)==1)
    {printf("%d\n",*(point+2));
      // if(reg==0){
       passwod_into[password_len]=*(point+2);
        // }

         // if(reg==1){
       // Password[password_len]=*point;
    //  }

       printf("%d\n",password_len);
      password_len++;
       k3=1;
    } 
     *(tem+4)=gpio_get_level(key_c4);
     if(*(tem+4)==1)
    {printf("%d\n",*(point+3));
      // if(reg==0){
       passwod_into[password_len]=*(point+3);

       // }

         // if(reg==1){
       // Password[password_len]=*point;
    //  }

       printf("%d\n",password_len);
      password_len++;
      k4=1;
    }
}


void password(){
  for(int i=0;i<5;i++){
    if(passwod_into[i]==Password[i])
      continue;
    else
    {
       printf("密码错误");
      cross=1;
       break; 
    }
  }
    

  if(cross!=1)
  { vTaskSuspend(motor_3);
      vTaskSuspend(motor_2);
      vTaskResume(motor_1);
    }
    cross=0;
}







//键盘函数
void key(void *pvParam ){


gpio_config_t led_conf;
  
    memset(&led_conf,0,sizeof(led_conf));

   led_conf.mode = GPIO_MODE_INPUT;
   led_conf.pin_bit_mask = (1<<key_r1)|(1<<key_r2)|(1<<key_r3)|(1<<key_r4);
   led_conf.pull_down_en = 1; 


gpio_config_t led_conf1;
   
   led_conf1.mode = GPIO_MODE_OUTPUT;
   led_conf1.pin_bit_mask = (1<<key_c1)|(1<<key_c2)|(1<<key_c3)|(1<<key_c4);


   int *pArrayAddr;
    pArrayAddr=(int *)pvParam;
   
 while(1){
   
  

 
   esp_err_t r = gpio_config(&led_conf);
  
   esp_err_t m = gpio_config(&led_conf1);
 

     gpio_set_level(key_c1, 1);
     gpio_set_level(key_c2, 1);
     gpio_set_level(key_c3, 1);
     gpio_set_level(key_c4, 1);
    
   
    *pArrayAddr=gpio_get_level(key_r1);
    *(pArrayAddr+1)=gpio_get_level(key_r2);
    *(pArrayAddr+2)=gpio_get_level(key_r3);
    *(pArrayAddr+3)=gpio_get_level(key_r4);

    
    

    //扫描第一行
    if(*pArrayAddr==1)
    {
      //调用函数扫描第一行
      key_r(pArrayAddr,pArrayAddr+6 ,key_r1);

        printf("k1=%d\n",k1);

         if(password_len==1)
          {u8g2_ClearBuffer(&u8g2);
            u8g2_SendBuffer(&u8g2);
          }

      if(k1==1)
      {
        u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
       u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "1"); 
        u8g2_SendBuffer(&u8g2);
      }

      if(k2==1){
       
       u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
       u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "2"); 
       u8g2_SendBuffer(&u8g2);
      }

       if(k3==1){
       u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
       u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "3"); 
       u8g2_SendBuffer(&u8g2);
       
      }
   
       if(k4==1){
      u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
       u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "4"); 
       u8g2_SendBuffer(&u8g2);
       
      }

       
      
   /**/

    }
    
    
    
    //扫描第二行
    if(*(pArrayAddr+1)==1)
    {


       //修改后
       key_r(pArrayAddr,pArrayAddr+18 ,key_r2);
      
       if(password_len==1)
     {u8g2_ClearBuffer(&u8g2);}

     if(k1==1){
        
        u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
        u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "5"); 
        u8g2_SendBuffer(&u8g2);
      }

       if(k2==1){
       
         u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
         u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "6"); 
         u8g2_SendBuffer(&u8g2);
      }

       if(k3==1){
         u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
         u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "7"); 
         u8g2_SendBuffer(&u8g2);
      }
   
       if(k4==1){
         u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
         u8g2_DrawStr(&u8g2, 10*(password_len+1), 15, "8"); 
         u8g2_SendBuffer(&u8g2);
      }



  
    }
    


    //扫描第三行
    if(*(pArrayAddr+2)==1)
    {
   
   key_r(pArrayAddr,pArrayAddr+10 ,key_r3);


    if(k1==1){
        display(10*(password_len+1),15,9);
      }

       if(k2==1){
        display(10*(password_len+1),15,10);
      }

       if(k3==1){
        display(10*(password_len+1),15,11);
      }
   
       if(k4==1){
        display(10*(password_len+1),15,12);
      }

    

    }
    




   //扫描第四行
    if(*(pArrayAddr+3)==1)
    {
   
    key_r(pArrayAddr,pArrayAddr+14 ,key_r4);
   
   
    
   if(k3==1)
      { sin=1;
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
    u8g2_DrawStr(&u8g2, 10, 15, "pi pei"); 
    u8g2_SendBuffer(&u8g2);}
   if(k4==1)
       {sin=0;
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
    u8g2_DrawStr(&u8g2, 10, 15, "lu ru zhi wen"); 
    u8g2_SendBuffer(&u8g2);}

       k3=0,k4=0;
    }

    //密码检测

    if(password_len==5){
      password();
      printf("passworld:");
      for(int j=0;j<5;j++)
     printf("%d ",passwod_into[j]);
      
      u8g2_ClearBuffer(&u8g2);
      u8g2_SendBuffer(&u8g2); 
      password_len=0;
    }
    
    
   // if(*(pArrayAddr+5)==5)
   // {
    //  vTaskSuspend(motor_3);
    //  vTaskSuspend(motor_2);
    //  vTaskResume(motor_1);

   
  // }

   // if(*(pArrayAddr+5)==6)
   // {
    //  vTaskSuspend(motor_3);
    //  vTaskSuspend(motor_1);
    //  vTaskResume(motor_2);
   // }

   // if(*(pArrayAddr+5)==7)
   // {
     // vTaskSuspend(motor_1);
     // vTaskSuspend(motor_2);
     // vTaskResume(motor_3);
   // }
      vTaskDelay(50 / portTICK_PERIOD_MS); 
   }

}











void Semaphores(){
 
   xUartMutex = xSemaphoreCreateMutex();
  

 
}














//探测手指，探测到后录入指纹图像存于ImageBuffer，返回确认码
void PS_GetImage(void *pvParam){
   
    

  uint8_t send_data[12]={0xEF,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0x01,0x00,0x05};
   while(1)
       
   {
   
    
     //xSemaphoreGive(xUartMutex);
   
   
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
   {
    printf("无手指\n");
    //printf("kk\n");
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
    if(data6[9]==0x00){
    vTaskResume(motor_1);}
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








//u8g2回调函数

uint8_t u8g2__i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{

     ESP_LOGI(TAG, "The callback function is called, message type: %d", msg); 
      static i2c_cmd_handle_t cmd = NULL;
        static uint8_t dc_mode = 0;

      switch(msg)
      {
        case U8X8_MSG_BYTE_SEND:
         ESP_LOGI(TAG, "发送字节: 0x%02x", arg_int);
        
          if (cmd != NULL) {
                i2c_master_write_byte(cmd, arg_int, true);
            } else {
                ESP_LOGE(TAG, "CMD is NULL during BYTE_SEND");
                return 0;
            }
         break;
        case U8X8_MSG_BYTE_INIT:
         break;
        case U8X8_MSG_BYTE_SET_DC:
         ESP_LOGI(TAG, "set DC: %s", arg_int ? "data" : "commond");
         dc_mode = arg_int;
         break;
        case U8X8_MSG_BYTE_START_TRANSFER:

         ESP_LOGI(TAG, "开始I2C传输");
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (ADDR << 1) | 0x00, true);
        uint8_t control_byte = dc_mode ? 0x40 : 0x00;
            i2c_master_write_byte(cmd, control_byte, true);
        break;
        case U8X8_MSG_BYTE_END_TRANSFER:
          ESP_LOGI(TAG, "结束传输");
        i2c_master_stop(cmd);
            err = i2c_master_cmd_begin(I2C_PORT, cmd, 1000 / portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
             cmd = NULL;
             if (err != ESP_OK) {
                ESP_LOGE(TAG, "I2C transfer failed: %d", err);
                return 0;
            }
            break;

             default:
            return 0; 
      }

      return 1;
}



uint8_t u8g2_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
     ESP_LOGI(TAG, "第二个回调函数被调用，消息类型: %d", msg);
    switch(msg)
    {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
           
            break;
            
        case U8X8_MSG_DELAY_MILLI:
         
            vTaskDelay(pdMS_TO_TICKS(arg_int));
            break;
            
        case U8X8_MSG_DELAY_10MICRO:
         
            esp_rom_delay_us(10);
            break;
            
        case U8X8_MSG_DELAY_100NANO:
           
            esp_rom_delay_us(1);
            break;
            
        case U8X8_MSG_GPIO_I2C_CLOCK:
        case U8X8_MSG_GPIO_I2C_DATA:
           
            break;
            
        case U8X8_MSG_GPIO_RESET:
           
            break;
            
        default:
            return 0;
    }
    return 1;
}




uint8_t u8x8_byte_espidf_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  // 从u8x8设备结构体中获取I2C配置数据
  //u8x8_esp32_i2c_t *i2c = (u8x8_esp32_i2c_t *)u8x8->bus_ptr;
  
  static uint8_t buffer[32];   // U8g2 will process data in chunks of 32 bytes
  static uint8_t buf_idx;
  uint8_t *data;

  switch (msg)
  {
    case U8X8_MSG_BYTE_SEND: // 发送数据（在START和STOP之间）
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
        buffer[buf_idx++] = *data;
        data++;
        arg_int--;
      }
      break;
      
    case U8X8_MSG_BYTE_INIT: // 初始化，通常已经在外部完成
      break;
      
    case U8X8_MSG_BYTE_SET_DC: // 对于I2C，忽略DC线（用于SPI）
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER: // 开始传输，重置缓冲区索引
      buf_idx = 0;
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER:   // 结束传输，执行实际的I2C写入
      // 创建I2C命令链接
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      
      // 启动位
      i2c_master_start(cmd);
      
      // 从机地址 + 写标志
      i2c_master_write_byte(cmd, (ADDR << 1) | I2C_MASTER_WRITE, true);
      
      // 写入数据缓冲区
      if (buf_idx > 0) {
        i2c_master_write(cmd, buffer, buf_idx, true);
      }
      
      // 停止位
      i2c_master_stop(cmd);
      
      // 执行I2C传输
      esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
      
      // 释放命令链接
      i2c_cmd_link_delete(cmd);
      
      // 检查传输是否成功
      if (ret != ESP_OK) {
        return 0; // 失败
      }
      break;
      
    default:
      return 0; // 未知消息类型，失败
  }
  return 1; // 成功
}


void display(int x,int y,char z)
{
 
   
   const char *oled_str = &z;
  u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
   u8g2_SetFontMode(&u8g2, 0);
  ESP_LOGI(TAG, "world start");
  u8g2_DrawStr(&u8g2, x, y, oled_str); 
  ESP_LOGI(TAG, "world over");
  ESP_LOGI(TAG, "send");
  u8g2_SendBuffer(&u8g2);
}