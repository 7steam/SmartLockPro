#include <stdio.h>
#include "driver/i2c_master.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <string.h>
#include "u8g2.h"
#include "esp_log.h"


esp_err_t err;
u8g2_t u8g2;
static const char *TAG = "I2C_DRIVER";



#define ADDR    0x3C
#define I2C_PORT I2C_NUM_0 
#define OLED_WIDTH       128
#define OLED_HEIGHT      64
#define GPIO_NUM_4       4
#define GPIO_NUM_5       5




void oled_init();
void oled_clear();



uint8_t u8g2__i2c_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr); 
uint8_t u8g2_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr); 
 uint8_t u8x8_byte_espidf_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


 










 void app_main(void)
{
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


    
//oled_init( );

//oled_clear( );
 ESP_LOGI(TAG, "初始化U8G2显示库");
 u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,U8G2_R0,
                                        u8x8_byte_espidf_hw_i2c,
                                        u8g2_gpio_and_delay_cb );


       vTaskDelay(pdMS_TO_TICKS(100));                                   
    
    ESP_LOGI(TAG, "Initialize the display");
    u8g2_InitDisplay(&u8g2);
    ESP_LOGI(TAG, "Init end");


    u8g2_SetPowerSave(&u8g2, 0);
    ESP_LOGI(TAG, "power setting end");

   // oled_init();
    ESP_LOGI(TAG, "clear start");
    u8g2_ClearBuffer(&u8g2);
    ESP_LOGI(TAG, "clear end");

    ESP_LOGI(TAG, "fint setting"); 
    u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
   // u8g2_SetFontMode(&u8g2, 0);
    u8g2_DrawStr(&u8g2, 10, 15, "1");
     //u8g2_DrawPixel(&u8g2,10,10);
    
     ESP_LOGI(TAG, "发送缓冲区内容到显示");
   u8g2_SendBuffer(&u8g2);
   
  
  

   vTaskDelay(pdMS_TO_TICKS(5000)); 

   

  u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
  u8g2_SetFontMode(&u8g2, 0);
  u8g2_DrawStr(&u8g2, 20, 15, "2");
  u8g2_SendBuffer(&u8g2);


  vTaskDelay(pdMS_TO_TICKS(5000)); 

   

  u8g2_SetFont(&u8g2, u8g2_font_7x13B_tf);
  u8g2_SetFontMode(&u8g2, 0);
  u8g2_DrawStr(&u8g2, 30, 15, "3");
  u8g2_SendBuffer(&u8g2);


 while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));

         ESP_LOGI(TAG, "循环");
    }

}




//OLED地址 + 写位（0x3C << 1 | 0 → 0x78）
//控制字节：0x00（命令）或 0x40（数据）
//数据/命令字节
//停止条件（Stop

void OLED_cmd(uint8_t cmd)
{
i2c_cmd_handle_t OLED_start = i2c_cmd_link_create();
 i2c_master_start(OLED_start);
 i2c_master_write_byte(OLED_start, (ADDR << 1) | 0x00, true);
    i2c_master_write_byte(OLED_start, 0x00, true); // 控制字节：命令
    i2c_master_write_byte(OLED_start, cmd, true);
    i2c_master_stop(OLED_start);
    i2c_master_cmd_begin(I2C_PORT, OLED_start, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(OLED_start);

}



void oled_init() {
    // 初始化命令序列
    const uint8_t cmds[] = {
        0xAE, // 关闭显示
        0xD5, 0x80, // 设置时钟分频
        0xA8, 0x3F, // 设置复用率
        0xD3, 0x00, // 设置显示偏移
        0x40,       // 设置显示起始行
        0x8D, 0x14, // 启用电荷泵
        0x20, 0x00, // 水平地址模式
       0xA1,       // 段重映射
        0xC8,       // COM扫描方向
        0xDA, 0x12, // COM引脚配置
        0x81, 0xCF, // 对比度设置
        0xD9, 0xF1, // 预充电周期
        0xDB, 0x30, // VCOMH级别
        0xA4,       // 恢复显示内容
        0xA6,       // 正常显示
        0xAF        // 开启显示
    };
     // 发送初始化命令
    for (int i = 0; i < sizeof(cmds); i++) {
        OLED_cmd(cmds[i]);
    }
}




void oled_clear() {
    for (int page = 0; page < 8; page++) {
        OLED_cmd(0xB0 | page); 
        OLED_cmd(0x00);       
        OLED_cmd(0x10);       
        
      
       
            i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();
            i2c_master_start(cmd_link);
            i2c_master_write_byte(cmd_link, (ADDR << 1) | 0x00, true);
            i2c_master_write_byte(cmd_link, 0x40, true);

             for (int col = 0; col < 128; col++) {
            i2c_master_write_byte(cmd_link, 0x00, true);
             }
            i2c_master_stop(cmd_link);
            i2c_master_cmd_begin(I2C_PORT, cmd_link, pdMS_TO_TICKS(100));
            i2c_cmd_link_delete(cmd_link);
       
    }
}












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