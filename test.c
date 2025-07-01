#include <stdio.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, ""); // 设置本地化，支持中文
    char line[100];
    int len = 0;

    printf("请输入一行文本：");
    fgets(line, sizeof(line), stdin);  // 从用户输入读取一行，包括空格

    // 计算长度（直到遇到 '\0'）
    while (line[len] != '\0') {
        len++;
    }

    printf("你输入的内容是（逐字符打印）：\n");
    for (int i = 0; i < len; i++) {
        putchar(line[i]);
    }

    return 0;
}
