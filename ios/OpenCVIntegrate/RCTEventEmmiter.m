//
//  RCTEventEmmiter.m
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#import <Foundation/Foundation.h>
#import "RCTEventEmmiter.h"

@implementation RCTEventEmmiter {
  bool hasListeners;
}

#pragma mark RCTEventEmitter Subclass Methods

-(instancetype)init {
    if (self = [super init]) {
      for (NSString *eventName in [self supportedEvents])
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(emitEvent:) name:eventName object:nil];
    }

    return self;
}

// Will be called when this module's first listener is added.
-(void)startObserving {
    hasListeners = YES;
    // Set up any upstream listeners or background tasks as necessary
}

// Will be called when this module's last listener is removed, or on dealloc.
-(void)stopObserving {
    hasListeners = NO;
    // Remove upstream listeners, stop unnecessary background tasks
}

-(NSArray<NSString *> *)supportedEvents {
    NSMutableArray *events = [NSMutableArray new];

    for (int i = 0; i < OSNotificationEventTypesArray.count; i++)
        [events addObject:OSEventString(i)];

    return events;
}

#pragma mark Send Event Methods

- (void)emitEvent:(NSNotification *)notification {
    if (!hasListeners) {
      NSLog(@"Attempted to send an event (%@) when no listeners were set.", notification.name);
      return;
    }
  
  [self sendEventWithName:notification.name body:notification.userInfo];
}

+ (void)sendEventWithName:(NSString *)name withBody:(NSDictionary *)body {
    [[NSNotificationCenter defaultCenter] postNotificationName:name object:nil userInfo:body];
}

/*
   This class acts as the module & event emitter
   It is initialized automatically by React-Native
   This subclass handles communication between the SDK and JavaScript
*/
RCT_EXPORT_MODULE(OpenCVModule)

RCT_EXPORT_METHOD(start){
  
}

@end
