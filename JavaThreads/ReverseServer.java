import java.io.*;               // For Input/Output streams
import java.net.*;              // For networking classes, socket, ServerSocket
import java.util.concurrent.*;  // For ExecutorService and Executors

public class reverseServer {
    public static void main(String[] args) throws IOException {
        int poolSize = 3;       // Number of threads in the pool
        int port = 4444;        // Network port to listen on
        ServerSocket serverSocket = null;      // Will accept client connections 


        // Parse command line arguments, if any, override defaults
        try {   
           if (args.length >1) poolSize = Integer.parseInt(args[1]);
           if (args.length >0) port = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
           System.out.println("USAGE: java ReverseServer [poolSize] [port]");
           System.exit(1);
        }

        // Create the server socket
        try {
            serverSocket = new ServerSocket(port);
        } catch (IOException e) {
            System.out.println("Can not listen on port: " + port);
            System.out.println("USAGE:java ReverseServer [port] [poolSize]");
            System.exit(1);
        }

        // Create a thread pool to handle clients
        ExecutorService executor = Executors.newFixedThreadPool(poolSize); 

        // Main server loop, wait for clients and handle them
        try {
            while (true) {
                Socket socket = serverSocket.accept();    // Wait for a client to connect, blocks here
                // when a client connects, returns a new Socket for the client
                // the socket is the connection to that client

                executor.execute( new handler(socket) );
                // Creates a new handler for this client socket and submits it to the thread pool
                // A thread from the pool will rn it
                // main loop immediately goes back to accept() to wait for next client
            }
        } finally {
            serverSocket.close();
            executor.shutdown();
        }
    }
}
