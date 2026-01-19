// 用户界面函数实现

#include "../include/ui.h"
#include <string.h>
#include <ctype.h>

// 清屏
void clear_screen() {
    system("cls"); // Windows系统清屏
}

// 暂停
void pause_screen() {
    printf("\n\t\t[按回车键继续...]");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 清空输入缓存
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 获取有效的整数输入
int get_valid_int(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            // 检查范围
            if (value >= min && value <= max) {
                clear_input_buffer(); 
                return value;
            } else {
                printf("\t\t[错误] 输入超出范围 (%d - %d)，请重试。\n", min, max);
            }
        } else {
            printf("\t\t[错误] 输入无效，请输入数字。\n");
            clear_input_buffer(); 
        }
    }
}

// 获取有效的浮点数输入
float get_valid_float(const char* prompt, float min, float max) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1) {
            if (value >= min && value <= max) {
                clear_input_buffer();
                return value;
            } else {
                printf("\t\t[错误] 输入超出范围 (%.2f - %.2f)，请重试。\n", min, max);
            }
        } else {
            printf("\t\t[错误] 输入无效，请输入数字。\n");
            clear_input_buffer();
        }
    }
}

// 安全获取字符串输入
void get_valid_string(const char* prompt, char* buffer, int max_len) {
    printf("%s", prompt);
    char temp[1024]; 
    if (fgets(temp, sizeof(temp), stdin) != NULL) {
        size_t len = strlen(temp);
        if (len > 0 && temp[len-1] == '\n') {
            temp[len-1] = '\0';
        } else {
            clear_input_buffer();
        }
        if (strlen(temp) > (size_t)max_len - 1) {
            temp[max_len - 1] = '\0'; 
            printf("\t\t[警告] 输入过长，已自动截断为: %s\n", temp);
        }
        strcpy(buffer, temp);
    } else {
        buffer[0] = '\0'; 
    }
    while (strlen(buffer) == 0) {
        printf("\t\t[错误] 输入不能为空，请重试: ");
        if (fgets(temp, sizeof(temp), stdin) != NULL) {
            size_t len = strlen(temp);
            if (len > 0 && temp[len-1] == '\n') temp[len-1] = '\0';
            else clear_input_buffer();
            if (strlen(temp) > (size_t)max_len - 1) temp[max_len - 1] = '\0';
            strcpy(buffer, temp);
        }
    }
}

// 主菜单
void show_menu() {
    printf("\n\t\t+====================================================+\n");
    printf("\t\t|                智能冰箱管理系统                    |\n");
    printf("\t\t+====================================================+\n");
    printf("\t\t|  [1] 显示冰箱状态    [2] 存入食物                  |\n");
    printf("\t\t|  [3] 取出食物        [4] 按种类查询                |\n");
    printf("\t\t|  [5] 修改食物信息    [6] 按体积排序                |\n");
    printf("\t\t|  [7] 统计信息        [8] 保存数据                  |\n");
    printf("\t\t|  [9] 清空冰箱        [0] 退出系统                  |\n");
    printf("\t\t+====================================================+\n");
    printf("\t\t请输入选项(0-9): ");
}

// 获取食物类型
FoodType get_food_type() {
    printf("\n\t\t请选择食物种类:\n");
    printf("\t\t[1] 蔬菜  [2] 肉类  [3] 水果\n");
    int choice = get_valid_int("\t\t请输入选项(1-3): ", 1, 3);
    return (FoodType)choice;
}

// 输入食物信息
FoodNode* input_food() {
    clear_screen(); // 进入输入界面清屏
    printf("\n\t\t+============== 存入食物 ==============+\n");
    
    char name[50];
    get_valid_string("\t\t请输入食物名称: ", name, 50);
    
    float volume = get_valid_float("\t\t请输入体积(0.1-100): ", 0.1, 100.0);
    int temperature = get_valid_int("\t\t请输入储存温度(-20到10): ", -20, 10);
    FoodType type = get_food_type();
    
    return create_food(name, volume, temperature, type);
}
