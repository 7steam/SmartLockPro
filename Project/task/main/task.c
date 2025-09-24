#include <stdio.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <string.h>
/*
int uart_buffer_size = (1024 * 2);
QueueHandle_t uart_queue;
 uart_port_t uart_num = UART_NUM_1;
 uint8_t data[128];
int length = 0;
*/

#define gp_4  4
#define gp_5  5
#define gp_2  2
#define gp_3  3
#define gp_6  6
#define gp_7  7
#define gp_8  8
#define gp_9  9
#define gp_10  10
#define gp_11  11


//void re(void *pvParam);

void app_main(void)
{




    gpio_config_t led_conf;
    memset(&led_conf,0,sizeof(led_conf));
   led_conf.mode = GPIO_MODE_OUTPUT;
   led_conf.pin_bit_mask = (1<<gp_2)|(1<<gp_3)|(1<<gp_4)|(1<<gp_5);
   led_conf.pull_down_en = 0; 
   esp_err_t r = gpio_config(&led_conf);





   gpio_set_level(gp_2,1);
   gpio_set_level(gp_3,1);
   gpio_set_level(gp_4,1);

/*
uart_config_t uart_config = {
    .baud_rate = 115200,         //波特率
    .data_bits = UART_DATA_8_BITS,  //数据位数
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,   //停止位
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    
};


uart_param_config(UART_NUM_1, &uart_config);
uart_set_pin(UART_NUM_1, 4, 5, 18, 19);

uart_driver_install(UART_NUM_1, uart_buffer_size, uart_buffer_size, 10, &uart_queue, 0);

   
xTaskCreate(re,"uart_1",10000,NULL,1,NULL );

*/
}


/*
void re(void *pvParam){
   
while(1)
{
    length = uart_read_bytes(uart_num, data, 12, 100);
    //printf("%d\n",data[0]);
    
     printf("%d\n",data[0]);
     printf("%d\n",data[1]);
     printf("%d\n",length);
    // printf("%d\n",data[2]);
    vTaskDelay(50/ portTICK_PERIOD_MS);
}


}*/