package com.github.piasy.vulkantutorial;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private static final String VERTEX_SHADER = "shaders/triangle.vert.spv";
    private static final String FRAGMENT_SHADER = "shaders/triangle.frag.spv";

    private HelloTriangleApplication mTriangleApplication;
    private AssetManager mAssetManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mAssetManager = getAssets();

        mTriangleApplication = new HelloTriangleApplication(mAssetManager, VERTEX_SHADER,
                FRAGMENT_SHADER);

        SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface);
        surfaceView.getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        mTriangleApplication.run(holder.getSurface());
    }

    @Override
    public void surfaceChanged(final SurfaceHolder holder, final int format, final int width,
            final int height) {
    }

    @Override
    public void surfaceDestroyed(final SurfaceHolder holder) {
    }
}
