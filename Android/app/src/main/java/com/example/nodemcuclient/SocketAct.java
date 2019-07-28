package com.example.nodemcuclient;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;

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

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_socket);

        server = getIntent().getStringExtra("Server");
        port = getIntent().getIntExtra("Port", -1);

        client = new TcpClient(server, port);
        client.connect();

        Button forward = findViewById(R.id.button_forward);
        forward.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    client.send(INS_FORWARD_PRESSED);
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    client.send(INS_NULL);
                    return true;
                }
                return false;
            }
        });

        Button backward = findViewById(R.id.button_backward);
        backward.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    client.send(INS_BACKWARED_PRESSED);
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    client.send(INS_NULL);
                    return true;
                }
                return false;
            }
        });

        Button left = findViewById(R.id.button_right);
        left.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    client.send(INS_LEFT_PRESSED);
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    client.send(INS_LEFT_RELEASED);
                    return true;
                }
                return false;
            }
        });

        Button right = findViewById(R.id.button_right);
        right.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN ) {
                    client.send(INS_RIGHT_PRESSED);
                    return true;
                } else if (event.getAction() == MotionEvent.ACTION_UP ) {
                    client.send(INS_RIGHT_RELEASED);
                    return true;
                }
                return false;
            }
        });
    }
}
