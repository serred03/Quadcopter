package com.example.serred.quadcontrols;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.SeekBar;
import android.widget.TextView;

public class MyActivity extends ActionBarActivity implements SensorEventListener {

    private VerticalSeekBar ThrottleSeekBar;
    private VerticalSeekBar RudderSeekBar;
    private TextView ThrottleTextView;
    private TextView RudderTextView;
    private TextView AileronTextView;
    private TextView ElevatorTextView;

    //Values to be sent to quadcopter
    int TH;
    int RD;

    //Orientation sensor initialization
    private SensorManager mSensorManager;
    Sensor accelerometer;
    Sensor magnetometer;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my);

        initializeVariables();


        ThrottleSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                //On progress update TH value.
                TH = mapThrottleValue(i);
                String th_string = "TH: " + Integer.toString(TH);
                ThrottleTextView.setText(th_string);

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        //Function to get the progress of the Rudder Seekbar.
        RudderSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                //On progress update RD value.
                RD = mapThrottleValue(i);
                String rd_string = "RD: " + Integer.toString(RD);
                RudderTextView.setText(rd_string);

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });



    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_my, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void initializeVariables() {
        ThrottleSeekBar = (VerticalSeekBar) findViewById(R.id.Throttle_seek);
        RudderSeekBar = (VerticalSeekBar) findViewById(R.id.Rudder_seek);
        ThrottleTextView = (TextView) findViewById(R.id.throttle_textView);
        AileronTextView = (TextView) findViewById(R.id.aileron_textView);
        RudderTextView = (TextView) findViewById(R.id.rudder_textView);
        ElevatorTextView = (TextView) findViewById(R.id.elevator_textView);
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        accelerometer = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        magnetometer = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_UI);
        mSensorManager.registerListener(this, magnetometer, SensorManager.SENSOR_DELAY_UI);
    }

    @Override
    protected void onPause(){
        super.onPause();
        mSensorManager.unregisterListener(this);
    }
    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    float [] mGravity;
    float [] mGeomagnetic;
    Float azimut;
    Float elevator_value;
    Float aileron_value;

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER)
            mGravity = event.values;
        if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD)
            mGeomagnetic = event.values;
        if (mGravity != null && mGeomagnetic != null) {
            float R[] = new float[9];
            float I[] = new float[9];
            boolean success = SensorManager.getRotationMatrix(R, I, mGravity, mGeomagnetic);
            if (success) {
                float orientation[] = new float[3];
                SensorManager.getOrientation(R, orientation);
                azimut = orientation[0];
                elevator_value= orientation[1]; // orientation contains: azimut, pitch and roll
                aileron_value = orientation[2]; //aileron = roll, elevator = pitch


                String ai = "AI: " + Float.toString(aileron_value);
                String el = "EL: " + Float.toString(elevator_value);

                AileronTextView.setText(ai);
                ElevatorTextView.setText(el);
            }

        }
    }


    public int mapThrottleValue(int v){
        return  v * 72/100+56;
    }

}
