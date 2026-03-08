package ThreadPool;

import java.io.*;
import java.net.*;

public class handler {
    private Socket socket;

    public handler(Socket socket) {
        this.socket = socket;
    }

    public void run() {
        try {
            BufferedReader rd = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter wr = new PrintWriter(socket.getOutputStream());

            String str;
            while ((str = rd.readLine()) != null) {
                // reverse the string and send it back to the clinet
                for (int i = str.length(); i > 0; i--)
                    wr.print(str.charAt(i - 1));
                wr.println();
                wr.flush();
            }
            socket.close();
        } catch (IOException e) {
            ;
        }
    }
}
