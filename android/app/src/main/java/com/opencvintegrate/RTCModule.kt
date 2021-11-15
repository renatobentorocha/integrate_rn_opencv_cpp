package com.opencvintegrate

import android.graphics.Bitmap
import android.util.Base64
import com.facebook.react.bridge.*
import com.opencvintegrate.jni.NativeLib
import java.io.ByteArrayOutputStream
import android.graphics.BitmapFactory
import android.graphics.Matrix

import java.io.InputStream

import android.net.Uri
import android.util.Log
import androidx.annotation.Nullable
import com.facebook.react.modules.core.DeviceEventManagerModule
import android.media.ExifInterface
import com.facebook.react.bridge.ReactMethod

enum class OSNotificationEventTypes {
    UpdatedImage,
}

class RTCModule (private var context: ReactApplicationContext): ReactContextBaseJavaModule(context) {
    private lateinit var original: Bitmap
    private lateinit var copy: Bitmap

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
    fun start(){
        NativeLib.start {
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
    fun addListener(eventName: String?) {
        // Keep: Required for RN built in Event Emitter Calls.
    }

    @ReactMethod
    fun removeListeners(count: Int?) {
        // Keep: Required for RN built in Event Emitter Calls.
    }

    @ReactMethod
    fun load(imageUrl: String, exif: Int, promise: Promise) {
        try {
            val uri = Uri.parse(imageUrl)
            val bitmap = bitmapFromUri(uri)

            if(bitmap != null) {
                val rotated = rotate(bitmap, bitmap.width, bitmap.height, getDegreeImageOrientation(exif))

                if (rotated != null) {
                    this.original = rotated.copy(bitmap.config, true)
                    this.copy = rotated.copy(rotated.config, true)
                }

                NativeLib.load(rotated)
            } else {
                Log.e("BITMAP_NULL", "")
            }

            promise.resolve(imageUrl)
        } catch (e: Exception) {
            promise.reject("LOAD_ERROR", e.message)
        }
    }

    @ReactMethod
    fun toGray(promise: Promise) {
        try {
            NativeLib.toGray(copy)
        } catch (e: Exception) {
            promise.reject("LOAD_ERROR", e.message)
        }
    }

    @ReactMethod
    fun connectedComponents(promise: Promise) {
        try {
            NativeLib.connectedComponents(copy)
        } catch (e: Exception) {
            promise.reject("LOAD_ERROR", e.message)
        }
    }

    @ReactMethod
    fun toOriginal(promise: Promise) {
        try {
            onUpdateListener(original)
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

    fun getDegreeImageOrientation(exif: Int): Int {
        return exifToDegrees(exif)
    }

    private fun exifToDegrees(exifOrientation: Int): Int {
        return if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_90) {
            90
        } else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_180) {
            180
        } else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_270) {
            270
        } else {
            0
        }
    }

    fun rotate(bitmap: Bitmap?, width: Int, height: Int, degrees: Int): Bitmap? {
        val matrix = Matrix()
        matrix.postRotate(degrees.toFloat())
        return Bitmap.createBitmap(bitmap!!, 0, 0, width, height, matrix, true)
    }
}