// 主程序

#include "../include/ui.h"  // 界面函数
#include "../include/fridge.h"
#include "../include/file_io.h"

#define DATA_FILE "data/foods.csv"  // 数据文件

// 声明可能未公开的函数
const char* get_type_name(FoodType type);

int main() {
    Fridge fridge;  // 冰箱
    int choice;  // 用户选择
    FoodNode* food;  // 食物节点
    FoodNode* existing; // 现有食物节点
    char name[50];
    float new_volume;
    int new_temp;
    FoodType type;
    int t_choice; // 温度选择
    int Ty_choice; // 种类选择
    
    init_fridge(&fridge);  // 初始化
    system("color 0B");    // 设置淡青色字体
    load_from_file(&fridge, DATA_FILE);  // 加载数据
    
    int first_run = 1; // 标记首次运行

    while (1) {  // 主循环
        if (!first_run) {
            clear_screen(); // 非首次运行，先清屏
        }
        first_run = 0;
        
        show_menu();  // 显示菜单
        
        // 使用 robust input 读取选择
        choice = get_valid_int("", 0, 9);
        
        // 选择后立即清屏
        if (choice != 0) { 
            clear_screen();
        }
        
        switch (choice) {  // 根据选择执行
            case 1:  // 显示信息
                display_fridge(&fridge);
                pause_screen();
                break;
            case 2:  // 存入食物
                food = input_food(); 
                
                // 检查冲突
                existing = find_food(&fridge, food->name);
                if (existing != NULL) {
                    if (existing->temperature != food->temperature) {
                        printf("\n\t\t[冲突] 温度不一致! 现有: %d℃, 新入: %d℃\n", existing->temperature, food->temperature);
                        printf("\t\t请选择: [1] 保留现有(%d℃)  [2] 使用新入(%d℃)\n", existing->temperature, food->temperature);
                        t_choice = get_valid_int("\t\t请输入选项(1-2): ", 1, 2);
                        if (t_choice == 1) food->temperature = existing->temperature; 
                    }
                    if (existing->type != food->type) {
                        printf("\n\t\t[冲突] 种类不一致! 现有: %s, 新入: %s\n", get_type_name(existing->type), get_type_name(food->type));
                        printf("\t\t请选择: [1] 保留现有(%s)  [2] 使用新入(%s)\n", get_type_name(existing->type), get_type_name(food->type));
                        Ty_choice = get_valid_int("\t\t请输入选项(1-2): ", 1, 2);
                        if (Ty_choice == 1) food->type = existing->type; 
                    }
                }
                add_food(&fridge, food); 
                pause_screen();
                break;
            case 3:  // 取出食物
                display_fridge(&fridge);  // 先显示列表
                get_valid_string("\n\t\t请输入食物名称: ", name, 50);
                remove_food(&fridge, name);
                pause_screen();
                break;
            case 4:  // 按种类查询
                type = get_food_type(); 
                search_by_type(&fridge, type);
                pause_screen();
                break;
            case 5:  // 修改食物信息
                get_valid_string("\n\t\t请输入食物名称: ", name, 50);
                food = find_food(&fridge, name);
                if (food == NULL) {
                    printf("\t\t[错误] 没有找到[%s]!\n", name);
                } else {
                    printf("\t\t当前:体积=%.2f,温度=%d℃\n", food->volume, food->temperature);
                    new_volume = get_valid_float("\t\t请输入新体积(0.1-100): ", 0.1, 100.0);
                    new_temp = get_valid_int("\t\t请输入新温度(-20到10): ", -20, 10);
                    modify_food(&fridge, name, new_volume, new_temp);
                }
                pause_screen();
                break;
            case 6:  // 排序
                sort_by_volume(&fridge);
                display_fridge(&fridge);
                pause_screen();
                break;
            case 7:  // 统计
                show_statistics(&fridge);
                pause_screen();
                break;
            case 8:  // 保存
                save_to_file(&fridge, DATA_FILE);
                pause_screen();
                break;
            case 9:  // 清空
                printf("\n\t\t确定清空? (1-确定, 0-取消)\n");
                choice = get_valid_int("\t\t请输入选项: ", 0, 1);
                if (choice == 1) {
                    clear_fridge(&fridge);
                } else {
                    printf("\t\t[提示] 已取消\n");
                }
                pause_screen();
                break;
            case 0:  // 退出
                printf("\n\t\t[提示] 正在自动保存数据...\n");
                save_to_file(&fridge, DATA_FILE);  // 自动保存
                clear_fridge(&fridge);
                printf("\n\t\t感谢使用智能冰箱管理系统，再见！\n");
                return 0;
            default:  // 无效选项
                printf("\t\t[错误] 无效选项!\n");
                pause_screen();
                break;
        }
    }
    return 0;
}
