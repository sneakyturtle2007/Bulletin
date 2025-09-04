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
    let result2 = await calendar_functionality();
    console.log(result2);
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
        if(response.status == "Success" && testindex == 5){
          event_id = response.event_id;
          console.log(`addinvitees|${event_id}|admin`);
          client.write(`addinvitees|${event_id}|admin`);
          
          testindex ++;
        }else if(response.status == "Success" && testindex == 6){
          console.log(`removeinvitees|${event_id}|admin`);
          client.write(`removeinvitees|${event_id}|admin`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 7){
          console.log(`addinvitees|${event_id}|test,friend,Backend`);
          client.write(`addinvitees|${event_id}|test,friend,Backend`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 8){
          console.log(`removeinvitees|${event_id}|test,friend`);
          client.write(`removeinvitees|${event_id}|test,friend`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 9){
          console.log(`deleteevent|${event_id}`);
          client.write(`deleteevent|${event_id}`);
          testindex ++;
        }else if(response.status == "Success" && testindex == 10){
          client.removeListener('data', onData);
          resolve("Event Test" + ' \u2713');
        }else{
          client.write("terminate");
          client.removeListener('data', onData);
          reject("Event Test" + ' \u2717' + "\n" + response.status);
        }
      }
      client.on('data', onData);
      client.write("createevent|3|test|2021-2-24|2021-3-24|1500|1600|NONE|private|NONE|NONE|NONE");
    }catch(err){
      console.log(err);
      reject("Event Test" + ' \u2717' + "\n" + err.message);
    }
  });
}
function calendar_functionality(){
  return new Promise( (resolve, reject) =>{
    try{
      let onData = async (data) => {
        let response = data.toString().trim();
        response = response.substring(0, response.length -1);
        console.log(response + "\n");
        response = JSON.parse(response);
        if(response[0].status == "Success" && testindex == 10){
          console.log("terminate");
          client.write("terminate");
          resolve("Calendar Test" + ' \u2713');
        }else{
          client.write("terminate");
          client.removeListener('data', onData);
          reject("Calendar Test" + ' \u2717' + "\n" + response.status);
        }
      }
      client.on('data', onData);
      console.log("getmonthevents|3|2021|1");
      client.write("getmonthevents|3|2021|1");
    }catch(err){
      console.log(err);
      reject("Calendar Test" + ' \u2717' + "\n" + err.message);
    }
  });
}