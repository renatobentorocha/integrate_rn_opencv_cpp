//
//  BrokerLeft.m
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 15/11/21.
//

#import <Foundation/Foundation.h>
#import "BrokerLeft.h"
#import "RCTEventEmmiter.h"

@implementation BrokerLeft {
  BrokerRight *broker;
}

- (instancetype)init
{
  self = [super init];
  if (self) {
    self->broker = [[BrokerRight alloc] init];
    self->broker.delegate = self;
  }
  return self;
}

- (NSString *)imageToBase64:(UIImage *)image {
  NSData * data = UIImageJPEGRepresentation(image, 1);
  NSString *base64Str = [data base64EncodedStringWithOptions:NSDataBase64Encoding64CharacterLineLength];
  return base64Str;
}

- (void)didUpdate:(id)image {
  NSString *base64Str = [self imageToBase64:image];
  [RCTEventEmmiter sendEventWithName:OSEventString(UpdatedImage) withBody:@{@"image_base_64": base64Str}];
}

- (void)load:(UIImage *)image {
  [self->broker load:image];
}

- (void)toGray:(UIImage *)image {
  [self->broker toGray:image];
}

- (void)connectedComponents:(UIImage *)image {
  [self->broker connectedComponents:image];
}

@end
