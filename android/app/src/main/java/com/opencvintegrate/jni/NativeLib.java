package com.opencvintegrate.jni;

import android.graphics.Bitmap;

import androidx.annotation.Keep;

public class NativeLib {
    public static native synchronized void init(OnUpdateListener onUpdateListener);
    public static native synchronized void load(Bitmap bitmap);
    public static native synchronized void toGray(Bitmap bitmap);
    public static native synchronized void connectedComponents(Bitmap bitmap);

    public interface OnUpdateListener {
        @Keep
        void onUpdate(Bitmap bitmap);
    }
}
