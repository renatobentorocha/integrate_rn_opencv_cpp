//
//  Broker.m
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "Broker.h"
#import "RCTEventEmmiter.h"
#import "Listener.hpp"
#import "Drawer.hpp"
#import "opencv2/imgcodecs/ios.h"

@implementation Broker {
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
    });
    
    _drawer = new Drawer(_listener);
  }
  return self;
}

- (NSString *)imageToBase64:(UIImage *)image {
  NSData * data = UIImageJPEGRepresentation(image, 1);
  NSString *base64Str = [data base64EncodedStringWithOptions:NSDataBase64Encoding64CharacterLineLength];
  return base64Str;
}

- (void)simulatorDidUpdateImage:(UIImage *)image {
  NSString *base64Str = [self imageToBase64:image];
  [RCTEventEmmiter sendEventWithName:OSEventString(UpdatedImage) withBody:@{@"image_base_64": base64Str}];
}

- (void)start {
  
}

- (void)load:(UIImage *)image {
  
}

- (void)toGray:(UIImage *)image {
  
}

- (void)connectedComponents:(UIImage *)image {
  
}

- (void) didUpdate {
  [self->_delegate didUpdate:self->image];
}

@end

