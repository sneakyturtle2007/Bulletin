var currentTest = 0;
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

async function EventTest(title, details, date, startTime, endTime, publicityType, invitees, userid, client){
    return new Promise(async (resolve, reject) => {
        try{
            client.write(`createevent ${title} ${details} ${date} ${startTime} ${endTime} ${publicityType} ${invitees} ${userid}`);
            client.on('data', async (data) => {

                value = -1;

                response = data.toString().trim();
                
                try{
                    value = Number(response.split(',')[0])
                }catch(err){
                    console.log(err);
                }
                

                if(response == 'Event created'){
                    client.write(`createevent ${title} ${details} ${date} ${startTime} ${endTime} ${publicityType} ${invitees} ${userid}`);
                
                }else if(response == 'User already invited'){
                    client.write(`geteventinfo 1`);

                }else if(value >= 0){
                    resolve("Event Test" + ' \u2713')
                }else{
                    err = "Event Test" + ' \u2717' + "\n" + data.toString().trim();
                    reject(err);

                }
            });
        }catch(err){
            err = "Event Test" + ' \u2717' + "\n" + err.toString().trim();
            reject(err);
        }
            
    });
        
}
const {Socket} = require('net');
const client = new Socket();

client.connect(22,'127.0.0.1' ,async () => {
    
    console.log('Connected');
    
    
    client.write(`createevent 1 test 2021/2/24-2023/5/3 1500 1600 private john,aba NONE`);
    client.on('data', ( data) =>{

        console.log(data.toString().trim());
    });
    
});
/*
try{
        usertest = await UserTest('testing', 'example@gmail.com', 'testing', client);
        console.log(usertest);
    }catch(err){
        console.log(err);
    }
*/

/*
eventtest = await EventTest('title', 'details', '2021/12/01-2022/12/23', '12:00', '13:00', 'public', 'example', '1', client);
    console.log(eventtest);
*/
