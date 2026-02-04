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
        // Create input and output streams to communicate with the client through the socket
        // rd reads data sent by the client, wr sends data back to the client
         BufferedReader rd = new BufferedReader(new InputStreamReader(socket.getInputStream()));
         PrintWriter wr = new PrintWriter(socket.getOutputStream());
         String str;
        
        // rd.readLine() reads a line of text from the client, blocks until a line is available
        // The loop continues until the client closes the connection (rd.readLine() returns null)
         while ((str = rd.readLine()) != null) {
            for ( int i=str.length(); i > 0; i-- ) wr.print(str.charAt(i-1));
                wr.println();   
                wr.flush();      // flush() forces the output to be sent immediately, without waiting for the buffer to fill up
            }
            socket.close();
         } catch ( IOException e ) {;}
    }
}
