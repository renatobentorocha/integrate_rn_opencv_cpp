//
//  BrokerLeft.h
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 15/11/21.
//

#ifndef BrokerLeft_h
#define BrokerLeft_h
#import "BrokerRight.h"
#import "RCTEventEmmiter.h"

@interface BrokerLeft : NSObject<BrokerDelegate>

@property(weak) id<EventEmmiterDelegate> delegate;

- (void)load:(UIImage *)image;
- (void)toGray:(UIImage *)image;
- (void)connectedComponents:(UIImage *)image;

@end

#endif /* BrokerLeft_h */
