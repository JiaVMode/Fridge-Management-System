// 文件读写函数实现

#include "../include/file_io.h"
#include <stdio.h>
#include <string.h>

// 保存到文件
int save_to_file(Fridge* fridge, char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("\t\t[警告] 无法打开文件 %s 进行保存!\n", filename);
        return 0;
    }
    
    // 写入CSV头
    fprintf(fp, "Name,Volume,Temperature,Type\n");
    
    FoodNode* current = fridge->food_list;
    while (current != NULL) {
        // 格式: 名称,体积,温度,种类ID
        fprintf(fp, "%s,%.2f,%d,%d\n", 
                current->name, 
                current->volume, 
                current->temperature, 
                current->type);
        current = current->next;
    }
    
    fclose(fp);
    printf("\t\t[成功] 数据已保存至 %s\n", filename);
    return 1;
}

// 从文件加载
int load_from_file(Fridge* fridge, char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        // 文件不存在是正常的，不报错
        return 0;
    }
    
    char line[256];
    // 跳过第一行(表头)
    fgets(line, sizeof(line), fp);
    
    int loaded_count = 0;
    while (fgets(line, sizeof(line), fp)) {
        // 去除换行符
        size_t len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0';
        
        // 解析CSV: Name,Volume,Temperature,Type
        // 注意：使用sscanf比strtok更简单，因为格式固定
        char name[50];
        float volume;
        int temp;
        int type_int;
        
        // %49[^,] 读取直到逗号，限制49字符
        if (sscanf(line, "%49[^,],%f,%d,%d", name, &volume, &temp, &type_int) == 4) {
            FoodNode* food = create_food(name, volume, temp, (FoodType)type_int);
            if (food != NULL) {
                // 使用add_food添加到冰箱，这样会自动处理体积合并和检查
                // 为了避免打印太多"成功放入"的信息，这里可以暂时静默，但
                // 考虑到逻辑复用，直接调用最安全。
                // *注意*: add_food 会打印信息。如果加载很多，会刷屏。
                // 改进方案：这里手动插入，或者接受刷屏。
                // 为了"逻辑正确"优先，我们手动实现简化版插入（不打印，但合并）
                // 实际上，重用代码是最好的。文件加载通常只在启动时，刷屏也会被后续清屏盖过。
                add_food(fridge, food);
                loaded_count++;
            }
        }
    }
    
    fclose(fp);
    if (loaded_count > 0) {
        printf("\t\t[提示] 已从文件加载 %d 条记录\n", loaded_count);
    }
    return 1;
}
