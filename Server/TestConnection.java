import java.util.*;
import java.io.*;
import java.net.*;

public class TestConnection {

    public static void main(String[] args) {
        InputStreamReader inputStreamReader;
        OutputStreamWriter outputStreamWriter;
        BufferedReader bufferedReader;
        BufferedWriter bufferedWriter;
        System.out.println("test");
        try {
            // Create a socket to connect to the server
            Socket socket = new Socket("127.0.0.1", 22);
            inputStreamReader = new InputStreamReader(socket.getInputStream());
            outputStreamWriter = new OutputStreamWriter(socket.getOutputStream());
            bufferedReader = new BufferedReader(inputStreamReader);
            bufferedWriter = new BufferedWriter(outputStreamWriter);
            System.out.println("Connected to the server");
            bufferedWriter.write("Hello from the client");
            bufferedWriter.newLine();
            bufferedWriter.flush();
            bufferedWriter.write("login admin admin");
            bufferedWriter.newLine();
            System.out.println("test");
            bufferedWriter.flush();
            System.out.println("sent message to server");
            String message = "";
            if(bufferedReader.ready()){
                message = bufferedReader.readLine();
            }
            
            System.out.println("Server: " + message);
            while(true){
                if(bufferedReader.ready()){
                    System.out.println("Testsing");
                    if(bufferedReader.ready()){
                        message = bufferedReader.readLine();
                    }
                    
                    System.out.println(message);
                    if(message.equals("Login successful")){
                        break;
                    } else {
                        System.out.println("Server: " + message);
                    }
                }else{
                    System.out.println("Not Ready");
                }
            }
            // Close the socket
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}