var currentTest = 0;
var eventID;
async function UserTest(username, email, password, client){
    return new Promise(async (resolve, reject) => {
        try{
            client.write(`createuser ${username} ${email} ${password}`);
            client.on('data', async (data) => {
                value = -1;
                
                response = data.toString().trim();

                try{
                    value = Number(response.split(',')[0])
                }catch(err){
                    console.log(err);
                }
                
                if(response == 'User created'){
                    client.write(`createuser example ${email} ${password}`);

                }else if(response == 'Email taken'){
                    client.write(`createuser ${username} example@gmail.com ${password}`);

                }else if(response == 'Username taken'){
                    client.write(`login ${email} ${password}`);
                
                }else if(response == 'User logged in'){
                    client.write(`getuserinfo ${username}`);
                
                }else if(value >= 0){
                    client.write(`getuserinfo a`);
                    
                }else if(response == 'User not found'){
                    client.write(`addfriend ${username} example`);
                
                }else if(response == 'Friend added'){
                    client.write(`addfriend ${username} example`);
                
                }else if(response == 'Friend already added'){
                    client.write(`deleteuser ${username}`);
                
                }else if(response == 'User deleted'){
                    resolve("User Test" + ' \u2713')
                }else{
                    err = "User Test" + ' \u2717' + "\n" + data.toString().trim();
                    reject(err);
                    
                }
            });
        }catch(err){
            err = "User Test" + ' \u2717' + "\n" + err.toString().trim();
            reject(err);
        }
        
    });
   
}

async function EventTest(userid, title, date, startTime, endTime, publicityType, invitees, details, client){
    return new Promise(async (resolve, reject) => {
        try{

            client.write(`createevent ${userid} ${title} ${date} ${startTime} ${endTime} ${publicityType} ${invitees} ${details}`);
            
            client.on('data', async (data) => {
                value = -1;

                let response = data.toString().trim();

                console.log(`Response: ${response}\n`);
                if(response.includes('Event created')){
                    eventID = response.split(" ")[2];
                    console.log(`Sent: geteventinfo ${eventID}`);
                    client.write(`geteventinfo ${eventID}`); 
                }else{
                    try{
                        response = DealingWithParenthesis(response);
                        response = response.split(",");
                        if(response[7] == userid){
                            console.log(`Sent: deleteevent ${eventID}`);
                            client.write(`deleteevent ${eventID}`);
                            
                        }else if(response == 'Event deleted'){
                            resolve("Event Test" + ' \u2713')
                        }else{
                            err = "Event Test" + ' \u2717' + "\n" + data.toString().trim();
                            reject(err);
                        }
                    }catch(err){
                        console.log(err);
                    }
                }
                
            });
        }catch(err){
            err = "Event Test" + ' \u2717' + "\n" + err.toString().trim();
            reject(err);
        }
            
    });
        
}
function DealingWithParenthesis(source){
    let result;
    try{
        let date = source.substring(source.indexOf("("),source.indexOf(")") + 1);
        result = source.replace(date, "");
        let invitees = result.substring(result.indexOf("("),(result.indexOf(")") + 1));
        result = result.replace(invitees, "");
    }catch(err){
        throw err;
    }
    return result;
}

const {Socket} = require('net');
const client = new Socket();

client.connect(22,'127.0.0.1' ,async () => {
    
    console.log('Connected');
    
    try{
        usertest = await UserTest('testing', 'example@gmail.com', 'testing', client);
        console.log(usertest);
        //client.write('wipeallevents');
        eventtest = await EventTest('1', 'test', '2021/2/24,2023/5/3', '1500', '1600', 'private', 'admin,friend', 'NONE', client);
        console.log(eventtest);
    }catch(err){
        console.log(err);
    }
    
    
});
/*
client.write(`createevent 1 test 2021/2/24,2023/5/3 1500 1600 private john,aba NONE`);
    client.on('data', ( data) =>{

        console.log(data.toString().trim());
    });

*/

/*

*/
