#include "../include/food.h"

// 创建食物节点
FoodNode* create_food(char* name, float vol, int temp, FoodType type) {
    FoodNode* food = (FoodNode*)malloc(sizeof(FoodNode));
    if (food == NULL) return NULL;
    
    // 安全复制名称
    strncpy(food->name, name, 49);
    food->name[49] = '\0'; // 确保结束符
    
    food->volume = vol;
    food->temperature = temp;
    food->type = type;
    food->next = NULL;
    return food;
}

// 释放食物内存
void free_food(FoodNode* food) {
    if (food != NULL) {
        free(food);
    }
}

// 获取种类名称
const char* get_type_name(FoodType type) {
    switch(type) {
        case VEGETABLE: return "蔬菜";
        case MEAT: return "肉类"; 
        case FRUIT: return "水果";
        default: return "未知";
    }
}
