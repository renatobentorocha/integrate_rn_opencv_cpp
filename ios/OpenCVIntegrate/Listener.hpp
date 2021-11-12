//
//  Drawer.hpp
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#ifndef Listener_hpp
#define Listener_hpp

#include <stdio.h>
#include <functional>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"

class Listener {
  std::function<void(cv::Mat result)> _run;
  
public:
  Listener(std::function<void(cv::Mat result)> run);
  void run(cv::Mat result);
};

#endif /* Listener_hpp */
