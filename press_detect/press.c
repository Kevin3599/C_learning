#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "press.h"

static int press_time = 0;
static int press_flag = 0;
static int adc_fd = -1;
static int current_adc_value = 0;

// ADC初始化函数
int adc_init(const char* adc_path) {
    // 打开ADC设备文件
    adc_fd = open(adc_path, O_RDONLY);
    if (adc_fd < 0) {
        perror("Failed to open ADC device");
        printf("请检查ADC设备路径: %s\n", adc_path);
        return -1;
    }
    
    printf("ADC设备初始化成功: %s\n", adc_path);
    return 0;
}

// 读取ADC原始值
int adc_read_raw(void) {
    char buffer[16];
    int value = 0;
    
    if (adc_fd < 0) {
        printf("ADC设备未初始化\n");
        return -1;
    }
    
    // 重置文件指针到开始位置
    lseek(adc_fd, 0, SEEK_SET);
    
    // 读取ADC值
    if (read(adc_fd, buffer, sizeof(buffer)) > 0) {
        value = atoi(buffer);
        current_adc_value = value;
    } else {
        perror("未读取到ADC值");
        return -1;
    }
    
    return value;
}

// 获取当前ADC值（不重新读取）
int get_current_adc_value(void) {
    return current_adc_value;
}

// 清理ADC资源
void cleanup_adc(void) {
    if (adc_fd >= 0) {
        close(adc_fd);
        adc_fd = -1;
        printf("ADC资源已清理\n");
    }
}

// 调试函数：打印当前ADC值
void debug_print_adc(void) {
    int adc_val = adc_read_raw();
    printf("当前ADC值: %d\n", adc_val);
}

// 通过ADC值判断按键是否被按下的函数
// key4 ADC=9, key5 ADC=235, key6 ADC=457, key7 ADC=683
int detect_key_press(const char* adc_path) {
    if (adc_init(adc_path) == 0) {
        int adc_val = adc_read_raw();
        if (adc_val < 50) {
            printf("key4被按下! ADC值: %d\n", adc_val);
            press_flag = 1;
            return 4;  // 返回按键编号
        } else if (adc_val >= 200 && adc_val < 350) {
            printf("key5被按下! ADC值: %d\n", adc_val);
            press_flag = 1;
            return 5;
        } else if (adc_val >= 400 && adc_val < 550) {
            printf("key6被按下! ADC值: %d\n", adc_val);
            press_flag = 1;
            return 6;
        } else if (adc_val >= 650) {
            printf("key7被按下! ADC值: %d\n", adc_val);
            press_flag = 1;
            return 7;
        } else {
            printf("没有按键被按下, ADC值: %d\n", adc_val);
            press_flag = 0;
            return 0;
        }
    }
    return -1;
}

