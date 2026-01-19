#ifndef FILE_IO_H
#define FILE_IO_H

#include "fridge.h"  // 需要Fridge定义

// 保存到文件
int save_to_file(Fridge* fridge, char* filename);

// 从文件加载
int load_from_file(Fridge* fridge, char* filename);

#endif
