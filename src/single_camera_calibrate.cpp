#include "single_camera_calibrate.hpp"


/*
 * @描述 方块标定版方块间角点的世界坐标
 * @参数 boardSize 标定板方块间角点的列数和行数cv::Size(width，height)
 * @返回 std::vector<cv::Point3f> 标定版方块间角点的世界坐标点集
 */
std::vector<cv::Point3f> calcObpoint(cv::Size boardSize)
{   
    std::vector<cv::Point3f> objPoint;
    for (int i_col=0; i_col<boardSize.width; i_col++)
    {
        for (int i_row=0; i_row<boardSize.height; i_row++)
        {
            objPoint.push_back(cv::Point3f(i_row, i_col, 0));
        }
    }
    return objPoint;
}


/*
 * @描述 从目录中获取图片的路径
 * @参数 dir 有图片的目录
 * @参数 postfix 图片的后缀名{JPG、PNG、JPG+PNG}，默认{JPG+PNG}
 * @返回 std::vector<std::string> 包含当前目录下所有选定后缀的图片文件的路径
 */
std::vector<std::string> getImagePathFromDir(const std::string dir, int postfix)
{
    std::vector<std::string> fileNames = qy::listDir(dir.data());
    std::vector<std::string> imagePaths;
    for (auto name : fileNames)
    {
        switch (postfix)
        {
            case POSTFIX::JPG:
                if (qy::endsWith(name, ".jpg"))
                {
                    imagePaths.push_back(qy::pathJoin(dir, name));
                }
                break;

            case POSTFIX::PNG:
                if (qy::endsWith(name, ".png"))
                {
                    imagePaths.push_back(qy::pathJoin(dir, name));
                }
                break;

            case POSTFIX::JPG + POSTFIX::PNG:
                if (qy::endsWith(name, ".jpg") or qy::endsWith(name, ".png"))
                {
                    imagePaths.push_back(qy::pathJoin(dir, name));
                }
                break;
        
            default:
                break;
        }
    }

    return imagePaths;
}

namespace caliQY
{
    
void singleCameraCalibration(const std::string imageDir, cv::Size boardSize, cv::Mat& internalMatrixOut, cv::Mat& radialMatrixOut, cv::Mat& tangentialMatrixOut, int postfix)
{
    // 读取图片目录下的图片的文件名
    std::vector<std::string> filePaths = getImagePathFromDir(imageDir, postfix);

    // 定义标定板角点的世界坐标和图像坐标的容器
    std::vector<cv::Point3f> objp = calcObpoint(boardSize);
    std::vector< std::vector<cv::Point3f> > objPoints;
    std::vector< std::vector<cv::Point2f> > imgPoints;

    // 通过文件名依次读取图片，并检查图片中标定板角点，获取角点的坐标
    int success_count = 0;
    int image_count = 0;
    int total_num = filePaths.size();
    cv::Size imgSize;
    cv::namedWindow("calibrate", cv::WINDOW_GUI_EXPANDED);
    for (auto path : filePaths)
    {
        cv::Mat image = cv::imread(path);
        CV_Assert(!image.empty());
        image_count++;
        
        if (imgSize.width == 0 && imgSize.height == 0)
        {
            imgSize = image.size();
        }
        
        // 在标定板上检测角点并记录成功检测到的角点
        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        cv::Mat corners;
        if (cv::findChessboardCorners(gray, cv::Size(7, 10), corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE))
        {
            success_count++;

            cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 100, 0.001);
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            objPoints.push_back(objp);
            imgPoints.push_back(corners.clone());

            cv::drawChessboardCorners(image, cv::Size(7, 10), corners, true);   
        }

        // 在图片上显示当前角点检测成功的图片数量和任务完成进度
        char buffer_str[20];
        double rate = (double)image_count / (double)total_num * 100.0;
        sprintf(buffer_str, "%.1f%%", rate);
        std::string dispText_line1 = std::string("RateOfProgress: ") + std::string(buffer_str);
        std::string dispText_line2 = std::string("Successed: ") + std::to_string(success_count);
        cv::putText(image, dispText_line1, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 255), 1);
        cv::putText(image, dispText_line2, cv::Point(10, 70), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0, 0, 255), 1);

        cv::imshow("calibrate", image);
        cv::waitKey(50);       
    }

    // 注销显示窗口
    cv::destroyWindow("calibrate");
    std::cout << "Calibrate calculating ..." << std::endl;
    
    // 通过标定板角点的世界坐标点集和对应的图片坐标点集，计算出标定结果
    cv::Mat internalMatrix, distMatrix;
    cv::Mat k1k2k3, p1p2;
    std::vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(objPoints, imgPoints, imgSize, internalMatrix, distMatrix, rvecs, tvecs);
    // 将径向畸变和切向畸变分为两个变量
    k1k2k3 = distMatrix.colRange(0, 3);
    p1p2   = distMatrix.colRange(3, 5);

    // 在终端上打印结果
    std::cout << "Result! ========================================================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "Used image: " << std::to_string(total_num) << "" << std::endl;
    std::cout << "Successed find chessboard corner image: " << std::to_string(success_count) << "" << std::endl;
    std::cout << std::endl;
    std::cout << "Internal matrix:" << std::endl;
    std::cout << internalMatrix << std::endl;
    std::cout << std::endl;
    std::cout << "Distortion matrix:" << std::endl;
    std::cout << distMatrix << std::endl;
    std::cout << std::endl;
    std::cout << "Radial distortion matrix:" << std::endl;
    std::cout << k1k2k3 << std::endl;
    std::cout << std::endl;
    std::cout << "Tangential distortion matrix:" << std::endl;
    std::cout << p1p2 << std::endl;
    std::cout << std::endl;
    std::cout << "Done! =========================================================================================="<< std::endl;

    // 输出标定结果
    internalMatrixOut   = internalMatrix.clone();
    radialMatrixOut     = k1k2k3.clone();
    tangentialMatrixOut = p1p2.clone();
}

};
