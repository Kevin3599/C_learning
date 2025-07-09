#include <stdio.h>
#include <string.h>

// 例子1：为基本数据类型创建别名
typedef int Integer;
typedef float Real;
typedef char Character;

// 例子2：为数组类型创建别名
typedef int IntArray[10];
typedef char String[100];

// 例子3：为指针类型创建别名
typedef int* IntPtr;
typedef char* CharPtr;

// 例子4：为结构体创建别名
typedef struct {
    char name[50];
    int age;
    float height;
} Person;

// 例子5：为结构体指针创建别名
typedef Person* PersonPtr;

// 例子6：为函数指针创建别名
typedef int (*MathFunc)(int, int);

// 用于函数指针示例的函数
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

// 例子7：为枚举创建别名
typedef enum {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Weekday;

int main() {
    printf("=== typedef 使用示例 ===\n\n");

    // 例子1：基本数据类型别名
    printf("1. 基本数据类型别名:\n");
    Integer num = 42;
    Real pi = 3.14f;
    Character grade = 'A';
    printf("   Integer num = %d\n", num);
    printf("   Real pi = %.2f\n", pi);
    printf("   Character grade = %c\n\n", grade);

    // 例子2：数组类型别名
    printf("2. 数组类型别名:\n");
    IntArray numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    String message = "Hello, typedef!";
    printf("   IntArray: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("...\n");
    printf("   String: %s\n\n", message);

    // 例子3：指针类型别名
    printf("3. 指针类型别名:\n");
    int value = 100;
    IntPtr ptr = &value;
    CharPtr str = "typedef指针";
    printf("   IntPtr指向的值: %d\n", *ptr);
    printf("   CharPtr指向的字符串: %s\n\n", str);

    // 例子4和5：结构体和结构体指针别名
    printf("4. 结构体别名:\n");
    Person student = {"张三", 20, 175.5f};
    PersonPtr studentPtr = &student;
    printf("   Person结构体: 姓名=%s, 年龄=%d, 身高=%.1f\n", 
           student.name, student.age, student.height);
    printf("   通过PersonPtr访问: 姓名=%s, 年龄=%d\n\n", 
           studentPtr->name, studentPtr->age);

    // 例子6：函数指针别名
    printf("5. 函数指针别名:\n");
    MathFunc operation;
    
    operation = add;
    printf("   使用add函数: 5 + 3 = %d\n", operation(5, 3));
    
    operation = multiply;
    printf("   使用multiply函数: 5 * 3 = %d\n\n", operation(5, 3));

    // 例子7：枚举别名
    printf("6. 枚举别名:\n");
    Weekday today = FRIDAY;
    printf("   今天是星期: %d (0=周一, 4=周五)\n", today);
    
    switch(today) {
        case MONDAY:
            printf("   今天是周一\n");
            break;
        case FRIDAY:
            printf("   今天是周五，快到周末了！\n");
            break;
        default:
            printf("   其他工作日\n");
    }

    printf("\n=== typedef的优点 ===\n");
    printf("1. 代码更易读：Person比struct {...}更清晰\n");
    printf("2. 类型安全：编译器可以进行类型检查\n");
    printf("3. 易于维护：修改类型定义只需改一处\n");
    printf("4. 简化复杂类型：特别是函数指针和多重指针\n");

    return 0;
}
