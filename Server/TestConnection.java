import java.io.*;
import java.net.*;
import java.util.*;

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

      bufferedWriter.write("getuserinfo test");
      bufferedWriter.newLine();
      bufferedWriter.flush();
      System.out.println("sent message to server");
      String message = "";
      String userinfo = "";
      
      while (true) {
        if (bufferedReader.ready()) {
          message = bufferedReader.readLine();
          System.out.println("Server: " + message);
          if (message != null) {
            bufferedWriter.write("addfriend test friend");
            bufferedWriter.newLine();
            bufferedWriter.flush();
            break;
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
