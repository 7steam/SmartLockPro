#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"



#define motor1 7
#define motor2 18
#define motor3 12
#define motor4 8


void app_main(void)
{
gpio_config_t motor_conf;
  
    //memset(&led_conf,0,sizeof(led_conf));

   motor_conf.mode = GPIO_MODE_OUTPUT;
   motor_conf.pin_bit_mask = (1<<motor1)|(1<<motor2)|(1<<motor3)|(1<<motor4);
   //led_conf.pull_down_en = 1; 
   esp_err_t r = gpio_config(&motor_conf);


   void motor_left( );
   void motor_right( );

   gpio_set_level(motor1, 1);
   gpio_set_level(motor2, 1);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor4, 1);



    motor_left( );
   motor_right( );



}





void motor_left( ){
   for(int i=0;i<281;i++){
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
}

   
}




void motor_right( ){
for(int i=0;i<281;i++){
   gpio_set_level(motor4, 0);
  vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor4, 1);
   gpio_set_level(motor3, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor2, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor2, 1);
   gpio_set_level(motor2, 0); 
   vTaskDelay(10 / portTICK_PERIOD_MS);
   gpio_set_level(motor2, 1); 
}


}



