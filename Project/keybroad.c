#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

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
#define motor4 8

void app_main(void)
{

   // 电机gpio设置
   gpio_config_t motor_conf;

   // memset(&led_conf,0,sizeof(led_conf));

   motor_conf.mode = GPIO_MODE_OUTPUT;
   motor_conf.pin_bit_mask = (1 << motor1) | (1 << motor2) | (1 << motor3) | (1 << motor4);
   // led_conf.pull_down_en = 1;
   esp_err_t r = gpio_config(&motor_conf);

   // 电机四根信号线全部输出高电平
   gpio_set_level(motor1, 1);
   gpio_set_level(motor2, 1);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor4, 1);

   int t, k, s, c, j, u = 0;

   void motor_left();
   void motor_right();
   void motor_over();

   while (1)
   {

      // vTaskDelay(200 / portTICK_PERIOD_MS);

   t4:

      // 键盘gpio设置
      gpio_config_t led_conf;

      memset(&led_conf, 0, sizeof(led_conf));

      led_conf.mode = GPIO_MODE_INPUT;
      led_conf.pin_bit_mask = (1 << key_r1) | (1 << key_r2) | (1 << key_r3) | (1 << key_r4);
      led_conf.pull_down_en = 1;

      //放在while中对gpio进行频繁的配置是不可取的 ， 会拖慢速度造成gpio状态不稳定
      // 最好移到while外面，一次性配置好所有gpio的属性。
      esp_err_t r = gpio_config(&led_conf);
      // printf("r = %d\n",r);

      gpio_config_t led_conf1;

      led_conf1.mode = GPIO_MODE_OUTPUT;
      led_conf1.pin_bit_mask = (1 << key_c1) | (1 << key_c2) | (1 << key_c3) | (1 << key_c4);
      // led_conf1.pull_down_en = 1;
      esp_err_t m = gpio_config(&led_conf1);
      // printf("m = %d\n",m);

      gpio_set_level(key_c1, 1);
      gpio_set_level(key_c2, 1);
      gpio_set_level(key_c3, 1);
      gpio_set_level(key_c4, 1);

      k = gpio_get_level(key_r1);
      s = gpio_get_level(key_r2);
      c = gpio_get_level(key_r3);
      j = gpio_get_level(key_r4);

      //接下来的4个if语句代码高度相似，只是某些数据不同，可以考虑
      //封装成一个函数，然后通过参数进行控制。可以很大程度上减少代码量。

      // 扫描第一行
      if (k == 1)
      {

         gpio_config_t led_conf;
         memset(&led_conf, 0, sizeof(led_conf));
         led_conf.mode = GPIO_MODE_OUTPUT;
         led_conf.pin_bit_mask = (1 << key_r1) | (1 << key_r2) | (1 << key_r3) | (1 << key_r4);
         led_conf.pull_down_en = 0;
         esp_err_t r = gpio_config(&led_conf);
         // printf("r = %d\n",r);

         gpio_config_t led_conf1;
         led_conf1.mode = GPIO_MODE_INPUT;
         led_conf1.pin_bit_mask = (1 << key_c1) | (1 << key_c2) | (1 << key_c3) | (1 << key_c4);
         led_conf1.pull_down_en = 1;
         esp_err_t m = gpio_config(&led_conf1);
         // printf("m = %d\n",m);

         gpio_set_level(key_r1, 1);
         t = gpio_get_level(key_c1);
         if (t == 1)
         {
            printf("1\n");
            continue;
         }
         t = gpio_get_level(key_c2);
         if (t == 1)
         {
            printf("2\n");
            continue;
         }
         t = gpio_get_level(key_c3);
         if (t == 1)
         {
            printf("3\n");
            continue;
         }
         t = gpio_get_level(key_c4);
         if (t == 1)
         {
            printf("4\n");
            continue;
         }
      }

      // 扫描第二行
      if (s == 1)
      {
         gpio_config_t led_conf;
         memset(&led_conf, 0, sizeof(led_conf));
         led_conf.mode = GPIO_MODE_OUTPUT;
         led_conf.pin_bit_mask = (1 << key_r1) | (1 << key_r2) | (1 << key_r3) | (1 << key_r4);
         led_conf.pull_down_en = 0;
         esp_err_t r = gpio_config(&led_conf);
         // printf("r = %d\n",r);

         gpio_config_t led_conf1;
         led_conf1.mode = GPIO_MODE_INPUT;
         led_conf1.pin_bit_mask = (1 << key_c1) | (1 << key_c2) | (1 << key_c3) | (1 << key_c4);
         led_conf1.pull_down_en = 1;
         esp_err_t m = gpio_config(&led_conf1);
         // printf("m = %d\n",m);

         gpio_set_level(key_r2, 1);
         t = gpio_get_level(key_c1);
         if (t == 1)
         {
            printf("5\n");
            u = 5;
         t1:
            //这里的motor_left函数，还有其他的motor函数，实际上会造成一些问题的。
            //这个函数实际上是一个阻塞函数，就是说执行到这里的时候就不能干别的事情
            //了，会导致比如键盘检测功能拖慢，造成不灵敏。程序规模大了以后，造成扩展困难的问题。
            // 以后要学会至少是使用多线程来控制，然后两个线程直接进行通信。
            motor_left();   
            goto t4;
            continue;
         }
         t = gpio_get_level(key_c2);
         if (t == 1)
         {
            printf("6\n");
            u = 6;
         t2:
            motor_right();
            goto t4;
            continue;
         }
         t = gpio_get_level(key_c3);
         if (t == 1)
         {
            printf("7\n");
            u = 7;
         t3:
            motor_over();
            goto t4;
            continue;
         }
         t = gpio_get_level(key_c4);
         if (t == 1)
         {
            printf("8\n");
            continue;
         }
      }

      // 扫描第三行
      if (c == 1)
      {
         gpio_config_t led_conf;
         memset(&led_conf, 0, sizeof(led_conf));
         led_conf.mode = GPIO_MODE_OUTPUT;
         led_conf.pin_bit_mask = (1 << key_r1) | (1 << key_r2) | (1 << key_r3) | (1 << key_r4);
         led_conf.pull_down_en = 0;
         esp_err_t r = gpio_config(&led_conf);
         // printf("r = %d\n",r);

         gpio_config_t led_conf1;
         led_conf1.mode = GPIO_MODE_INPUT;
         led_conf1.pin_bit_mask = (1 << key_c1) | (1 << key_c2) | (1 << key_c3) | (1 << key_c4);
         led_conf1.pull_down_en = 1;
         esp_err_t m = gpio_config(&led_conf1);
         // printf("m = %d\n",m);

         gpio_set_level(key_r3, 1);
         t = gpio_get_level(key_c1);
         if (t == 1)
         {
            printf("9\n");
            continue;
         }
         t = gpio_get_level(key_c2);
         if (t == 1)
         {
            printf("10\n");
            continue;
         }
         t = gpio_get_level(key_c3);
         if (t == 1)
         {
            printf("11\n");
            continue;
         }
         t = gpio_get_level(key_c4);
         if (t == 1)
         {
            printf("12\n");
            continue;
         }
      }

      // 扫描第四行
      if (j == 1)
      {
         gpio_config_t led_conf;
         memset(&led_conf, 0, sizeof(led_conf));
         led_conf.mode = GPIO_MODE_OUTPUT;
         led_conf.pin_bit_mask = (1 << key_r1) | (1 << key_r2) | (1 << key_r3) | (1 << key_r4);
         led_conf.pull_down_en = 0;
         esp_err_t r = gpio_config(&led_conf);
         // printf("r = %d\n",r);

         gpio_config_t led_conf1;
         led_conf1.mode = GPIO_MODE_INPUT;
         led_conf1.pin_bit_mask = (1 << key_c1) | (1 << key_c2) | (1 << key_c3) | (1 << key_c4);
         led_conf1.pull_down_en = 1;
         esp_err_t m = gpio_config(&led_conf1);
         // printf("m = %d\n",m);

         gpio_set_level(key_r4, 1);
         t = gpio_get_level(key_c1);
         if (t == 1)
         {
            printf("13\n");
            continue;
         }
         t = gpio_get_level(key_c2);
         if (t == 1)
         {
            printf("   14\n");
            continue;
         }
         t = gpio_get_level(key_c3);
         if (t == 1)
         {
            printf("15\n");
            continue;
         }
         t = gpio_get_level(key_c4);
         if (t == 1)
         {
            printf("16\n");
            continue;
         }
      }

      vTaskDelay(50 / portTICK_PERIOD_MS);
      if (u == 5)
         goto t1;
      if (u == 6)
         goto t2;
      if (u == 7)
         goto t3;
   }
}

void motor_left()
{

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

void motor_right()
{

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
}

void motor_over()
{
   gpio_set_level(motor1, 1);
   gpio_set_level(motor2, 2);
   gpio_set_level(motor3, 1);
   gpio_set_level(motor4, 1);
}






void key( ){

 gpio_config_t led_conf;
        
         led_conf.mode = GPIO_MODE_OUTPUT;
         led_conf.pin_bit_mask = (1 << key_r1) | (1 << key_r2) | (1 << key_r3) | (1 << key_r4);
         led_conf.pull_down_en = 0;
         esp_err_t r = gpio_config(&led_conf);
        

         gpio_config_t led_conf1;
         led_conf1.mode = GPIO_MODE_INPUT;
         led_conf1.pin_bit_mask = (1 << key_c1) | (1 << key_c2) | (1 << key_c3) | (1 << key_c4);
         led_conf1.pull_down_en = 1;
         esp_err_t m = gpio_config(&led_conf1);





          gpio_set_level(key_r1, 1);
         t = gpio_get_level(key_c1);
         if (t == 1)
         {
            printf("1\n");
            continue;
         }
         t = gpio_get_level(key_c2);
         if (t == 1)
         {
            printf("2\n");
            continue;
         }
         t = gpio_get_level(key_c3);
         if (t == 1)
         {
            printf("3\n");
            continue;
         }
         t = gpio_get_level(key_c4);
         if (t == 1)
         {
            printf("4\n");
            continue;
         }



}