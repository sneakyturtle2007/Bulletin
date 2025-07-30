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
        response = response.substring(0, response.length -1);
        console.log("Recieved: " + response + "\n");
        response = JSON.parse(response);
        if(response.status == "Success" && testindex == 0){
          console.log("login|luis|password");
          client.write("login|luis|password");
          testindex++;
        }else if(response.status != "Invalid credentials" && testindex == 1){
          userID = response.user_id;
          console.log(`addfriend|${userID}|luis2`);
          client.write(`addfriend|${userID}|luis2`);
          testindex++;
        }else if(response.status == "Success" && testindex == 2){
          console.log(`removefriend|${userID}|luis2`);
          client.write(`removefriend|${userID}|luis2`);
          testindex++;
        }else if(response.status == "Success" && testindex == 3){
          console.log(`deleteuser|${userID}`);
          client.write(`deleteuser|${userID}`);
          testindex++;
        } else if(response.status ==  "Success" && testindex == 4){
          console.log(`deleteuser|${userID}`);
          client.write(`deleteuser|${userID}`);
          testindex++;
        }else if(response.status ==  "user_management.c/delete_user/ERROR: User does not exist.\n" && testindex == 5){
          //client.write("terminate");
          client.removeListener('data', onData);
          resolve("User Test" + ' \u2713');
        }else{
          client.write("terminate");
          client.removeListener('data', onData);
          reject("User Test" + ' \u2717' + "\n" + response.status);
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
    let event_id;
    try {
      
      let onData = async (data) => {
        let response = data.toString().trim();
        response = response.substring(0, response.length -1);
        console.log(response + "\n");
        response = JSON.parse(response);
        if(response.event_id != 0 && testindex == 5){
          event_id = response.event_id;
          console.log(`addinvitee|${event_id}|admin`);
          client.write(`addinvitee|${event_id}|admin`);
          
          testindex ++;
        
        }else if(response.status == "Success" && testindex == 6){
          console.log(`removeinvitee|${event_id}|admin`);
          client.write(`removeinvitee|${event_id}|admin`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 7){
          console.log(`addinvitee|${event_id}|test`);
          client.write(`addinvitee|${event_id}|test`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 8){
          console.log(`addinvitee|${event_id}|friend`);
          client.write(`addinvitee|${event_id}|friend`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 9){
          console.log(`removemultipleinvitees|${event_id}|test,friend`);
          client.write(`removemultipleinvitees|${event_id}|test,friend`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 10){
          console.log(`deleteevent|${event_id}`);
          client.write(`deleteevent|${event_id}`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 11){
          console.log("terminate");
          client.write("terminate");
          client.removeListener('data', onData);
          resolve("Event Test" + ' \u2713');
        }else{
          client.write("terminate");
          client.removeListener('data', onData);
          reject("Event Test" + ' \u2717' + "\n" + response.status);
        }
      }
      client.on('data', onData);
      client.write("createevent|3|test|2021/2/24|2021/3/24|1500|1600|NONE|private|NONE|NONE");
    }catch(err){
      console.log(err);
      reject("Event Test" + ' \u2717' + "\n" + err.message);
    }
  });
}