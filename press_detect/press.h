#ifndef __PRESS_H
#define __PRESS_H

// ADC相关函数声明
int adc_init(const char* adc_path);
int adc_read_raw(void);
int get_current_adc_value(void);
void cleanup_adc(void);
void debug_print_adc(void);
int detect_key_press(const char* adc_path);
int detect_key_from_adc(int adc_val);
int wait_for_key_press(void);
void wait_for_key_release(void);
void simple_key_detect_and_print(int key);

// 常用的RK系列ADC设备路径
//#define RK3588_ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
//#define RK3568_ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
//#define RK3399_ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

#endif