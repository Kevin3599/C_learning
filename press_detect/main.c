#include <stdio.h>
#include "press.h"

int main() {
    printf("开始测试ADC按键检测\n");
    
    // 测试ADC读取功能
    const char* test_adc_path = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";
    
    // 检测按键
    int key = detect_key_press(test_adc_path);
    if (key > 0) {
        printf("检测到按键%d被按下!\n", key);
    } else if (key == 0) {
        printf("没有按键被按下\n");
    } else {
        printf("ADC设备访问失败\n");
    }
    
    // 清理资源
    cleanup_adc();
    
    return 0;
}