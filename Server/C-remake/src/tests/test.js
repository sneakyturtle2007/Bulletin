const {Socket} = require('net');
const client = new Socket();
const IP = '127.0.0.1';
const PORT = '8080';

client.connect(PORT, IP, async () =>{
  console.log("connected");

  client.on('data', (data) =>{
    let info = data.toString().trim();
    
    console.log("Recieved: ", info);

    if(info === "terminate\0"){
      client.write("terminate");
      console.log("Terminating...");
      client.end();
    }else{
      console.log("incorrect response");
    }

  });

  client.write('test');
});
