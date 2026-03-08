package ThreadPool;
import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class reverseServer {
    public static void main(String[] args) throws IOException {
        int poolSize = 3, port = 4444;
        try {
            if (args.length > 1) poolSize = Integer.parseInt(args[1]);
            if (args.length > 0) port = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            System.out.println("USAGE: java ReverseServer [poolSize] [port]");
            System.exit(1);
        }
        
        ServerSocket serverSocket = new ServerSocket(port);
        
        // Create a thread pool with fixed size
        ExecutorService executor = Executors.newFixedThreadPool(poolSize);
        
        while (true) {
            Socket socket = serverSocket.accept();
            executor.execute(new Handler(socket));  // Submit to pool
        }
    }
}