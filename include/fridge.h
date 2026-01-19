#ifndef FRIDGE_H
#define FRIDGE_H

#include "food.h"

// 冰箱结构体
typedef struct {
    float capacity;      // 总容积
    float used_volume;   // 已用容积
    int temperature;     // 当前温度
    int min_temp;        // 最小温度
    int max_temp;        // 最大温度
    FoodNode* food_list; // 食物链表
} Fridge;

// 函数声明
void init_fridge(Fridge* fridge);
int add_food(Fridge* fridge, FoodNode* food);
int remove_food(Fridge* fridge, char* name);
FoodNode* find_food(Fridge* fridge, char* name);
void search_by_type(Fridge* fridge, FoodType type);
int modify_food(Fridge* fridge, char* name, float new_volume, int new_temp);
void update_temperature(Fridge* fridge);
void sort_by_volume(Fridge* fridge);
void display_fridge(Fridge* fridge);
void clear_fridge(Fridge* fridge);
void show_statistics(Fridge* fridge);
int get_food_count(Fridge* fridge);

#endif
