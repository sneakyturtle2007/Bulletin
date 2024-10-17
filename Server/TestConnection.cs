
using System;

using System.Net;

using System.Net.Sockets;

using System.Text;



class TcpClientExample

{

    static void Main(string[] args)

    {

        string serverAddress = "127.0.0.1"; // Localhost

        int serverPort = 22;



        try{
            using (TcpClient client = new TcpClient(serverAddress, serverPort)){
                Console.WriteLine("Connected to server");
                // Send data to server
                NetworkStream stream = client.GetStream();
                byte[] data = Encoding.ASCII.GetBytes("Hello from client");
                stream.Write(data, 0, data.Length);
                // Receive data from server
                byte[] buffer = new byte[1024];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string response = Encoding.ASCII.GetString(buffer, 0, bytesRead);
                Console.WriteLine("Server response: " + response);
            }
        }catch(Exception ex){
            Console.WriteLine("Error: " + ex.Message);
        }
    }
}