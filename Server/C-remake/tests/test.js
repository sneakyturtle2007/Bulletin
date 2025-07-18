const {Socket} = require('net');
const client = new Socket();
const IP = '127.0.0.1';
const PORT = '8080';
let testindex = 0;


client.connect(PORT, IP, async () =>{
  console.log("connected");
  try{
    
    console.log("Running User Functionality Test");
    let result = await user_functionality_test();
    console.log(result);
    console.log("Running Event Functionality Test");
    let result1 = await event_functionality();
    console.log(result1);
  }catch(err){
    console.error(err);
  }
});
function user_functionality_test(){
  return new Promise((resolve, reject) => {
    let userID = "";
    try{
      
      let onData = async (data) => {
        let response = data.toString().trim();
        response = response.substring(0, response.length );
        console.log("Recieved: " + response + "\n");
        if(response == "Success" && testindex == 0){
          console.log("login|luis|password");
          client.write("login|luis|password");
          testindex++;
        }else if(response != "Invalid credentials" && response.split("|")[2] == "NONE"  && testindex == 1){
          userID = response.split("|")[0];
          console.log(`addfriend|${userID}|luis2`);
          client.write(`addfriend|${userID}|luis2`);
          testindex++;
        }else if(response == "Success" && testindex == 2){
          console.log(`removefriend|${userID}|luis2`);
          client.write(`removefriend|${userID}|luis2`);
          testindex++;
        }else if(response == "Success" && testindex == 3){
          console.log(`deleteuser|${userID}`);
          client.write(`deleteuser|${userID}`);
          testindex++;
        } else if(response == "Success" && testindex == 4){
          console.log(`deleteuser|${userID}`);
          client.write(`deleteuser|${userID}`);
          testindex++;
        }else if(response == "user_management.c/delete_user/ERROR: User does not exist." && testindex == 5){
          //client.write("terminate");
          client.removeListener('data', onData);
          resolve("User Test" + ' \u2713');
        }else{
          client.write("terminate");
          client.removeListener('data', onData);
          reject("User Test" + ' \u2717' + "\n" + response);
        }
      }
      client.on('data', onData);
      client.write("createuser|luis|testing@email.com|password");
    }catch(err){
      console.log(err);
      reject("User Test" + ' \u2717' + "\n" + err.message);
    }
  });
}

function event_functionality(){
  return new Promise( (resolve, reject) => {
    let event_id = "";
    try {
      
      let onData = async (data) => {
        let response = data.toString().trim();
        console.log(response + "\n");
        if(!isNaN(Number(response.substring(0, response.length - 1))) && testindex == 5){
          event_id = response.substring(0,response.length-1);
          console.log(`addinvitee|${event_id}|admin`);
          client.write(`addinvitee|${event_id}|admin`);
          testindex ++;
        }else if(response == "Success" && testindex == 6){
          console.log(`deleteevent|${event_id}`);
          client.write(`deleteevent|${event_id}`);
          testindex ++;
        }else if(response == "Success" && testindex == 7){
          console.log("terminate");
          client.write("terminate");
          client.removeListener('data', onData);
          resolve("Event Test" + ' \u2713');
        }else{
          client.write("terminate");
          client.removeListener('data', onData);
          reject("Event Test" + ' \u2717' + "\n" + response);
        }
      }
      client.on('data', onData);
      client.write("createevent|3|test|2021/2/24-2021/3/24|1500|1600|NONE|private|NONE|NONE");
    }catch(err){
      console.log(err);
      reject("Event Test" + ' \u2717' + "\n" + err.message);
    }
  });
}