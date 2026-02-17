package ServerExecutor;

import java.io.*;
import java.net.*;

public class ReverseClient {
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket("localhost", 4444);
        
        BufferedReader stdin = new BufferedReader(
            new InputStreamReader(System.in));
        BufferedReader br = new BufferedReader(
            new InputStreamReader(socket.getInputStream()));
        PrintWriter pw = new PrintWriter(socket.getOutputStream());
        
        String str;
        while ((str = stdin.readLine()) != null) {
            pw.println(str);     // Send to server
            pw.flush();
            str = br.readLine(); // Read reversed string
            System.out.println(str);
        }
    }
}
