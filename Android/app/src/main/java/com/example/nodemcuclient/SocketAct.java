package com.example.nodemcuclient;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import io.github.controlwear.virtual.joystick.android.JoystickView;

public class SocketAct extends AppCompatActivity {

    private static String server;
    private static int port;
    private static final String INS_FORWARD_PRESSED = "FORWARD_P";
    private static final String INS_BACKWARED_PRESSED = "BACKWARD_P";
    private static final String INS_LEFT_PRESSED = "LEFT_P";
    private static final String INS_LEFT_RELEASED = "LEFT_R";
    private static final String INS_RIGHT_PRESSED = "RIGHT_P";
    private static final String INS_RIGHT_RELEASED = "RIGHT_R";
    private static final String INS_NULL = "STOP";

    TcpClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        JoystickView joy;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_socket);

        server = getIntent().getStringExtra("Server");
        port = getIntent().getIntExtra("Port", -1);


        client = new TcpClient(server, port);
        client.connect();

        joy = findViewById(R.id.joystick1);
        joy.setOnMoveListener(new JoystickView.OnMoveListener() {
            @Override
            public void onMove(int angle, int strength) {
                ((TextView)findViewById(R.id.label)).setText(angle + " | " + strength);
                if (strength == 0) {
                    client.send(INS_NULL);
                } else if (0 <= angle && angle <= 180) {
                    if (Math.abs(angle - 90) < 30) {
                        client.send(INS_RIGHT_RELEASED);
                        client.send(INS_FORWARD_PRESSED);
                    } else if (angle < 90) {
                        client.send(INS_RIGHT_PRESSED);
                    } else {
                        client.send(INS_LEFT_PRESSED);
                    }
                } else {
                    if (Math.abs(angle - 270) < 25) {
                        client.send(INS_RIGHT_RELEASED);
                        client.send(INS_BACKWARED_PRESSED);
                    } else if (angle < 270) {
                        client.send(INS_LEFT_PRESSED);
                    } else {
                        client.send(INS_RIGHT_PRESSED);
                    }
                }

            }
        });
    }
}
