package com.github.piasy.vulkantutorial;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("vulkan");
        System.loadLibrary("vulkan-tutorial");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface);
        surfaceView.getHolder().addCallback(this);
    }

    private native void runVulkan(Surface surface);

    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        runVulkan(holder.getSurface());
    }

    @Override
    public void surfaceChanged(final SurfaceHolder holder, final int format, final int width,
            final int height) {

    }

    @Override
    public void surfaceDestroyed(final SurfaceHolder holder) {

    }
}
