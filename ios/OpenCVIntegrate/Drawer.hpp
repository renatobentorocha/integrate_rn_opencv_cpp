//
//  Drawer.hpp
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#ifndef Drawer_hpp
#define Drawer_hpp

#include <stdio.h>
#include <functional>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

class Drawer {
  std::function<void(cv::Mat result)> _listener;
  
public:
  Drawer(std::function<void(cv::Mat result)> listener);
  void draw(cv::Mat result);
};

#endif /* Drawer_hpp */
