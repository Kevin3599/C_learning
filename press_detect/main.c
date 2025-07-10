#include <stdio.h>
#include <signal.h>
#include "press.h"

// 全局变量控制程序运行
static int running = 1;

// 信号处理函数
void signal_handler(int sig) {
    printf("\n程序退出\n");
    running = 0;
}

int main() {
    // 注册信号处理函数
    signal(SIGINT, signal_handler);
    
    // 初始化ADC设备
    const char* test_adc_path = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
    
    if (adc_init(test_adc_path) != 0) {
        printf("ADC初始化失败\n");
        return -1;
    }
    
    // 检查并显示当前ADC状态
    int adc_val = adc_read_raw();
    printf("当前ADC状态: %d\n", adc_val);
    
    // 如果有按键按下，先等待释放
    if (adc_val <= 900) {
        printf("请先释放按键\n");
        wait_for_key_release();
        printf("可以开始检测\n");
    }
    
    // 主循环 - 检测按键
    while (running) {
        int pressed_key = wait_for_key_press();
        if (pressed_key > 0 && running) {
            simple_key_detect_and_print(pressed_key);
        }
    }
    
    cleanup_adc();
    return 0;
}
