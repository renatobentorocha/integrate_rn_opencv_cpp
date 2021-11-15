//
//  Broker.h
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//
#import <UIKit/UIKit.h>

#ifndef BrokerRight_h
#define BrokerRight_h

@protocol BrokerDelegate <NSObject>

- (void) didUpdate:(UIImage *)image;

@end

@interface BrokerRight : NSObject

@property(weak) id<BrokerDelegate> delegate;

- (void)load:(UIImage *)image;
- (void)toGray:(UIImage *)image;
- (void)connectedComponents:(UIImage *)image;

@end

#endif /* BrokerRight_h */
