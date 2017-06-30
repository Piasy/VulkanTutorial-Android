package com.github.piasy.vulkantutorial;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;

public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {

    private static final String VERTEX_SHADER = "shaders/triangle.vert.spv";
    private static final String FRAGMENT_SHADER = "shaders/triangle.frag.spv";

    private HelloTriangleApplication mTriangleApplication;
    private AssetManager mAssetManager;
    private boolean mFirstSurfaceChange;
    private boolean mFullscreen = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mAssetManager = getAssets();

        mTriangleApplication = new HelloTriangleApplication(mAssetManager, VERTEX_SHADER,
                FRAGMENT_SHADER);

        final SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface);
        surfaceView.getHolder().addCallback(this);

        findViewById(R.id.mBtnResize).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(final View v) {
                mFullscreen = !mFullscreen;

                ViewGroup.LayoutParams params = surfaceView.getLayoutParams();

                if (mFullscreen) {
                    params.width = ViewGroup.LayoutParams.MATCH_PARENT;
                    params.height = ViewGroup.LayoutParams.MATCH_PARENT;
                } else {
                    params.width = 180;
                    params.height = 320;
                }

                surfaceView.setLayoutParams(params);
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();

        mTriangleApplication.resume();
    }

    @Override
    protected void onPause() {
        super.onPause();

        mTriangleApplication.pause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        mTriangleApplication.stop();
    }

    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        mTriangleApplication.run(holder.getSurface());
        mFirstSurfaceChange = true;
    }

    @Override
    public void surfaceChanged(final SurfaceHolder holder, final int format, final int width,
            final int height) {
        if (!mFirstSurfaceChange) {
            mTriangleApplication.surfaceChanged();
        }
        mFirstSurfaceChange = false;
    }

    @Override
    public void surfaceDestroyed(final SurfaceHolder holder) {
    }
}
