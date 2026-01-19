#ifndef UI_H
#define UI_H

#include <stdio.h>
#include <stdlib.h>
#include "food.h"

// 界面函数
void clear_screen();
void pause_screen();
void clear_input_buffer();

// 输入辅助函数
int get_valid_int(const char* prompt, int min, int max);
float get_valid_float(const char* prompt, float min, float max);
void get_valid_string(const char* prompt, char* buffer, int max_len);

// 业务界面
void show_menu();
FoodType get_food_type();
FoodNode* input_food();

#endif
