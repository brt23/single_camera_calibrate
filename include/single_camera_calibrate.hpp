#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <cstring>

#include "file_operations.hpp"
#include "path_operations.hpp"
#include "string_operations.hpp"


/*
* @描述 图片文件后缀枚举类型
* @描述 暂只用于getImagePathFromDir函数，以方便通过后缀来选择文件
*/
enum POSTFIX
{
    JPG=1,
    PNG=2
};


/*
* @描述 单目相机标定模块命名空间
*/
namespace caliQY
{

/*
* @描述 单目相机标定
* @参数 imageDir 包含标定图片的目录 
* @参数 boardSize 标定板方块间角点的列数和行数cv::Size(width，height)
* @参数 internalMatrixOut 返回计算出的相机内参矩阵
* @参数 radialMatrixOut 返回计算出的相机径向畸变向量
* @参数 tangentialMatrixOut 返回计算出的相机切向畸变向量
* @参数 postfix 图片的后缀名{JPG、PNG、JPG+PNG}，默认{JPG}
*/
void singleCameraCalibration(const std::string imageDir, cv::Size boardSize, cv::Mat& internalMatrixOut, cv::Mat& radialMatrixOut, cv::Mat& tangentialMatrixOut, int postfix=POSTFIX::JPG);

};
