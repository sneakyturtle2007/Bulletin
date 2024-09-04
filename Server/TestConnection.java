import java.util.*;
import java.io.*;
import java.net.*;

public class TestConnection {
    
    public static void main(String[] args) {
        try {
            // Create a socket to connect to the server
            Socket socket = new Socket("127.0.0.1", 22);

            System.out.println("Connected to the server");

            // Close the socket
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}