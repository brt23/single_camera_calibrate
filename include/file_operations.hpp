#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <cstring>
#include <assert.h>


namespace qy
{

/*
 * @描述 读取text文件中保存的图片路径
 * @参数 file text文件路径
 * @返回 std::vector<std::string> 文件中所有的图片路径
 */
std::vector<std::string> readText(const std::string file);

};
