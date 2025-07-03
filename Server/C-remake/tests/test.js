const {Socket} = require('net');
const client = new Socket();
const IP = '127.0.0.1';
const PORT = '8080';

client.connect(PORT, IP, async () =>{
  console.log("connected");
  client.on('data', (data) =>{
    let info = data.toString().trim();
    info = info.substring(0, info.length -1);
    console.log("Recieved: " + info + "\n");

    if(info != "Invalid Command\0"){
      client.write("password");
      console.log("Terminating...\n");
      client.end();
    }else{
      console.log("incorrect response");
    }

  });
  client.write("test");
});
