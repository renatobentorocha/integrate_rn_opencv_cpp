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

@implementation Broker {
  
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

@end

