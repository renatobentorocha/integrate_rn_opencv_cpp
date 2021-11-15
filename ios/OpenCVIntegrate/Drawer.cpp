//
//  Drawer.cpp
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 12/11/21.
//

#include "Drawer.hpp"
#include <opencv2/opencv.hpp>

void Drawer::load(cv::Mat mat) {
  _mat = mat;
  _listener->run(mat);
}

void Drawer::toGray(cv::Mat src) {
  cv::Mat dst;
  toGray(src, dst);
  _listener->run(dst);
}

void Drawer::toGray(cv::Mat src, cv::Mat &dst) {
  cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
}

void Drawer::connectedComponents(cv::Mat src) {
  cv::Mat img = src;

  cv::Mat result;
  img.copyTo(result);

  // Create binary image from source image
  cv::Mat bw;
  cv::cvtColor(img, bw, cv::COLOR_BGR2GRAY);
  cv::threshold(bw, bw, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

  // Perform the distance transform algorithm
  cv::Mat dist;
  cv::distanceTransform(bw, dist, cv::DIST_L2, 5);

  // Normalize the distance image for range = {0.0, 1.0}
  // so we can visualize and threshold it
  cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);

  // Threshold to obtain the peaks
  // This will be the markers for the foreground objects
  cv::threshold(dist, dist, 0, 255, cv::THRESH_BINARY);

  cv::Mat markers(img.size(), CV_32S);
  dist.convertTo(dist, CV_8UC1);
  int nLabels = cv::connectedComponents(dist, markers, 8);

  cv::Mat markers_cp = markers.clone();
  markers_cp.convertTo(markers_cp, CV_8UC1);

  std::vector<cv::Vec3b> colors(nLabels);
  colors[0] = cv::Vec3b(0, 0, 0); //background

  for (int label = 1; label < nLabels; ++label)
  {
      colors[label] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
  }

  cv::Mat dst(img.size(), CV_8UC3);

  for (int r = 0; r < dst.rows; ++r)
  {
      for (int c = 0; c < dst.cols; ++c)
      {
          int label = markers.at<int>(r, c);
          cv::Vec3b &pixel = dst.at<cv::Vec3b>(r, c);
          pixel = colors[label];
      }
  }

  _listener->run(dst);
}
