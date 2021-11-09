package com.opencvintegrate

import android.graphics.Bitmap
import android.util.Base64
import com.facebook.react.bridge.*
import com.opencvintegrate.jni.NativeLib
import java.io.ByteArrayOutputStream
import android.graphics.BitmapFactory

import java.io.InputStream

import android.net.Uri
import android.util.Log
import androidx.annotation.Nullable
import com.facebook.react.modules.core.DeviceEventManagerModule

enum class OSNotificationEventTypes {
    UpdatedImage,
}

class RTCModule (private var context: ReactApplicationContext): ReactContextBaseJavaModule(context) {
    override fun getName(): String {
        return "NativeLibModule"
    }

    private fun sendEvent(reactContext: ReactContext,
                          eventName: String,
                          @Nullable params: WritableMap) {
        reactContext
                .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter::class.java)
                .emit(eventName, params)
    }

    @ReactMethod
    fun init(){
        NativeLib.init {
            if(it!= null) {
                onUpdateListener(it)
            }
        }
    }

    private fun onUpdateListener(bitmap: Bitmap) {
        val base64 = bitmapToBase64(bitmap)
        val params = Arguments.createMap()
        params.putString("image_base_64", base64)
        sendEvent(context, OSNotificationEventTypes.UpdatedImage.name, params)
    }

    @ReactMethod
    fun toGray(imageUrl: String, promise: Promise) {
        try {
            val uri = Uri.parse(imageUrl)
            val bitmap = bitmapFromUri(uri)

            if(bitmap != null) {
                NativeLib.toGray(bitmap)
            } else {
                Log.e("BITMAP_NULL", "")
            }

            promise.resolve(imageUrl)
        } catch (e: Exception) {
            promise.reject("LOAD_ERROR", e.message)
        }
    }

    private fun bitmapToBase64(bitmap: Bitmap): String {
        val byteArrayOutputStream = ByteArrayOutputStream()
        bitmap.compress(Bitmap.CompressFormat.JPEG, 100, byteArrayOutputStream)
        val byteArray: ByteArray = byteArrayOutputStream.toByteArray()
        return Base64.encodeToString(byteArray, Base64.DEFAULT)
    }

    @Throws(Exception::class)
    fun bitmapFromUri(uri: Uri?): Bitmap? {
        return try {
            val inputStream: InputStream? = uri?.let { context.contentResolver.openInputStream(it) }
            val bitmap = Bitmap.createBitmap(BitmapFactory.decodeStream(inputStream)).copy(Bitmap.Config.ARGB_8888, true)

            inputStream?.close()
            bitmap
        } catch (e: Exception) {
            throw e
        }
    }
}