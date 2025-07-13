const {Socket} = require('net');
const client = new Socket();
const IP = '127.0.0.1';
const PORT = '8080';
let testindex = 0;
let userID = "";

client.connect(PORT, IP, async () =>{
  console.log("connected");
  try{
    console.log("Running User Functionality Test");
    let result = await user_functionality_test();
    console.log(result);
  }catch(err){
    console.error(err);
  }
});
function user_functionality_test(){
  return new Promise((resolve, reject) => {
    
    try{
      client.write("createuser|luis|testing@email.com|password");
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
            client.write("terminate");
            client.removeListener('data', onData);
            resolve("User Test" + ' \u2713');
          }else{
            client.write("terminate");
            client.removeListener('data', onData);
            reject("User Test" + ' \u2717' + "\n" + response);
          }
      }
      client.on('data', onData);
    }catch(err){
      console.log(err);
      reject("User Test" + ' \u2717' + "\n" + err.message);
    }
  });
}