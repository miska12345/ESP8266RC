package com.example.nodemcuclient;

import android.annotation.SuppressLint;
import java.net.InetAddress;
import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class SocketAct extends AppCompatActivity {

    boolean online = false;
    PrintWriter bufferOut;

    View contextView;
    Socket sock;
    Button forward;
    Button backward;
    Button left;
    Button right;

    private static final String INS_FORWARD_PRESSED = "FORWARD_P";
    private static final String INS_BACKWARED_PRESSED = "BACKWARD_P";
    private static final String INS_LEFT_PRESSED = "LEFT_P";
    private static final String INS_LEFT_RELEASED = "LEFT_R";
    private static final String INS_RIGHT_PRESSED = "RIGHT_P";
    private static final String INS_RIGHT_RELEASED = "RIGHT_R";
    private static final String INS_NULL = "STOP";
    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        String server;
        int port;

        forward = findViewById(R.id.button_forward);
        backward = findViewById(R.id.button_backward);
        left = findViewById(R.id.button_left);
        right = findViewById(R.id.button_right);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_socket);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        contextView = findViewById(R.id.view);
        server = getIntent().getStringExtra("Server");
        port = getIntent().getIntExtra("Port", -1);

        if (port == -1) {
            throw new IllegalArgumentException();
        }

        if (doSocket(server, port)) {
            // Connected!
            TextView lb = (TextView)findViewById(R.id.label_status);
            lb.setTextColor(this.getResources().getColor(R.color.colorGreen));
            setButtonEvents();
        }
    }

    /*
     * Establish remote connection
     * @requires server != null, port > 0
     * @param server the server to connect
     * @param port the port the connect
     * @return true iff connection has been established, false otherwise
     */
    private boolean doSocket(String server, int port) {
        try {
            InetAddress serverAddr = InetAddress.getByName(server);
            sock = new Socket(serverAddr, port);
            bufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(sock.getOutputStream())), true);
            Snackbar.make(contextView, "Connection established", Snackbar.LENGTH_SHORT).show();
            online = true;
            return true;
        } catch (UnknownHostException e) {
            Snackbar.make(contextView, "Unknown host", Snackbar.LENGTH_SHORT).show();
        } catch (IOException e) {
            Snackbar.make(contextView, e.getMessage(), Snackbar.LENGTH_SHORT).show();
        }
        return false;
    }

    /*
     * Initialize event handlers for all buttons
     */
    private void setButtonEvents() {
        forward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent e) {
                if(e.getAction() == MotionEvent.ACTION_DOWN) {
                    send(INS_FORWARD_PRESSED);
                    System.out.println("pressed");
                } else if (e.getAction() == MotionEvent.ACTION_UP) {
                    System.out.println("unpressed");
                    send(INS_NULL);
                }
                return true;
            }
        });

        backward.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent e) {
                if(e.getAction() == MotionEvent.ACTION_DOWN) {
                    send(INS_BACKWARED_PRESSED);
                } else if (e.getAction() == MotionEvent.ACTION_UP) {
                    send(INS_NULL);
                }
                return true;
            }
        });

        left.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent e) {
                if(e.getAction() == MotionEvent.ACTION_DOWN) {
                    send(INS_LEFT_PRESSED);
                } else if (e.getAction() == MotionEvent.ACTION_UP) {
                    send(INS_LEFT_RELEASED);
                }
                return true;
            }
        });

        right.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent e) {
                if(e.getAction() == MotionEvent.ACTION_DOWN) {
                    send(INS_RIGHT_PRESSED);
                } else if (e.getAction() == MotionEvent.ACTION_UP) {
                    send(INS_RIGHT_RELEASED);
                }
                return true;
            }
        });
    }

    /*
     * Send the given string to NodeMCU server
     * @requires str != null
     * @param str the string to send
     */
    private void send(final String str) {
        if (!online)
            return;

        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (bufferOut != null) {
                    bufferOut.write(str + "\r");
                    bufferOut.flush();
                }
            }
        };
        Thread thread = new Thread(runnable);
        thread.start();
    }
}
