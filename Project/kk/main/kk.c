#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include <pthread.h>
#define motor1 7



void pp(void *pvParameters);
void pk(void *pvParameters);
void pr(int *point,gpio_num_t gpio_num);


void pr(int *point,gpio_num_t gpio_num){
    printf("%d\n",*point);
    printf("%d\n",*(point+1));
    gpio_set_level(gpio_num, 1);
}


int tt[3]={1,2,3};

void pp(void *pvParam){
    int *pArrayAddr;
    pArrayAddr=(int *)pvParam;
    while (1){
       pr(pArrayAddr+1,motor1);
     vTaskDelay(10 / portTICK_PERIOD_MS); 

    }
   
    
    
     
}



/*void pk(void *pvParameters){
    
   while(1){
    printf("1111111111\n");
    vTaskDelay(10 / portTICK_PERIOD_MS);  
    }
}*/


/*#define gg1 9
#define gg2 4*/


/*void yu(gpio_num_t r){
int k=gpio_get_level(r);
printf("11111\n");
printf("%d\n",k);
printf("11111\n");
}*/







void app_main(void)
{


gpio_config_t motor_conf;
  
  
   motor_conf.mode = GPIO_MODE_OUTPUT;
   motor_conf.pin_bit_mask = (1<<motor1);
   
   esp_err_t r = gpio_config(&motor_conf);

//TaskHandle_t pxTask = NULL;


xTaskCreate(
    pp,
    "task 2",
    10000,
    (void *)&tt,
    1,
    NULL
 );

/*xTaskCreate(
    pp,
    "task 1",
    10000,
    NULL,
    0,
    &pxTask
 );*/

 

 //vTaskStartScheduler();

 //for (;;);













/*pthread_t th1;
pthread_t th2;
pthread_create(&th1,NULL,pp,NULL);
pthread_create(&th2,NULL,pp,NULL);
pthread_join(th1,NULL);
pthread_join(th2,NULL);   */ 




}
