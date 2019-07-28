package com.example.nodemcuclient;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;

public class TcpClient {
    private String address;
    private Integer port;
    private Integer timeout = 2000;

    PrintWriter bufferOut;

    private Socket socket;

    public TcpClient(String address, int port) {
        this.address = address;
        this.port = port;
    }

    public void connect() {
        new ConnectThread().start();
    }

    public void send(String msg) {
        new SendMessageThread(msg).start();
    }

    public void disconnect() {
        new DisconnectThread().run();
    }

    private class ConnectThread extends Thread {
        @Override
        public void run() {
            try {
                socket = new Socket();
                socket.connect(new InetSocketAddress(InetAddress.getByName(address), port), timeout);
                bufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private class SendMessageThread extends Thread {
        private String messageLine;
        public SendMessageThread(String message) {
            this.messageLine = message + "\r";
        }

        @Override
        public void run() {
            bufferOut.print(messageLine);
            bufferOut.flush();
        }
    }

    private class DisconnectThread extends Thread {
        @Override
        public void run() {
            try {
                bufferOut.flush();
                bufferOut.close();

                socket.close();
            } catch(IOException e) {
                e.printStackTrace();
            }
        }
    }
}
