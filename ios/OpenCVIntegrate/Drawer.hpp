//
//  Drawer.hpp
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 12/11/21.
//

#ifndef Drawer_hpp
#define Drawer_hpp

#include <stdio.h>
#include "Listener.hpp"
#include <opencv2/opencv.hpp>


class Drawer {
  Listener _listener;
  void toGray(cv::Mat src, cv::Mat &dst);
  
public:
  Drawer(Listener listener) : _listener {listener} {};
  void toGray(cv::Mat src);
  void connectedComponents(cv::Mat src);
};

#endif /* Drawer_hpp */
