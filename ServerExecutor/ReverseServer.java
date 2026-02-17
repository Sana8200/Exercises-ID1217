package ServerExecutor;

import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class ReverseServer {
    public static void main(String[] args) throws IOException {
        int poolSize = 3, port = 4444;
        
        ServerSocket serverSocket = new ServerSocket(port);
        
        // Create thread pool with 3 threads
        ExecutorService executor = Executors.newFixedThreadPool(poolSize);
        
        System.out.println("Server running on port " + port);
        
        while (true) {
            Socket socket = serverSocket.accept();  // Wait for client
            executor.execute(new Handler(socket));   // Handle in pool thread
        }
    }
}