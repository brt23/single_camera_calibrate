#include "single_camera_calibrate.hpp"

#ifdef CURRENT_FILE_PATH
std::string currentFilePath = CURRENT_FILE_PATH;
#else
#error Path of current repository is not defined in CMakeLists.txt.
#endif


int main()
{   
    std::string imageFileDir = "data/mono";
    imageFileDir = qy::pathJoin(currentFilePath, imageFileDir);

    cv::Mat internalMatrix, radialMatrix, tangentialMatrix;
    caliQY::singleCameraCalibration(imageFileDir, cv::Size(10, 7), internalMatrix, radialMatrix, tangentialMatrix);

    return 0;
}