let eventID;
let userid;
async function UserTest(username, email, password, client){
    return new Promise(async (resolve, reject) => {
        try{
            client.write(`createuser ${username} ${email} ${password}`);
            client.on('data', async (data) => {
                let value = -1;
                
                let response = data.toString().trim();

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
                    reject("User Test" + ' \u2717' + "\n" + data.toString().trim());
                    
                }
            });
        }catch(err){
            err = "User Test" + ' \u2717' + "\n" + err.toString().trim();
            reject(err);
        }
        
    });
   
}

async function EventTest(userID, title, date, startTime, endTime, publicityType, invitees, details, client){
    return new Promise( (resolve, reject) => {
        try{
            userid = userID;
            client.write(`createevent ${userID} ${title} ${date} ${startTime} ${endTime} ${publicityType} ${invitees} ${details}`);
            
            client.on('data', async (data) => {

                let response = data.toString().trim();
        
                //console.log(`Response: ${response}\n`);
                if(response.includes('Event created')){
                    eventID = response.split(" ")[2];
                    console.log(`Sent: geteventinfo ${eventID}`);
                    client.write(`geteventinfo ${eventID}`);
                     
                }else{
                    try{
                        response = DealingWithParenthesis(response);
                        response = response.split(",");
                    }catch(err){
                        //PLACEHOLDER
                    }
                    try{
                        response = JSON.parse(data);
                    }catch(err){
                        //PLACEHOLDER
                    }
                    if(response[7] == userID){
                        console.log(`Sent: addinvitee ${eventID} test`);
                        client.write(`addinvitee ${eventID} test`);

                    }else if(response == "Invitee added"){
                        console.log(`Sent: addinvitee ${eventID} test`);
                        client.write(`addinvitee ${eventID} test`);

                    }else if(response == "Invitee already added"){
                        console.log(`Sent: removeinvitee ${eventID} test`);
                        client.write(`removeinvitee ${eventID} test`);

                    }else if(response == "Invitee removed"){
                        console.log(`Sent: removeinvitee ${eventID} test`);
                        client.write(`removeinvitee ${eventID} test`);

                    }else if(response == "Invitee not found"){
                        console.log(`Sent: deleteevent ${eventID}`);
                        client.write(`deleteevent ${eventID}`);
                        
                    }else if(response == 'Event deleted'){
                        console.log(`Sent: deleteevent ${eventID}`);
                        client.write(`deleteevent ${eventID}`);

                    }else if(response == 'Event not found'){  
                        console.log(`Sent: getevents ${userid}`);
                        client.write(`getevents ${userid}`);
        
                    }else if(response.length > 0){
                        //console.log("testing");
                        resolve("Event Test" + ' \u2713');
                    }else{
                        
                        err = "Event Test" + ' \u2717' + "\n" + data.toString().trim();
                        reject(err);
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

async function CalendarTest(userID,year, month, client){
    return new Promise((resolve, reject) => {
        try{
            client.write('getmonthevents '+ userID + ' ' + year + ' ' + month);
            client.on('data', async (data) => {
                let response = data.toString().trim();
                try{
                    response = JSON.parse(data);
                }catch(e){
                    // PLACEHOLDER
                }
                console.log(response.length);
                if(response.length > 0 && response != "No events found"){
                    console.log(`Sent: getmonthevents ${userID} ${year} 3`);
                    client.write('getmonthevents '+ userID + ' ' + year + ' ' + 3);
                }else if(response == "No events found"){        
                    
                    resolve("Calendar Test" + ' \u2713');
                    
                }else{
                    reject("Calendar Test" + ' \u2717' + "\n" + response);
                }
            });
        }catch(e){
            err = "Calendar Test" + ' \u2717' + "\n" + e.toString().trim();
            reject(err);
        }
    })
}
const {Socket} = require('net');
const client = new Socket();
const IP = '100.103.6.83';

client.connect(8000,IP,async () => {
    
    console.log('Connected');
    
    try{
        let usertest = await UserTest('testing', 'example@gmail.com', 'testing', client);
        console.log(usertest);

        let eventtest = await EventTest('2', 'test', '2021/2/24,2023/5/3', '1500', '1600', 'private', 'admin,friend', 'NONE', client);
        console.log(eventtest);

        let calendartest = await CalendarTest('2', '2021', '2', client);
        console.log(calendartest);

        client.end();
    }catch(err){
        console.log(err);
    }
    
    
});

