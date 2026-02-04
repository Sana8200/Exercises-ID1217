import java.io.*;
import java.net.*;

// Handler implements Runnable - This means it can be executed by a thread!

// Each handler is created with a socket, to connection to one specifi client
public class Handler implements Runnable {
	private Socket socket;
	public Handler(Socket socket) {  this.socket = socket; }

    // The run() method is what the thread will execute
	public void run() {
	   try {
         BufferedReader rd = new BufferedReader(new InputStreamReader(socket.getInputStream()));
         PrintWriter wr = new PrintWriter(socket.getOutputStream());
         String str;
         while ((str = rd.readLine()) != null) {
            for ( int i=str.length(); i > 0; i-- ) wr.print(str.charAt(i-1));
                wr.println(); 
                wr.flush();
            }
            socket.close();
         } catch ( IOException e ) {;}
    }
}
