#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // 检查DFU设备是否存在
    FILE *check = popen("dfu-util -l", "r");
    if (!check) {
        printf("无法运行 dfu-util,请检查是否已安装。\n");
        return 1;
    }
    char buf[512];
    int found = 0;
    while (fgets(buf, sizeof(buf), check)) {
        if (strstr(buf, "Found DFU")) {
            found = 1;
            break;
        }
    }
    pclose(check);
    if (!found) {
        printf("未检测到DFU设备,请确认设备已进入DFU模式后再试!\n");
        return 1;
    }

    char firmware[128];
    char device_id[64];

    printf("请输入固件文件路径（如 klipper.bin):");
    scanf("%127s", firmware);

    printf("请输入设备ID(如 0483:df11):");
    scanf("%63s", device_id);

    char cmd[256];
    // 指定写入起始地址为 0x08000000，并在烧录后自动重启
    sprintf(cmd, "dfu-util -a 0 --dfuse-address 0x08000000:leave -D %s -d %s", firmware, device_id);

    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("无法运行 dfu-util,请检查是否已安装。\n");
        return 1;
    }

    char line[256];
    int pos = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\r' || ch == '\n') {
            if (pos > 0) {
                line[pos] = '\0';
                if (strncmp(line, "Download", 8) == 0) {
                    printf("\r%s", line);
                    fflush(stdout);
                }
                // 检查是否为 Download done.
                if (strstr(line, "Download done.") != NULL) {
                    printf("\n");
                }
                pos = 0;
            }
        } else {
            if (pos < sizeof(line) - 1) {
                line[pos++] = ch;
            }
        }
    }
    // 循环结束后，如果最后一次是进度条，补一个换行
    if (pos > 0 && strncmp(line, "Download", 8) == 0) {
        printf("\n");
    }

    pclose(fp);

    printf("下载完成（请根据输出判断是否成功）\n");
    return 0;
}