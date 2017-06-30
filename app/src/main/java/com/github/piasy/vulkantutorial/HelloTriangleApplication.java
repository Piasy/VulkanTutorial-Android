package com.github.piasy.vulkantutorial;

import android.content.res.AssetManager;
import android.view.Surface;

/**
 * Created by Piasy{github.com/Piasy} on 30/06/2017.
 */

public class HelloTriangleApplication {

    static {
        System.loadLibrary("vulkan");
        System.loadLibrary("vulkan-tutorial");
    }

    private long mNativeHandle;

    public HelloTriangleApplication(AssetManager assetManager, String vertexShader,
            String fragmentShader) {
        mNativeHandle = create(assetManager, vertexShader, fragmentShader);
    }

    private static native long create(AssetManager assetManager, String vertexShader,
            String fragmentShader);

    private static native void run(long nativeHandle, Surface surface);

    private static native void stop(long nativeHandle);

    public void run(final Surface surface) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                HelloTriangleApplication.run(mNativeHandle, surface);
            }
        }).start();
    }

    public void stop() {
        stop(mNativeHandle);
    }
}
