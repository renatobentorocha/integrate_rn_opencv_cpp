//
//  Drawer.cpp
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 12/11/21.
//

#include "Drawer.hpp"
#include <opencv2/opencv.hpp>

void Drawer::toGray(cv::Mat src) {
  cv::Mat dst;
  toGray(src, dst);
  _listener.run(dst);
}

void Drawer::toGray(cv::Mat src, cv::Mat &dst) {
  cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
}

void Drawer::connectedComponents(cv::Mat src) {
  
}
