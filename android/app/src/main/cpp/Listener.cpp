//
//  Drawer.cpp
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#include "Listener.hpp"

Listener::Listener(std::function<void(cv::Mat result)> run){
  _run = run;
};

void Listener::run(cv::Mat result) {
  _run(result);
};
