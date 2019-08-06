#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <cstring>
#include <assert.h>

#include "string_operations.hpp"


namespace qy
{

/*
 * @描述 读取目录下所有文件和文件夹的名称
 * @参数 dir 输入的目录
 * @返回 std::vector<std::string> 包含当前目录下所有的文件和文件夹名称的列表
 */
std::vector<std::string> listDir(const char* dir);


/*
 * @描述 用于将目录和文件名联合为一个路径，只能在linux下使用
 * @参数 dir 目录(如/home/y/文档 或 /home/y/文档/)
 * @参数 fileName 文件名(如image.jpg)
 * @返回 std::string 拼接好的路径
 */
std::string pathJoin(const std::string& dir, std::string& fileName);

};