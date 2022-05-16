package com.boolb;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private BluetoothAdapter myBluetooth = null;
    private Set<BluetoothDevice> pairedDevices;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    String address = null;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    ProgressBar progress;
    TextView status;
    Button reconnect;
    TextView value;
    Loop loop;

    private class ConnectBT extends AsyncTask<Void, Void, Void> {
        private boolean ConnectSuccess = true;

        @Override
        protected void onPreExecute() {
        }

        @Override
        protected Void doInBackground(Void... devices) {
            try {
                if (btSocket == null || !isBtConnected) {
                    BluetoothDevice device = myBluetooth.getRemoteDevice(address);
                    btSocket = device.createInsecureRfcommSocketToServiceRecord(myUUID);
                    btSocket.connect();
                }
            } catch (IOException e) {
                ConnectSuccess = false;
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            if (!ConnectSuccess) {
                status.setText("status: failed");
                progress.setVisibility(View.INVISIBLE);
                reconnect.setEnabled(true);
            } else {
                status.setText("status: connected");
                progress.setVisibility(View.INVISIBLE);
                reconnect.setEnabled(true);
                isBtConnected = true;

                loop = new Loop();
                loop.execute();
            }
        }
    }

    private void connect() {
        status = (TextView) findViewById(R.id.status);
        progress = (ProgressBar) findViewById(R.id.progress);
        reconnect = (Button) findViewById(R.id.reconnect);
        value = (TextView) findViewById(R.id.value);
        status.setText("status: connecting");
        progress.setVisibility(View.VISIBLE);
        reconnect.setEnabled(false);
        value.setText("-");
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        if (myBluetooth == null) {
            Toast.makeText(getApplicationContext(), "Bluetooth Device Not Available", Toast.LENGTH_LONG).show();
            status.setText("status: failed");
            progress.setVisibility(View.INVISIBLE);
            reconnect.setEnabled(true);
        } else {
            if (!myBluetooth.isEnabled()) {
                status.setText("status: failed");
                progress.setVisibility(View.INVISIBLE);
                reconnect.setEnabled(true);
                Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(turnBTon, 1);
            } else {
                pairedDevices = myBluetooth.getBondedDevices();
                boolean found = false;
                for (BluetoothDevice bt : pairedDevices) {
                    if (bt.getName().equals("HC-05")) {
                        address = bt.getAddress();
                        ConnectBT connect = new ConnectBT();
                        connect.execute();
                        found = true;
                    }
                }
                if (found == false) {
                    Toast.makeText(getApplicationContext(), "Device not found", Toast.LENGTH_LONG).show();
                    status.setText("status: failed");
                    progress.setVisibility(View.INVISIBLE);
                    reconnect.setEnabled(true);
                }
            }
        }
    }

    private class Loop extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... devices) {
            if (btSocket != null) {
                try {
                    while (true) {
                        byte[] buffer = new byte[100];
                        InputStream inputStream = btSocket.getInputStream();
                        int length = inputStream.read(buffer);
                        String s = new String(buffer, 0, length - 1);
                        if (s.contains("RON"))
                            value.setText(s.substring(s.indexOf("#") + 1, s.indexOf("RON")) + " RON");
                    }
                } catch (IOException e) {
                    Toast.makeText(getApplicationContext(), "Device not available", Toast.LENGTH_LONG).show();
                }
            }
            return null;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setTitle("Bank Account");
        connect();
    }

    public void reconnect(android.view.View view) throws IOException {
        if (isBtConnected == true) {
            btSocket.close();
            btSocket = null;
            isBtConnected = false;
            loop.cancel(true);
        }
        connect();
    }

    public void handleTopup(android.view.View view) {
        if (btSocket != null) {
            try {
                String msg = String.valueOf(100) + ";";
                btSocket.getOutputStream().write(String.valueOf(msg).getBytes());
                btSocket.getOutputStream().flush();
            } catch (IOException e) {
                Toast.makeText(getApplicationContext(), "Device not available", Toast.LENGTH_LONG).show();
            }
        }
    }
}

