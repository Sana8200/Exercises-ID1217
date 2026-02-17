package ServerExecutor;

import java.io.*;
import java.net.*;

public class Handler implements Runnable {
    private Socket socket;
    
    public Handler(Socket socket) { this.socket = socket; }
    
    public void run() {
        try {
            BufferedReader rd = new BufferedReader(
                new InputStreamReader(socket.getInputStream()));
            PrintWriter wr = new PrintWriter(socket.getOutputStream());
            
            String str;
            while ((str = rd.readLine()) != null) {
                // Reverse the string
                for (int i = str.length(); i > 0; i--)
                    wr.print(str.charAt(i - 1));
                wr.println();
                wr.flush();
            }
            socket.close();
        } catch (IOException e) { }
    }
}
