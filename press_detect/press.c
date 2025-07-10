#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>  
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
    
    // 静默初始化，不打印成功信息
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
        // 静默清理，不打印信息
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
        
        // 添加一个"无按键"的范围检测
        if (adc_val > 900) {  // 1023附近表示没有按键按下
            printf("按键未触发, ADC: %d\n", adc_val);
            press_flag = 0;
            return 0;
        }
        else if (adc_val < 50) {  // key4 ADC=9左右
            printf("key4 ADC: %d\n", adc_val);
            press_flag = 1;
            return 4;
        } 
        else if (adc_val >= 200 && adc_val < 300) {  // key5 ADC=235左右
            printf("key5 ADC: %d\n", adc_val);
            press_flag = 1;
            return 5;
        } 
        else if (adc_val >= 400 && adc_val < 500) {  // key6 ADC=457左右
            printf("key6 ADC: %d\n", adc_val);
            press_flag = 1;
            return 6;
        } 
        else if (adc_val >= 650 && adc_val < 750) {  // key7 ADC=683左右
            printf("key7 ADC: %d\n", adc_val);
            press_flag = 1;
            return 7;
        } 
        else {
            printf("ADC读取错误, ADC: %d\n", adc_val);
            press_flag = 0;
            return 0;
        }
    }
    return -1;
}

// 辅助函数：根据ADC值判断按键编号（不打印信息）
int detect_key_from_adc(int adc_val) {
    if (adc_val > 900) {
        return 0;  // 无按键
    } else if (adc_val < 50) {
        return 4;  // key4
    } else if (adc_val >= 200 && adc_val < 300) {
        return 5;  // key5
    } else if (adc_val >= 400 && adc_val < 500) {
        return 6;  // key6
    } else if (adc_val >= 650 && adc_val < 750) {
        return 7;  // key7
    } else {
        return 0;  // 异常值当作无按键
    }
}

// 等待按键按下的函数（简单轮询方式）
int wait_for_key_press(void) {
    // 静默等待，不打印等待信息
    
    while (1) {
        int adc_val = adc_read_raw();
        if (adc_val < 0) {
            printf("ADC读取失败\n");
            return -1;
        }
        
        // 检查是否有按键被按下（不是1023）
        if (adc_val <= 900) {  // 有按键按下
            int key = detect_key_from_adc(adc_val);
            if (key > 0) {
                // 只输出按键编号，不输出ADC值
                press_flag = 1;
                return key;
            }
        }
        
        usleep(10000);  // 10ms延时，避免占用太多CPU
    }
}

// 等待按键释放的函数
void wait_for_key_release(void) {
    // 静默等待释放，不打印等待信息

    // 记录按键按下的开始时间
    struct timespec start_time, current_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    int long_press_detected = 0;  // 长按检测标志
    const long LONG_PRESS_TIME_MS = 1000;  // 1秒 = 1000毫秒
    
    while (1) {
        int adc_val = adc_read_raw();
        
        // 获取当前时间
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        
        // 计算按键按下的持续时间（毫秒）
        long press_duration_ms = (current_time.tv_sec - start_time.tv_sec) * 1000 + 
                                 (current_time.tv_nsec - start_time.tv_nsec) / 1000000;
        
        // 检查是否达到长按时间且还未检测过长按
        if (press_duration_ms >= LONG_PRESS_TIME_MS && !long_press_detected) {
            // 静默检测长按，不在这里打印
            long_press_detected = 1;  // 标记已检测到长按
        }
        
        if (adc_val > 900) {  // 按键已释放
            // 只输出最终结果：按键编号 + 长按/短按
            if (long_press_detected) {
                printf("长按\n");
            } else {
                printf("短按\n");
            }
            
            press_flag = 0;
            break;
        }
        
        usleep(10000);  // 10ms延时
    }
}

// 简洁的按键检测函数 - 只输出按键编号和长短按
void simple_key_detect_and_print(int key) {
    printf("key%d ", key);  // 输出按键编号，不换行
    wait_for_key_release();  // 这个函数会输出"长按"或"短按"并换行
}

