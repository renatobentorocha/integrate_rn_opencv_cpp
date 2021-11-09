package com.opencvintegrate

import com.facebook.react.bridge.*

class RTCModule (private var context: ReactApplicationContext): ReactContextBaseJavaModule(context){
    override fun getName(): String {
        return "RTCModule"
    }

    @ReactMethod
    fun toGray(imageUrl: String, imageWidth: Double, imageHeight: Double, exif: Double, promise: Promise) {
        try {

            promise.resolve(imageUrl)
        } catch (e: Exception) {
            promise.reject("LOAD_ERROR", e.message)
        }
    }
}