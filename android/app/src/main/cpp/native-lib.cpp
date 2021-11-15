#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include "iostream"
#include "Listener.hpp"
#include "Drawer.hpp"

using namespace cv;

struct JavaCallback {
    jmethodID method;
    jobject callback;
};

struct JavaBroker {
    Drawer * _drawer;
    Listener * _listener;
    JNIEnv * env;
    Mat mat;
    jobject bitmap;
};

static JavaCallback jdrawer;
static JavaBroker jbroker;

void l(String val) {
    __android_log_print(ANDROID_LOG_DEBUG, "PQN", "%s", val.c_str());
}

void bitmapToMat(jobject bitmap, Mat &dst, bool needUnPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
        l("nBitmapToMat");
        CV_Assert( AndroidBitmap_getInfo(jbroker.env, bitmap, &info) >= 0 );
        l("AndroidBitmap_getInfo(jbroker.env, bitmap, &info) >= 0");
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        l("nBitmapToMat - ANDROID_BITMAP_FORMAT_RGBA_8888 || ANDROID_BITMAP_FORMAT_RGB_565");
        CV_Assert( AndroidBitmap_lockPixels(jbroker.env, bitmap, &pixels) >= 0 );
        l("AndroidBitmap_lockPixels(jbroker.env, bitmap, &pixels) >= 0");
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
        AndroidBitmap_unlockPixels(jbroker.env, bitmap);
        return;
    } catch(const cv::Exception& e) {
        AndroidBitmap_unlockPixels(jbroker.env, bitmap);
        l(e.err);
        jclass je = jbroker.env->FindClass("java/lang/Exception");
        jbroker.env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(jbroker.env, bitmap);
        l("nBitmapToMat caught unknown exception (...)");
        jclass je = jbroker.env->FindClass("java/lang/Exception");
        jbroker.env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
        return;
    }
}

void matToBitmap(Mat &src, jobject bitmap, bool needPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
        l("nMatToBitmap");
        CV_Assert( AndroidBitmap_getInfo(jbroker.env, bitmap, &info) >= 0 );
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        CV_Assert( src.dims == 2 && info.height == (uint32_t)src.rows && info.width == (uint32_t)src.cols );
        CV_Assert( src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4 );
        CV_Assert( AndroidBitmap_lockPixels(jbroker.env, bitmap, &pixels) >= 0 );
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
        AndroidBitmap_unlockPixels(jbroker.env, bitmap);
        return;
    } catch(const cv::Exception& e) {
        AndroidBitmap_unlockPixels(jbroker.env, bitmap);
        l(e.err);
        jclass je = jbroker.env->FindClass("java/lang/Exception");
        jbroker.env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(jbroker.env, bitmap);
        l("nMatToBitmap caught unknown exception (...)");
        jclass je = jbroker.env->FindClass("java/lang/Exception");
        jbroker.env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return;
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_opencvintegrate_jni_NativeLib_start(JNIEnv *env, jclass clazz, jobject on_update_listener) {
    jbroker.env = env;
    jdrawer.callback = jbroker.env->NewGlobalRef(on_update_listener);
    jdrawer.method = jbroker.env->GetMethodID(env->GetObjectClass(jdrawer.callback), "onUpdate", "(Landroid/graphics/Bitmap;)V");

    jbroker._listener = new Listener([](cv::Mat result) {
        matToBitmap(result, jbroker.bitmap, false);
        jbroker.env->CallVoidMethod(jdrawer.callback, jdrawer.method, jbroker.bitmap);
    });

    jbroker._drawer = new Drawer(jbroker._listener);
}

extern "C"
JNIEXPORT void JNICALL Java_com_opencvintegrate_jni_NativeLib_load(JNIEnv *env, jclass clazz, jobject bitmap) {
    Mat mat = Mat();
    bitmapToMat(bitmap, mat, false);
    jbroker.mat = mat;
    jbroker.bitmap = bitmap;
    jbroker._drawer->load(mat);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_opencvintegrate_jni_NativeLib_toGray(JNIEnv *env, jclass clazz, jobject bitmap) {
    jbroker.bitmap = bitmap;
    Mat mat = Mat();
    bitmapToMat(bitmap, mat, false);
    jbroker._drawer->toGray(mat);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_opencvintegrate_jni_NativeLib_connectedComponents(JNIEnv *env, jclass clazz, jobject bitmap) {
    jbroker.bitmap = bitmap;
    Mat mat = Mat();
    bitmapToMat(bitmap, mat, false);
    jbroker._drawer->connectedComponents(mat);
}