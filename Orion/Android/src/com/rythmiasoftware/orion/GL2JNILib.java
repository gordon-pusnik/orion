package com.rythmiasoftware.orion;

import android.content.res.AssetManager;

public class GL2JNILib {
    static {
        System.loadLibrary("gl2jni");
    }
    
   /**
    * @param width the current view width
    * @param height the current view height
    */
    public static native void init(int width, int height, AssetManager mgr, String dir);
    public static native void update(long ms);
    public static native void render();
	public static native void touchesBegan(float x, float y);
	public static native void touchesMoved(float x, float y);
	public static native void touchesEnded(float x, float y);
	public static native void pinchBegan();
	public static native void pinchChanged(float scale);
	public static native void pinchEnded();
	public static native void panBegan();
	public static native void panChanged(float x, float y);
	public static native void panEnded();
	public static native void rotateBegan();
	public static native void rotateChanged(float rotation);
	public static native void rotateEnded();
	public static native void destroy();
	public static native boolean gesturesEnabled();
}
