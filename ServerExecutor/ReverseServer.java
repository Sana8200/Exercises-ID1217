package ServerExecutor;

import java.io.*;
import java.net.*;
import java.util.concurrent.*;

public class ReverseServer {
    public static void main(String[] args) throws IOException {
        int poolSize = 3, port = 4444;
        ServerSocket serverSocket = null;
        try {
            if (args.length > 1)
                poolSize = Integer.parseInt(args[1]);
            if (args.length > 0)
                port = Integer.parseInt(args[0]);
        } catch (NumberFormatException e) {
            System.out.println("USAGE: java ReverseServer [poolSize] [port]");
            System.exit(1);
        }
        try {
            serverSocket = new ServerSocket(port);
        } catch (IOException e) {
            System.out.println("Can not listen on port: " + port);
            System.exit(1);
        }
        ExecutorService executor = Executors.newFixedThreadPool(poolSize);
        while (true) {
            Socket socket = serverSocket.accept();
            executor.execute(new Handler(socket));
        }
    }
}