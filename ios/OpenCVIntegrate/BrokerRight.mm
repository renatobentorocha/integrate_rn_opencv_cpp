//
//  Broker.m
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#import "Drawer.hpp"
#import "opencv2/imgcodecs/ios.h"
#import "Listener.hpp"
#import "BrokerRight.h"
#import <Foundation/Foundation.h>
#import "RCTEventEmmiter.h"


@implementation BrokerRight {
  Listener * _listener;
  Drawer * _drawer;
  UIImage * image;
}

- (instancetype)init
{
  self = [super init];
  if (self) {
    _listener = new Listener([self](cv::Mat result){
      cv::Mat mat = result.clone();
      self->image = MatToUIImage(mat);
      [self didUpdate];
    });
    
    _drawer = new Drawer(_listener);
  }
  return self;
}

- (void)load:(UIImage *)image {
  cv::Mat mat;
  UIImageToMat(image, mat);
  self->_drawer->load(mat);
}

- (void)toGray:(UIImage *)image {
  cv::Mat mat;
  UIImageToMat(image, mat);
  self->_drawer->toGray(mat);
}

- (void)connectedComponents:(UIImage *)image {
  cv::Mat mat;
  UIImageToMat(image, mat);
  self->_drawer->connectedComponents(mat);
}

- (void) didUpdate {  
  [self->_delegate didUpdate:self->image];
}

@end

