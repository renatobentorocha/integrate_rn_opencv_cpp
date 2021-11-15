//
//  RCTEventEmmiter.m
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 11/11/21.
//

#import <Foundation/Foundation.h>
#import "RCTEventEmmiter.h"
#import "BrokerLeft.h"

@implementation RCTEventEmmiter {
  UIImage * original;
  UIImage * copy;
  BrokerLeft * broker;
  bool hasListeners;
}

#pragma mark RCTEventEmitter Subclass Methods

-(instancetype)init {
    if (self = [super init]) {
      self->broker = [[BrokerLeft alloc] init];
      for (NSString *eventName in [self supportedEvents])
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(emitEvent:) name:eventName object:nil];
    }
    return self;
}

+ (BOOL)requiresMainQueueSetup {
  return TRUE;
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
RCT_EXPORT_MODULE(NativeLibModule)

RCT_EXPORT_METHOD(start){
  
}

RCT_EXPORT_METHOD(load:(NSString *)imageUrl exif:(double)exif resolver:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject){
  @try {
    NSLog(@"LOAD STR %@: ", imageUrl);
    
    UIImage * image = [UIImage imageWithContentsOfFile:imageUrl];
    self->original = [image copy];
    self->copy = image;
    [self->broker load:image];
    resolve(@[imageUrl]);
  } @catch (NSException *exception) {
    reject(@"LOAD_ERROR", exception.reason, nil);
  }
}

RCT_EXPORT_METHOD(toGray:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject) {
  @try {
    [self->broker toGray:copy];
    resolve(@[@(true)]);
  } @catch (NSException *exception) {
    reject(@"TO_GRAY", exception.reason, nil);
  }
}

RCT_EXPORT_METHOD(connectedComponents:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject) {
  @try {
    [self->broker connectedComponents:copy];
    resolve(@[@(true)]);
  } @catch (NSException *exception) {
    reject(@"CONNECTED_COMPONENTS", exception.reason, nil);
  }
}

RCT_EXPORT_METHOD(toOriginal:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject){
  @try {
    [broker didUpdate:original];
    resolve(@[@(true)]);
  } @catch (NSException *exception) {
    reject(@"TO_ORIGINAL", exception.reason, nil);
  }
}

@end
