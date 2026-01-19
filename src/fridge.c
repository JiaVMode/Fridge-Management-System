// 冰箱操作函数实现

#include "../include/fridge.h"  // 冰箱函数声明
#include "../include/ui.h" // 需要用到UI函数

// 获取字符的字节数和视觉宽度
void get_char_info(const char* s, int* byte_len, int* visual_len) {
    unsigned char c = (unsigned char)*s;
    if (c < 128) {
        *byte_len = 1;
        *visual_len = 1;
    } else {
        *visual_len = 2; // 中文固定2宽度
        if (c >= 0xE0) *byte_len = 3; // UTF-8 3字节
        else if (c >= 0xC0) *byte_len = 2; // UTF-8 2字节
        else *byte_len = 2; // GBK 2字节
    }
}

// 打印列内容（不带截断，只填充空格以对齐）
void print_col(const char* content, int width) {
    int current_visual = 0;
    const char* s = content;
    
    // 计算视觉宽度
    while (*s) {
        int char_byte_len = 0;
        int char_visual_len = 0;
        get_char_info(s, &char_byte_len, &char_visual_len);
        current_visual += char_visual_len;
        s += char_byte_len;
    }
    
    printf("%s", content);
    
    // 补齐空格
    for (int i = 0; i < width - current_visual; i++) {
        printf(" ");
    }
}

// 打印分割线
void print_bar(char c) {
    printf("\t\t");
    for (int i = 0; i < 50; i++) printf("%c", c);
    printf("\n");
}

// 初始化冰箱
void init_fridge(Fridge* fridge) {
    fridge->capacity = 100.0;
    fridge->used_volume = 0.0;
    fridge->temperature = 0;
    fridge->min_temp = -20;
    fridge->max_temp = 10;
    fridge->food_list = NULL;
}

// 添加食物
int add_food(Fridge* fridge, FoodNode* food) {
    if (food->temperature < fridge->min_temp) {
        printf("\t\t[错误] 食物温度(%d℃)低于下限(%d℃)!\n", food->temperature, fridge->min_temp);
        return 0;
    }
    float remaining = fridge->capacity - fridge->used_volume;
    if (food->volume > remaining) {
        printf("\t\t[错误] 食物体积(%.2f)大于剩余容积(%.2f)!\n", food->volume, remaining);
        return 0;
    }
    
    // 检查冲突
    FoodNode* existing = find_food(fridge, food->name);
    if (existing != NULL) {
        existing->volume += food->volume;
        fridge->used_volume += food->volume;
        existing->temperature = food->temperature;
        existing->type = food->type;
        update_temperature(fridge);
        printf("\t\t[成功] 已存在同名食物，体积已合并! (当前总积: %.2f)\n", existing->volume);
        free_food(food);
        return 1;
    }
    
    food->next = fridge->food_list;
    fridge->food_list = food;
    fridge->used_volume += food->volume;
    update_temperature(fridge);
    printf("\t\t[成功] 食物[%s]已放入!\n", food->name);
    return 1;
}

// 删除食物
int remove_food(Fridge* fridge, char* name) {
    FoodNode* current = fridge->food_list;
    FoodNode* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) fridge->food_list = current->next;
            else prev->next = current->next;
            fridge->used_volume -= current->volume;
            free_food(current);
            update_temperature(fridge);
            printf("\t\t[成功] 食物[%s]已取出!\n", name);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    printf("\t\t[错误] 没有找到食物[%s]!\n", name);
    return 0;
}

// 查找食物
FoodNode* find_food(Fridge* fridge, char* name) {
    FoodNode* current = fridge->food_list;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) return current;
        current = current->next;
    }
    return NULL;
}

// 打印列头
void print_header() {
    printf("\t\t");
    print_col("序号", 6);
    print_col("名称", 14);
    print_col("体积", 10);
    print_col("温度", 8);
    print_col("种类", 8);
    printf("\n");
}

// 打印行数据
void print_row(int i, FoodNode* f) {
    char seq[10], vol[20], temp[20];
    sprintf(seq, "%02d", i);
    sprintf(vol, "%.2f", f->volume);
    sprintf(temp, "%d℃", f->temperature);
    
    printf("\t\t");
    print_col(seq, 6);
    print_col(f->name, 14);
    print_col(vol, 10);
    print_col(temp, 8);
    print_col(get_type_name(f->type), 8);
    printf("\n");
}

// 按种类查询
void search_by_type(Fridge* fridge, FoodType type) {
    FoodNode* current = fridge->food_list;
    int count = 0;
    char title[50];
    sprintf(title, "查询结果: [%s]类", get_type_name(type));
    
    printf("\n");
    print_bar('=');
    printf("\t\t  %s\n", title);
    print_bar('-');
    
    int has_food = 0;
    FoodNode* check = fridge->food_list;
    while (check != NULL) {
        if (check->type == type) { has_food = 1; break; }
        check = check->next;
    }

    if (!has_food) {
        printf("\t\t  (空) 未找到该类食物\n");
    } else {
        print_header();
        print_bar('-');
        while (current != NULL) {
            if (current->type == type) {
                count++;
                print_row(count, current);
            }
            current = current->next;
        }
        print_bar('-');
        printf("\t\t  共找到 %d 个\n", count);
    }
    print_bar('=');
}

// 修改食物信息
int modify_food(Fridge* fridge, char* name, float new_volume, int new_temp) {
    FoodNode* food = find_food(fridge, name);
    if (food == NULL) {
        printf("\t\t[错误] 没有找到食物[%s]!\n", name);
        return 0;
    }
    if (new_temp < fridge->min_temp) {
        printf("\t\t[错误] 温度(%d℃)低于下限(%d℃)!\n", new_temp, fridge->min_temp);
        return 0;
    }
    float volume_diff = new_volume - food->volume;
    float remaining = fridge->capacity - fridge->used_volume;
    if (volume_diff > remaining) {
        printf("\t\t[错误] 体积超过剩余容积!\n");
        return 0;
    }
    fridge->used_volume += volume_diff;
    food->volume = new_volume;
    food->temperature = new_temp;
    update_temperature(fridge);
    printf("\t\t[成功] 食物[%s]已修改!\n", name);
    return 1;
}

// 更新冰箱温度
void update_temperature(Fridge* fridge) {
    FoodNode* current = fridge->food_list;
    int min_temp = fridge->max_temp;
    if (current == NULL) {
        fridge->temperature = 0;
        return;
    }
    while (current != NULL) {
        if (current->temperature < min_temp) min_temp = current->temperature;
        current = current->next;
    }
    fridge->temperature = min_temp;
}

// 按体积排序
void sort_by_volume(Fridge* fridge) {
    if (fridge->food_list == NULL || fridge->food_list->next == NULL) {
        printf("\t\t[提示] 食物数量不足,无需排序\n");
        return;
    }
    int swapped;
    FoodNode* ptr1;
    FoodNode* lptr = NULL;
    do {
        swapped = 0;
        ptr1 = fridge->food_list;
        while (ptr1->next != lptr) {
            if (ptr1->volume > ptr1->next->volume) {
                char temp_name[50]; float temp_vol; int temp_temp; FoodType temp_type;
                strcpy(temp_name, ptr1->name); temp_vol = ptr1->volume; temp_temp = ptr1->temperature; temp_type = ptr1->type;
                strcpy(ptr1->name, ptr1->next->name); ptr1->volume = ptr1->next->volume; ptr1->temperature = ptr1->next->temperature; ptr1->type = ptr1->next->type;
                strcpy(ptr1->next->name, temp_name); ptr1->next->volume = temp_vol; ptr1->next->temperature = temp_temp; ptr1->next->type = temp_type;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// 显示冰箱信息
void display_fridge(Fridge* fridge) {
    printf("\n");
    print_bar('=');
    printf("\t\t     >>> 智 能 冰 箱 状 态 <<<\n");
    print_bar('=');
    
    printf("\t\t  [总 容 积] : %.2f\n", fridge->capacity);
    printf("\t\t  [已 使 用] : %.2f\n", fridge->used_volume);
    printf("\t\t  [剩    余] : %.2f\n", fridge->capacity - fridge->used_volume);
    printf("\t\t  [当前温度] : %d ℃\n", fridge->temperature);
    printf("\t\t  [温度范围] : %d ℃ ~ %d ℃\n", fridge->min_temp, fridge->max_temp);
    
    print_bar('-');
    printf("\t\t            食 物 列 表\n");
    print_bar('-');
    
    FoodNode* current = fridge->food_list;
    int count = 0;
    if (current == NULL) {
        printf("\t\t  (空) 冰箱里空空如也...\n");
    } else {
        print_header();
        print_bar('-');
        while (current != NULL) {
            count++;
            print_row(count, current);
            current = current->next;
        }
        print_bar('-');
    }
    printf("\t\t  共计: %d 个食物\n", count);
    print_bar('=');
}

// 获取食物数量
int get_food_count(Fridge* fridge) {
    int count = 0;
    FoodNode* current = fridge->food_list;
    while (current != NULL) count++; current = current->next;
    return count;
}

// 清空冰箱
void clear_fridge(Fridge* fridge) {
    FoodNode* current = fridge->food_list;
    while (current != NULL) {
        FoodNode* next = current->next;
        free_food(current);
        current = next;
    }
    fridge->food_list = NULL;
    fridge->used_volume = 0;
    fridge->temperature = 0;
    printf("\t\t[成功] 冰箱已清空!\n");
}

// 统计信息
void show_statistics(Fridge* fridge) {
    int v_c = 0, m_c = 0, f_c = 0;
    float v_v = 0, m_v = 0, f_v = 0;
    FoodNode* cur = fridge->food_list;
    while (cur) {
        switch(cur->type){
            case VEGETABLE: v_c++; v_v+=cur->volume; break;
            case MEAT: m_c++; m_v+=cur->volume; break;
            case FRUIT: f_c++; f_v+=cur->volume; break;
        }
        cur = cur->next;
    }
    
    printf("\n");
    print_bar('=');
    printf("\t\t       统 计 信 息\n");
    print_bar('-');
    
    printf("\t\t  蔬菜 : %d 个   (体积: %.2f)\n", v_c, v_v);
    printf("\t\t  肉类 : %d 个   (体积: %.2f)\n", m_c, m_v);
    printf("\t\t  水果 : %d 个   (体积: %.2f)\n", f_c, f_v);
    
    print_bar('-');
    printf("\t\t  总计 : %d 个   (体积: %.2f)\n", v_c+m_c+f_c, v_v+m_v+f_v);
    print_bar('=');
}
