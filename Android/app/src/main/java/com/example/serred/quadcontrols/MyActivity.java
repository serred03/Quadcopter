package com.example.serred.quadcontrols;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class MyActivity extends ActionBarActivity implements SensorEventListener {

    private VerticalSeekBar ThrottleSeekBar;
    private VerticalSeekBar RudderSeekBar;
    private TextView ThrottleTextView;
    private TextView RudderTextView;
    private TextView AileronTextView;
    private TextView ElevatorTextView;
    private UDP_Client client;
    private Button EnableAcc;
    private Button DisableAcc;

    //Values to be sent to quadcopter
    int TH_value;
    int RD_value;
    int EL_value;
    int AI_value;

    //constants for mapping control values
    int limits = 80; // represents +/-80 on posible values of orientation
    int arduino_range = 75; //range of orientation values on arduino
    int arduino_min = 55;   //minimum value on arduino

    //Orientation sensor initialization
    private SensorManager mSensorManager;
    Sensor accelerometer;
    Sensor magnetometer;

    //Activate UDP send.
    boolean AccEnable = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my);

        initializeVariables();


        ThrottleSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                //On progress update TH value.
                TH_value = mapControlValues(i, 't');;
                String th_string = "TH_" + Integer.toString(TH_value);
                client.Message = th_string;
                client.SendData();
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
                RD_value = mapControlValues(i,'r');
                String rd_string = "RD_" + Integer.toString(RD_value);
                client.Message = rd_string;
                client.SendData();
                RudderTextView.setText(rd_string);

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        EnableAcc.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                AccEnable = true;
            }
        });

        DisableAcc.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                AccEnable = false;
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
        client = new UDP_Client();
        EnableAcc = (Button) findViewById(R.id.Enable_Acc);
        DisableAcc = (Button) findViewById(R.id.Disable_Acc);
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

                //aileron range 0.5(left) : -0.5(Right)
                //elevator range 0.7(Front) : -0.7(Back)
                float aileron_value= orientation[1]; // orientation contains: azimut, pitch and roll
                float elevator_value = orientation[2]; //aileron = roll, elevator = pitch

                AI_value = mapControlValues(aileron_value, 'a');
                EL_value = mapControlValues(elevator_value, 'e');

                String AI_text = "AI_" + Integer.toString(AI_value) + " ";
                String EL_text = "EL_" + Integer.toString(EL_value);

                if(AccEnable){
                    String command = AI_text + EL_text;

                    client.Message = command;
                    client.SendData();
                }else{
                    String command = "AI_93 EL_93";
                    client.Message = command;
                    client.SendData();
                }



                AileronTextView.setText(AI_text);
                ElevatorTextView.setText(EL_text);
            }

        }
    }

    public int mapControlValues(float v, char channel) {

        /*Function converts values from orientation sensors into
        * values that can be processed by the arduino.
        * */
        int mapped_value = 0;
        switch(channel) {

            case 't'://throttle
                mapped_value = Math.round(v * 70/100+58);
                break;
            case 'a'://aileron
                v *= -100;
                if(v < (limits*-1)) {
                    v = -80;
                } else if (v > limits) {
                    v = 80;
                }
                mapped_value = Math.round(((v+limits)*arduino_range)/(2*limits) + arduino_min);
                break;
            case 'e'://elevator
                v *= 100;
                if(v < (limits*-1)) {
                    v = -80;
                } else if (v > limits) {
                    v = 80;
                }
                mapped_value = Math.round(((v+limits)*arduino_range)/(2*limits) + arduino_min);
                break;
            case 'r'://rudder
                mapped_value = Math.round((v* arduino_range)/(100) + arduino_min);
                break;
            default:
                break;
        }

        return mapped_value;
    }


}
