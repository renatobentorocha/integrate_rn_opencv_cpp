#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include "iostream"

using namespace cv;

struct JavaCallback {
    jmethodID method;
    jobject callback;
};

struct JavaSimulator {
    JNIEnv *env;
    Mat mat;
};

static JavaCallback jdrawer;
static JavaSimulator jsim;

void l(String val) {
    __android_log_print(ANDROID_LOG_DEBUG, "PQN", "%s", val.c_str());
}

void bitmapToMat(jobject bitmap, Mat &dst, bool needUnPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
        l("nBitmapToMat");
        CV_Assert( AndroidBitmap_getInfo(jsim.env, bitmap, &info) >= 0 );
        l("AndroidBitmap_getInfo(jsim.env, bitmap, &info) >= 0");
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        l("nBitmapToMat - ANDROID_BITMAP_FORMAT_RGBA_8888 || ANDROID_BITMAP_FORMAT_RGB_565");
        CV_Assert( AndroidBitmap_lockPixels(jsim.env, bitmap, &pixels) >= 0 );
        l("AndroidBitmap_lockPixels(jsim.env, bitmap, &pixels) >= 0");
        CV_Assert( pixels );
        l("nBitmapToMat - pixels");
        dst.create(info.height, info.width, CV_8UC4);
        if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
        {
            l("nBitmapToMat: RGBA_8888 -> CV_8UC4");
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if(needUnPremultiplyAlpha) cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
            else tmp.copyTo(dst);
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            l("nBitmapToMat: RGB_565 -> CV_8UC4");
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            cvtColor(tmp, dst, COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        return;
    } catch(const cv::Exception& e) {
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        l(e.err);
        jclass je = jsim.env->FindClass("java/lang/Exception");
        jsim.env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        l("nBitmapToMat caught unknown exception (...)");
        jclass je = jsim.env->FindClass("java/lang/Exception");
        jsim.env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
        return;
    }
}

void matToBitmap(Mat &src, jobject bitmap, bool needPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
        l("nMatToBitmap");
        CV_Assert( AndroidBitmap_getInfo(jsim.env, bitmap, &info) >= 0 );
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        CV_Assert( src.dims == 2 && info.height == (uint32_t)src.rows && info.width == (uint32_t)src.cols );
        CV_Assert( src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4 );
        CV_Assert( AndroidBitmap_lockPixels(jsim.env, bitmap, &pixels) >= 0 );
        CV_Assert( pixels );
        if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
        {
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if(src.type() == CV_8UC1)
            {
                l("nMatToBitmap: CV_8UC1 -> RGBA_8888");
                cvtColor(src, tmp, COLOR_GRAY2RGBA);
            } else if(src.type() == CV_8UC3){
                l("nMatToBitmap: CV_8UC3 -> RGBA_8888");
                cvtColor(src, tmp, COLOR_RGB2RGBA);
            } else if(src.type() == CV_8UC4){
                l("nMatToBitmap: CV_8UC4 -> RGBA_8888");
                if(needPremultiplyAlpha) cvtColor(src, tmp, COLOR_RGBA2mRGBA);
                else src.copyTo(tmp);
            }
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            if(src.type() == CV_8UC1)
            {
                l("nMatToBitmap: CV_8UC1 -> RGB_565");
                cvtColor(src, tmp, COLOR_GRAY2BGR565);
            } else if(src.type() == CV_8UC3){
                l("nMatToBitmap: CV_8UC3 -> RGB_565");
                cvtColor(src, tmp, COLOR_RGB2BGR565);
            } else if(src.type() == CV_8UC4){
                l("nMatToBitmap: CV_8UC4 -> RGB_565");
                cvtColor(src, tmp, COLOR_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        return;
    } catch(const cv::Exception& e) {
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        l(e.err);
        jclass je = jsim.env->FindClass("java/lang/Exception");
        jsim.env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        l("nMatToBitmap caught unknown exception (...)");
        jclass je = jsim.env->FindClass("java/lang/Exception");
        jsim.env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return;
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_opencvintegrate_jni_NativeLib_init(JNIEnv *env, jclass clazz, jobject on_update_listener) {
    jsim.env = env;
    jdrawer.callback = jsim.env->NewGlobalRef(on_update_listener);
    jdrawer.method = jsim.env->GetMethodID(env->GetObjectClass(jdrawer.callback), "onUpdate", "(Landroid/graphics/Bitmap;)V");
}

extern "C"
JNIEXPORT void JNICALL Java_com_opencvintegrate_jni_NativeLib_load(JNIEnv *env, jclass clazz, jobject bitmap) {
    Mat mat = Mat();
    bitmapToMat(bitmap, mat, false);
    jsim.mat = mat;
    jsim.env->CallVoidMethod(jdrawer.callback, jdrawer.method, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_opencvintegrate_jni_NativeLib_toGray(JNIEnv *env, jclass clazz, jobject bitmap) {
    Mat mat = Mat();
    cvtColor(jsim.mat, mat, COLOR_BGR2GRAY);

    matToBitmap(mat, bitmap, false);

    jsim.env->CallVoidMethod(jdrawer.callback, jdrawer.method, bitmap);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_opencvintegrate_jni_NativeLib_connectedComponents(JNIEnv *env, jclass clazz, jobject bitmap) {
    Mat img = jsim.mat;

    cv::Mat result;
    img.copyTo(result);

    // Create binary image from source image
    cv::Mat bw;
    cv::cvtColor(img, bw, cv::COLOR_BGR2GRAY);
    cv::threshold(bw, bw, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    // Perform the distance transform algorithm
    cv::Mat dist;
    cv::distanceTransform(bw, dist, cv::DIST_L2, 5);

    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);

    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    cv::threshold(dist, dist, 0, 255, cv::THRESH_BINARY);

    cv::Mat markers(img.size(), CV_32S);
    dist.convertTo(dist, CV_8UC1);
    int nLabels = cv::connectedComponents(dist, markers, 8);

    cv::Mat markers_cp = markers.clone();
    markers_cp.convertTo(markers_cp, CV_8UC1);

    std::vector<cv::Vec3b> colors(nLabels);
    colors[0] = cv::Vec3b(0, 0, 0); //background

    for (int label = 1; label < nLabels; ++label)
    {
        colors[label] = cv::Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
    }

    cv::Mat dst(img.size(), CV_8UC3);

    for (int r = 0; r < dst.rows; ++r)
    {
        for (int c = 0; c < dst.cols; ++c)
        {
            int label = markers.at<int>(r, c);
            cv::Vec3b &pixel = dst.at<cv::Vec3b>(r, c);
            pixel = colors[label];
        }
    }

    matToBitmap(dst, bitmap, false);

    jsim.env->CallVoidMethod(jdrawer.callback, jdrawer.method, bitmap);
}