//
//  RCTEventEmmiter.h
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#ifndef RCTEventEmmiter_h
#define RCTEventEmmiter_h

#import <React/RCTBridgeModule.h>
#import <React/RCTEventEmitter.h>

typedef NS_ENUM(NSInteger, OSNotificationEventTypes) {
    UpdatedImage,
};

#define OSNotificationEventTypesArray @[@"UpdatedImage"]
#define OSEventString(enum) [OSNotificationEventTypesArray objectAtIndex:enum]

@protocol EventEmmiterDelegate <NSObject>

- (void) update:(UIImage *)image;

@end

@interface RCTEventEmmiter : RCTEventEmitter<RCTBridgeModule, EventEmmiterDelegate>

+ (void)sendEventWithName:(NSString *)name withBody:(NSDictionary *)body;

@end

#endif /* RCTEventEmmiter_h */
