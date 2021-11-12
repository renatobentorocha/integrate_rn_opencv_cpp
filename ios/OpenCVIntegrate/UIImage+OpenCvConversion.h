//
//  UIImage+OpenCvConversion.h
//  OpenCVIntegrate
//
//  Created by Renato Rocha on 12/11/21.
//

#ifndef UIImage_OpenCvConversion_h
#define UIImage_OpenCvConversion_h

#import <UIKit/UIKit.h>

#ifdef __cplusplus
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#import <opencv2/core/core.hpp>

#pragma clang pop
#endif

@interface UIImage (UIImage_OpenCvConversion)

#ifdef __cplusplus

+ (cv::Mat)toCvMat:(UIImage *)image;
+ (cv::Mat)toCvMatGray:(UIImage *)image;
+ (UIImage *)fromCvMat:(cv::Mat)cvMat;

#endif

@end

#endif /* UIImage_OpenCvConversion_h */
