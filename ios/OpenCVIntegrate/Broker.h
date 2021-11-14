//
//  Broker.h
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#ifndef Broker_h
#define Broker_h

@protocol BrokerDelegate <NSObject>

- (void) didUpdate:(UIImage *)Image;

@end

@interface Broker : NSObject

@property(weak) id<BrokerDelegate> delegate;

- (void)start;
- (void)load:(UIImage *)image;
- (void)toGray:(UIImage *)image;
- (void)connectedComponents:(UIImage *)image;

@end

#endif /* Broker_h */
