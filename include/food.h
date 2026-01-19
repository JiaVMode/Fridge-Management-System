#ifndef FOOD_H
#define FOOD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 食物种类枚举
typedef enum {
    VEGETABLE = 1, // 蔬菜
    MEAT,          // 肉类
    FRUIT          // 水果
} FoodType;

// 食物节点结构体
typedef struct FoodNode {
    char name[50];       // 名称
    float volume;        // 体积
    int temperature;     // 温度
    FoodType type;       // 种类
    struct FoodNode* next; // 下一个节点
} FoodNode;

// 函数声明
FoodNode* create_food(char* name, float vol, int temp, FoodType type);
void free_food(FoodNode* food);
const char* get_type_name(FoodType type); // 种类转字符串

#endif
