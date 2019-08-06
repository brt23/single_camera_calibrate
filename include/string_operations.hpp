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
 * @描述 用于判断一个字符结尾是不是包含另一个字符串
 * @描述 应用场景是用来判断文件的后缀名，以此来区分文件
 * @参数 target_str 被查找的字符串(如image.jpg)
 * @参数 find_str 查找的字符串(如.jpg)
 * @返回 bool 包含为真否则为假
 */
bool endsWith(const std::string& target_str, const std::string& find_str);

};