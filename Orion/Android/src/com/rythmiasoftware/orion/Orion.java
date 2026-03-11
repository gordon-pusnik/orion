package com.rythmiasoftware.orion;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.rythmiasoftware.orion.GL2JNILib;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;

public class Orion extends Activity {
	private GLSurfaceView mGLView;
	private static AssetManager mgr;
	public ScaleGestureDetector mScaleDetector;
	public float mScaleFactor;
	private boolean mScaling;
	private int mMoveCount;
	private float mPanInitialX;
	private float mPanInitialY;
	private float fX, fY, sX, sY;
	private int ptrID1, ptrID2;
	private float mAngle;
	private boolean firstTouch;
	private static final int INVALID_POINTER_ID = -1;
	private static String filesDir;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        mgr = getResources().getAssets();
        
        mGLView = new OpenGLSurfaceView(this);
        setContentView(mGLView);        
        
        ptrID1 = INVALID_POINTER_ID;
        ptrID2 = INVALID_POINTER_ID;
    }
    
    @Override
    public void finalize() throws Throwable {
    	GL2JNILib.destroy();
    	super.finalize();
    }
    
    @Override
    protected void onStart() {
        super.onStart();  // Always call the superclass method first
    }
 
    protected void onStop() {
        super.onStop();  // Always call the superclass method first
    }
     
    public class OpenGLSurfaceView extends GLSurfaceView {
    	public OpenGLSurfaceView(Context context) {
    		super(context);
    		
    		setEGLContextClientVersion(2);
    		
    		setRenderer(new OpenGLRenderer());
    		
            mScaleDetector = new ScaleGestureDetector(context, new ScaleListener());
            mScaleFactor = 1.0f;
            
            filesDir = context.getFilesDir().toString();
    	}
    }
    
    public boolean onTouchEvent (MotionEvent e) {
    	boolean ret = false;

    	boolean gesturesEnabled = GL2JNILib.gesturesEnabled();
//    	System.out.println("gestures enabled: " + gesturesEnabled);
    	
    	if (gesturesEnabled) {
        	// Let scale detector peek at the event first.
        	mScaleDetector.onTouchEvent(e);
        	
    		ret = handleGestures(e);
    	}
    	else {
    		ret = handleTouches(e);
    	}

    	return ret;
    }
  
    public boolean handleTouches(MotionEvent e) {
    	boolean ret = false;
    
      	int count = e.getPointerCount();   	
    	for (int i = 0; i < count; i++) { 
	        float x = e.getX(i);
	        float y = e.getY(i);
	        
	        switch (e.getAction() & MotionEvent.ACTION_MASK) {
	        	case MotionEvent.ACTION_DOWN:
	        		System.out.println("Down");
	        		GL2JNILib.touchesBegan(x, y);
	        		ret = true;
	        		break;
	            case MotionEvent.ACTION_MOVE:
	        		System.out.println("Move");
	            	GL2JNILib.touchesMoved(x, y);
	        		ret = true;
	        		break;
	            case MotionEvent.ACTION_UP:
	        		System.out.println("Up");
	        		GL2JNILib.touchesEnded(x, y);
	        		ret = true;
	        		break;
	            case MotionEvent.ACTION_POINTER_DOWN:
	            	System.out.println("Pointer down");
	        		GL2JNILib.touchesBegan(x, y);
	        		ret = true;	 
	            	break;
	            case MotionEvent.ACTION_POINTER_UP:
	            	System.out.println("Pointer up");
	        		GL2JNILib.touchesEnded(x, y);
	                ret = true;	                
	            	break;
	        };
    	}

    	return ret;
    }
    
    public boolean handleGestures(MotionEvent e) {
    	boolean ret = false;
    
        float x = e.getX(0);
        float y = e.getY(0);
        
        switch (e.getAction() & MotionEvent.ACTION_MASK) {
        	case MotionEvent.ACTION_DOWN:
        		System.out.println("Down");
        		GL2JNILib.touchesBegan(x, y);
        		ret = true;

                sX = e.getX();
                sY = e.getY();
                ptrID1 = e.getPointerId(0);
                mAngle = 0;
                firstTouch = true;

        		break;
            case MotionEvent.ACTION_MOVE:
            	if (mScaling) {
            		System.out.println("Scale");
            		GL2JNILib.pinchChanged(mScaleFactor);
            		GL2JNILib.panChanged(x - mPanInitialX, y - mPanInitialY);
            		
                    if (ptrID1 != INVALID_POINTER_ID && ptrID2 != INVALID_POINTER_ID){
                        float nfX, nfY, nsX, nsY;
                        nsX = e.getX(e.findPointerIndex(ptrID1));
                        nsY = e.getY(e.findPointerIndex(ptrID1));
                        nfX = e.getX(e.findPointerIndex(ptrID2));
                        nfY = e.getY(e.findPointerIndex(ptrID2));
                        if (firstTouch) {
                            mAngle = 0;
                            firstTouch = false;
                        } 
                        else {
                            mAngle += -Math.toRadians(angleBetweenLines(fX, fY, sX, sY, nfX, nfY, nsX, nsY));
                            
                            System.out.println("Rotation: " + mAngle);
                            
                            GL2JNILib.rotateChanged(mAngle);
                        }
                        
                        fX = nfX;
                        fY = nfY;
                        sX = nsX;
                        sY = nsY;
                    }
            	}
            	else {
            		// Consume a few move events, otherwise after scale has ended, 
            		// a few moves are generated which cause the model to jump.
            		if (mMoveCount > 5) {
            			System.out.println("Move");
            			GL2JNILib.touchesMoved(x, y);
            		}
            		mMoveCount++;
            	}
        		ret = true;
        		break;
            case MotionEvent.ACTION_UP:
            	System.out.println("Up");
        		GL2JNILib.touchesEnded(x, y);

                ptrID1 = INVALID_POINTER_ID;
                
                ret = true;
        		break;
            case MotionEvent.ACTION_POINTER_DOWN:
            	System.out.println("Pointer down");
            	mScaling = true;
            	mScaleFactor = 1.0f;
            	mPanInitialX = x;
            	mPanInitialY = y;
            	GL2JNILib.pinchBegan();
            	GL2JNILib.panBegan();
            	GL2JNILib.rotateBegan();
            	
                fX = e.getX();
                fY = e.getY();
                ptrID2 = e.getPointerId(e.getActionIndex());
                mAngle = 0;
                firstTouch = true;

            	break;
            case MotionEvent.ACTION_POINTER_UP:
            	System.out.println("Pointer up");
            	mScaling = false;
            	mMoveCount = 0;
            	GL2JNILib.pinchEnded();
            	GL2JNILib.panEnded();
            	GL2JNILib.rotateEnded();
                ptrID2 = INVALID_POINTER_ID;
            	break;
        };
        
    	return ret;
    }

    float getMidpoint(float a, float b) {
        return (a + b) / 2;
    }

    float findAngleDelta(float angle1, float angle2) {
        float From = ClipAngleTo0_360(angle2);
        float To   = ClipAngleTo0_360(angle1);

        float Dist  = To - From;

        if (Dist < -180.0f) {
            Dist += 360.0f;
        }
        else if (Dist > 180.0f) {
            Dist -= 360.0f;
        }

        return Dist;
    }
    
    float ClipAngleTo0_360(float Angle) { 
        return Angle % 360.0f;
    }

    float angleBetweenLines (float fx1, float fy1, float fx2, float fy2, float sx1, float sy1, float sx2, float sy2) {
           float angle1 = (float) Math.atan2( (fy1 - fy2), (fx1 - fx2) );
           float angle2 = (float) Math.atan2( (sy1 - sy2), (sx1 - sx2) );

           return findAngleDelta((float)Math.toDegrees(angle1),(float)Math.toDegrees(angle2));
    }
    
    private static class OpenGLRenderer implements GLSurfaceView.Renderer {
		public void onDrawFrame(GL10 arg0) {
	    	long time = System.currentTimeMillis();
	    	GL2JNILib.update(time);
			GL2JNILib.render();
		}
		
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GL2JNILib.init(width, height, mgr, filesDir);
		}
		
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		}
    }
    
    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
        @Override
        public boolean onScale(ScaleGestureDetector detector) {
            mScaleFactor *= detector.getScaleFactor();
            
            // Don't let the object get too small or too large.
            mScaleFactor = Math.max(0.1f, Math.min(mScaleFactor, 20.0f));
            
            System.out.println("Scale factor: " + mScaleFactor);
            
            return true;
        }
    }
 }