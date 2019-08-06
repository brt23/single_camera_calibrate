import numpy as np
import cv2
import os
import time
from tqdm import tqdm


objp = np.zeros((10*7,3), np.float32)
objp[:,:2] = np.mgrid[0:7, 0:10].T.reshape(-1,2) # shape(2, 7, 10) -> shape(10, 7, 2) -> shape(70, 2)

image_dir = "/home/y/文档/独立程序代码库/Python代码/calibrate_code_release/data/mono"

image_file = [file for file in os.listdir(image_dir) if file.endswith('.jpg')]
image_file.sort(key=lambda x: int(os.path.splitext(x)[0].split('_')[-1]))
image_path = [os.path.join(image_dir, file) for file in image_file]

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 100, 0.001)

objpoints = []
imgpoints = []

cv2.namedWindow("Calibrate", cv2.WINDOW_NORMAL)
cv2.resizeWindow("Calibrate", 640, 360)

for path in tqdm(image_path):
    image = cv2.imread(path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    ret, corners = cv2.findChessboardCorners(gray, (7, 10))
    if ret:
        corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        objpoints.append(objp)
        imgpoints.append(corners2)
        cv2.drawChessboardCorners(image, (7, 10), corners2, True)
        cv2.putText(image, "valid", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 255), 1)
    else:
        cv2.putText(image, "invalid", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 255), 1)
    
    cv2.putText(image, "success {}".format(len(imgpoints)), (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0, 0, 255), 1)
    cv2.imshow("Calibrate", image)
    key = cv2.waitKey(100)
    if key != -1:
        break

cv2.destroyAllWindows()

ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)
print("Is successfully: {}".format(ret))
print(mtx)
print(dist)
print(rvecs)
print(tvecs)

save_dir = 'results'
now_time = time.strftime("%Y%m%d_%H%M%S", time.localtime())
use_images = "A{}-U{}".format(len(image_path), len(objpoints))

np.savetxt(os.path.join(save_dir, '{}_{}_MTX.txt'.format(now_time, use_images,)), mtx)
np.savetxt(os.path.join(save_dir, '{}_{}_DIST.txt'.format(now_time, use_images,)),  dist)