/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include <string.h>

#include "freertos/queue.h"
#include "driver/timer.h"
#include "esp_clk_tree.h"
#include <rom/ets_sys.h>


#include "soft_serial_rmt.h"



#define RMT_TX_GPIO (23)
#define RMT_RX_GPIO (22)

static const char *TAG = "RMT TEST";

mdb_item16_t send_data[] = 
{
    {.cmd=1,.data=0xaa},
    {.cmd=0,.data=0x1},
    {.cmd=0,.data=0x10},
    {.cmd=0,.data=0xff},
    {.cmd=1,.data=0xff},
    {.cmd=0,.data=0x00},
    {.cmd=0,.data=0x0f},
    {.cmd=0,.data=0xf0},
    {.cmd=0,.data=0x30},
    {.cmd=0,.data=0x03},
    {.cmd=0,.data=0xe0},
    {.cmd=0,.data=0x0e},
    {.cmd=1,.data=0xc0},
    {.cmd=0,.data=0x0c},
    {.cmd=1,.data=0xcc},
    {.cmd=0,.data=0xbb},
    {.cmd=0,.data=0x33},
    {.cmd=1,.data=0x77},
    {.cmd=0,.data=0x88},
    {.cmd=0,.data=0x11},
    {.cmd=1,.data=0x0}

};


void app_main(void)

{

    #if DBG
// dbg logic analyzer
#include "logic_analyzer_ws_server.h"

    logic_analyzer_ws_server();
//
//  dbg GPIO PIN
    gpio_reset_pin(TX_TEST_GPIO);
    gpio_set_direction(TX_TEST_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(RX_TEST_GPIO);
    gpio_set_direction(RX_TEST_GPIO, GPIO_MODE_OUTPUT);
//
    #endif
    soft_serial_init(RMT_RX_GPIO, RMT_TX_GPIO); 

    mdb_item16_t data[64];
    int cnt = 0;
    int icnt=0;
    int err_cnt = 0;    
    while(1){
        vTaskDelay(10);
        #if DBG
        gpio_set_level(TX_TEST_GPIO,1);
        #endif
        soft_serial_write_data(send_data,sizeof(send_data)/sizeof(mdb_item16_t),portMAX_DELAY);
        soft_serial_read_data(data,sizeof(send_data)/sizeof(mdb_item16_t),portMAX_DELAY);
        #if DBG
        gpio_set_level(TX_TEST_GPIO,0);
        #endif
       if(memcmp((void*)send_data,(void*)data,sizeof(send_data)))
            {ESP_LOGE(TAG,"ERROR %d",err_cnt);}
       memset((void*)data,0,sizeof(send_data));
       cnt++;
       if( cnt >= 10)
            {ESP_LOGI(TAG,"Send/Receive %d packet icnt=%d",cnt, icnt++); cnt=0;}
    }

}
