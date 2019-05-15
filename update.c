#include <stdio.h>
#include "cmsis_os2.h"
#include "rtx_os.h"
#include "update.h"
#include "tfm_gpio_veneers.h"
#include "Driver_USART.h"
#include "target_cfg.h"
#include <string.h>

#define CURRENT_INTERFACE_VERSION 0x03
#define PAYLOAD_MAX_LEN 0x400

extern ARM_DRIVER_USART NS_DRIVER_STDIO;

uint8_t update_code_buffer[200];
uint8_t update_data_buffer[50];

osEventFlagsId_t uart_rx;
osEventFlagsId_t trigger_update_event;

/*
struct high_priority_info {
    void *old_task_addr; 
    void *sync_code_addr; 
    void *updated_addr; 
} hp_info;
*/

uint8_t red = 2;

void toggle_LED(uint8_t pin) {
    tfm_gpio_set(pin);
    osDelay(50);
    tfm_gpio_clear(pin);
}

void update_uart_cb(uint32_t event) {
    uint32_t mask = ARM_USART_EVENT_RECEIVE_COMPLETE;
    if (event & mask) {
        osEventFlagsSet(uart_rx, 0x01);
    }
}

void update_process_command(uint8_t command, uint8_t len, uint8_t *payload) {

    if (command == 0x0) { // Ping
        toggle_LED(red);
    } else if (command == 0x1) { // Update
        payload += 2; // skip padding bytes
        //uint8_t *update_addr = (uint8_t *)(((uint32_t *)payload)[0]);
        payload += 4; // skip to update binary
        len -= 4;

        memcpy(update_code_buffer, payload, len);

        update_data_buffer[0] = *((uint8_t *)0x20013380);
        // signal high priority task
        osEventFlagsSet(trigger_update_event, 0x01);
    } else {
        toggle_LED(red);
    }
}

static osThreadAttr_t new_blink_attr = {
    .name = "newblink",
    .stack_size = 1024U,
    .priority = osPriorityHigh,
};

struct update_args {
    osThreadId_t blink_id;
    uint64_t *blink_stack;
};
struct update_args *ua;


__attribute__((noreturn))
void update_high_priority(void *arg) {

    trigger_update_event = osEventFlagsNew(NULL);

    while (1) {
        osEventFlagsWait(trigger_update_event, 0x01, osFlagsWaitAny, osWaitForever);

        // hardcoded hw init
        tfm_gpio_disable(4); 
        tfm_gpio_enable_output(3); 
        tfm_gpio_clear(3);

        // modify tasks
        osThreadTerminate(ua->blink_id);
        osThreadNew((osThreadFunc_t) update_code_buffer, NULL, &new_blink_attr);
    }
}

__attribute__((noreturn))
void update_low_priority(void *arg) {

    uart_rx = osEventFlagsNew(NULL);

    ua = (struct update_args *)arg;
    new_blink_attr.stack_mem = ua->blink_stack;

    tfm_gpio_enable_output(red);
    tfm_gpio_clear(red);

    while (1) {
        uint16_t header[3]; 
        NS_DRIVER_STDIO.Receive(&header, sizeof(uint16_t) * 3);
        osEventFlagsWait(uart_rx, 0x01, osFlagsWaitAny, osWaitForever);

        uint16_t version = header[0];
        uint16_t command = header[1];
        uint16_t len     = header[2];

        if (version != CURRENT_INTERFACE_VERSION) {
            printf("Incorrect interface version! Expected %x, got %x\n", CURRENT_INTERFACE_VERSION, version);
            toggle_LED(red);
            continue;
        }

        if (len > PAYLOAD_MAX_LEN) {
            printf("Commmand payload too large! Maximum %x bytes, header indicates %x bytes\n", PAYLOAD_MAX_LEN, len);
            toggle_LED(red);
            continue;
        }

        uint8_t payload[PAYLOAD_MAX_LEN];
        NS_DRIVER_STDIO.Receive(&payload, len);
        osEventFlagsWait(uart_rx, 0x01, osFlagsWaitAny, osWaitForever);
        update_process_command(command, len, (uint8_t *) &payload);
    }
}
