package ServerExecutor;

import java.io.*;
import java.net.*;

public class ReverseClient {
    public static void main(String[] args) throws IOException {
        int port = 4444;
        String host = "localhost";
        try {
            if (args.length > 0)
                host = args[0];
            if (args.length > 1)
                port = Integer.parseInt(args[1]);
        } catch (NumberFormatException e) {
            System.out.println("USAGE: java Client [host] [port]");
            System.exit(1);
        }
        try {
            Socket socket = new Socket(host, port);
            BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in));
            BufferedReader br = new BufferedReader(
                    new InputStreamReader(socket.getInputStream()));
            PrintWriter pw = new PrintWriter(socket.getOutputStream());
            String str;
            while ((str = stdin.readLine()) != null) {
                pw.println(str);
                pw.flush();
                str = br.readLine();
                System.out.println(str);
            }
        } catch (IOException e) {
            System.out.println("Can not connect to the server");
            System.exit(1);
        }
    }
}