#include <stdio.h>
#include <stdlib.h>

int main() {
    char firmware[128];
    char device_id[64];

    printf("请输入固件文件路径（如 klipper.bin）：");
    scanf("%127s", firmware);

    printf("请输入设备ID（如 0483:df11）：");
    scanf("%63s", device_id);

    char cmd[256];
    sprintf(cmd, "dfu-util -a 0 -D %s -d %s", firmware, device_id);

    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("无法运行 dfu-util，请检查是否已安装。\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    pclose(fp);

    printf("下载完成（请根据输出判断是否成功）\n");
    return 0;
}