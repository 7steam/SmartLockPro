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
  ESP_LOGI(TAG, "����I2C����"); 
  i2c_param_config(I2C_PORT, &i2c_conf);
 ESP_LOGI(TAG, "��װI2C��������");
  esp_err_t ret=i2c_driver_install(I2C_PORT, i2c_conf.mode, 0, 0, 0);
if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C������װʧ��: %d", ret);
        return;
    }


    
//oled_init( );

//oled_clear( );
 ESP_LOGI(TAG, "��ʼ��U8G2��ʾ��");
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
    
     ESP_LOGI(TAG, "���ͻ��������ݵ���ʾ");
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

         ESP_LOGI(TAG, "ѭ��");
    }

}




//OLED��ַ + дλ��0x3C << 1 | 0 �� 0x78��
//�����ֽڣ�0x00������� 0x40�����ݣ�
//����/�����ֽ�
//ֹͣ������Stop

void OLED_cmd(uint8_t cmd)
{
i2c_cmd_handle_t OLED_start = i2c_cmd_link_create();
 i2c_master_start(OLED_start);
 i2c_master_write_byte(OLED_start, (ADDR << 1) | 0x00, true);
    i2c_master_write_byte(OLED_start, 0x00, true); // �����ֽڣ�����
    i2c_master_write_byte(OLED_start, cmd, true);
    i2c_master_stop(OLED_start);
    i2c_master_cmd_begin(I2C_PORT, OLED_start, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(OLED_start);

}



void oled_init() {
    // ��ʼ����������
    const uint8_t cmds[] = {
        0xAE, // �ر���ʾ
        0xD5, 0x80, // ����ʱ�ӷ�Ƶ
        0xA8, 0x3F, // ���ø�����
        0xD3, 0x00, // ������ʾƫ��
        0x40,       // ������ʾ��ʼ��
        0x8D, 0x14, // ���õ�ɱ�
        0x20, 0x00, // ˮƽ��ַģʽ
       0xA1,       // ����ӳ��
        0xC8,       // COMɨ�跽��
        0xDA, 0x12, // COM��������
        0x81, 0xCF, // �Աȶ�����
        0xD9, 0xF1, // Ԥ�������
        0xDB, 0x30, // VCOMH����
        0xA4,       // �ָ���ʾ����
        0xA6,       // ������ʾ
        0xAF        // ������ʾ
    };
     // ���ͳ�ʼ������
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
         ESP_LOGI(TAG, "�����ֽ�: 0x%02x", arg_int);
        
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

         ESP_LOGI(TAG, "��ʼI2C����");
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (ADDR << 1) | 0x00, true);
        uint8_t control_byte = dc_mode ? 0x40 : 0x00;
            i2c_master_write_byte(cmd, control_byte, true);
        break;
        case U8X8_MSG_BYTE_END_TRANSFER:
          ESP_LOGI(TAG, "��������");
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
     ESP_LOGI(TAG, "�ڶ����ص����������ã���Ϣ����: %d", msg);
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
  // ��u8x8�豸�ṹ���л�ȡI2C��������
  //u8x8_esp32_i2c_t *i2c = (u8x8_esp32_i2c_t *)u8x8->bus_ptr;
  
  static uint8_t buffer[32];   // U8g2 will process data in chunks of 32 bytes
  static uint8_t buf_idx;
  uint8_t *data;

  switch (msg)
  {
    case U8X8_MSG_BYTE_SEND: // �������ݣ���START��STOP֮�䣩
      data = (uint8_t *)arg_ptr;
      while( arg_int > 0 )
      {
        buffer[buf_idx++] = *data;
        data++;
        arg_int--;
      }
      break;
      
    case U8X8_MSG_BYTE_INIT: // ��ʼ����ͨ���Ѿ����ⲿ���
      break;
      
    case U8X8_MSG_BYTE_SET_DC: // ����I2C������DC�ߣ�����SPI��
      break;
      
    case U8X8_MSG_BYTE_START_TRANSFER: // ��ʼ���䣬���û���������
      buf_idx = 0;
      break;
      
    case U8X8_MSG_BYTE_END_TRANSFER:   // �������䣬ִ��ʵ�ʵ�I2Cд��
      // ����I2C��������
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      
      // ����λ
      i2c_master_start(cmd);
      
      // �ӻ���ַ + д��־
      i2c_master_write_byte(cmd, (ADDR << 1) | I2C_MASTER_WRITE, true);
      
      // д�����ݻ�����
      if (buf_idx > 0) {
        i2c_master_write(cmd, buffer, buf_idx, true);
      }
      
      // ֹͣλ
      i2c_master_stop(cmd);
      
      // ִ��I2C����
      esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(1000));
      
      // �ͷ���������
      i2c_cmd_link_delete(cmd);
      
      // ��鴫���Ƿ�ɹ�
      if (ret != ESP_OK) {
        return 0; // ʧ��
      }
      break;
      
    default:
      return 0; // δ֪��Ϣ���ͣ�ʧ��
  }
  return 1; // �ɹ�
}