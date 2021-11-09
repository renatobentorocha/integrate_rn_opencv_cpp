#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

using namespace cv;

struct JavaCallback {
    jmethodID method;
    jobject callback;
};

struct JavaSimulator {
    JNIEnv *env;
};

static JavaCallback jdrawer;
static JavaSimulator jsim;

void l(String val) {
    __android_log_print(ANDROID_LOG_DEBUG, "PQN", "%s", val.c_str());
}

void bitmapToMat(jobject bitmap, Mat m_addr, jboolean needUnPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;
    Mat dst = m_addr;

    try {
        l("nBitmapToMat");
        CV_Assert( AndroidBitmap_getInfo(jsim.env, bitmap, &info) >= 0 );
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                   info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        CV_Assert( AndroidBitmap_lockPixels(jsim.env, bitmap, &pixels) >= 0 );
        CV_Assert( pixels );
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
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        l("nBitmapToMat caught unknown exception (...)");
        return;
    }
}

void matToBitmap(Mat m_addr, jobject bitmap, jboolean needPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void *pixels = 0;
    Mat src = m_addr;

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
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(jsim.env, bitmap);
        l("nMatToBitmap caught unknown exception (...)");
        return;
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_opencvintegrate_jni_NativeLib_load(JNIEnv *env, jclass clazz, jobject on_update_listener) {
    jsim.env = env;
    jdrawer.callback = jsim.env->NewGlobalRef(on_update_listener);
    jdrawer.method = jsim.env->GetMethodID(env->GetObjectClass(jdrawer.callback), "onUpdate", "(Landroid/graphics/Bitmap;)V");
}

extern "C"
JNIEXPORT void JNICALL Java_com_opencvintegrate_jni_NativeLib_toGray(JNIEnv *env, jclass clazz, jobject bitmap) {
    jsim.env = env;
    Mat mat;
    bitmapToMat(bitmap, mat, false);

    cvtColor(mat, mat, COLOR_RGBA2GRAY);
    matToBitmap(mat, bitmap, false);
    jsim.env->CallVoidMethod(jdrawer.callback, jdrawer.method, bitmap);
}