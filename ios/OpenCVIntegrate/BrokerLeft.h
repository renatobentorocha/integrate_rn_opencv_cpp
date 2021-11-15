//
//  BrokerLeft.h
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 15/11/21.
//

#ifndef BrokerLeft_h
#define BrokerLeft_h
#import "BrokerRight.h"

@interface BrokerLeft : NSObject<BrokerDelegate>

- (void)load:(UIImage *)image;
- (void)toGray:(UIImage *)image;
- (void)connectedComponents:(UIImage *)image;

@end

#endif /* BrokerLeft_h */
