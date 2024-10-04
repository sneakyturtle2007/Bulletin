import java.util.*;
import java.io.*;
import java.net.*;

public class TestConnection {
    
    static void login(BufferedReader bufferedReader, BufferedWriter bufferedWriter) throws IOException {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter your username: ");
        String username = scanner.nextLine();
        System.out.println("Enter your password: ");
        String password = scanner.nextLine();
        bufferedWriter.write(username);
        bufferedWriter.newLine();
        bufferedWriter.flush();
        bufferedWriter.write(password);
        bufferedWriter.newLine();
        bufferedWriter.flush();
        while(true){
            if(bufferedReader.ready()){
                String message = bufferedReader.readLine();
                if(message.equals("exit")){
                    System.out.println("Server: " + message);
                    bufferedWriter.write("exit");
                    bufferedWriter.newLine();
                    bufferedWriter.flush();
                    break;
                } else {
                    System.out.println("Server: " + message);
                }
            }
        }
        scanner.close();
    }
    public static void main(String[] args) {
        InputStreamReader inputStreamReader;
        OutputStreamWriter outputStreamWriter;
        BufferedReader bufferedReader;
        BufferedWriter bufferedWriter;
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
            bufferedWriter.write("Login");
            bufferedWriter.newLine();
            bufferedWriter.flush();
            while(true){
                if(bufferedReader.ready()){
                    String message = bufferedReader.readLine();
                    if(message.toString().equals("exit")){
                        System.out.println("Server: " + message);
                        break;
                    } else if(message.toString().equals("Enter username")){ 
                        TestConnection.login(bufferedReader, bufferedWriter);
                        System.out.println("Server: " + message);
                    }
                }
            }
            // Close the socket
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}