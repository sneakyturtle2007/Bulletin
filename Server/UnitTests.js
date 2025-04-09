let eventID;
let userid;
async function UserTest(username, email, password, client){
    return new Promise(async (resolve, reject) => {
        try{
            client.write(`createuser|${username}|${email}|${password}`);
            let onData = async (data) => {
                let value = -1;
                
                let response = data.toString().trim();

                try{
                    value = Number(response.split(',')[0])
                }catch(err){
                    console.log(err);
                }
                
                if(response == 'User created'){
                    console.log(`Sent: createuser|example|${email}|${password}`);
                    client.write(`createuser|example|${email}|${password}`);

                }else if(response == 'Email taken'){
                    console.log(`Sent: createuser|${username}|example@gmail.com|${password}`);
                    client.write(`createuser|${username}|example@gmail.com|${password}`);

                }else if(response == 'Username taken'){
                    console.log(`Sent: login|${email}|${password}`);
                    client.write(`login|${email}|${password}`);
                
                }else if(response == 'User logged in'){
                    console.log(`Sent: getuserinfo|${username}`);
                    client.write(`getuserinfo|${username}`);
                
                }else if(value >= 0){
                    console.log(`Sent: getuserinfo|a`);
                    client.write(`getuserinfo|a`);
                    
                }else if(response == 'User not found'){
                    console.log(`Sent: addfriend|${username}|example`);
                    client.write(`addfriend|${username}|example`);
                
                }else if(response == 'Friend added'){
                    console.log(`Sent: addfriend|${username}|example`);
                    client.write(`addfriend|${username}|example`);
                
                }else if(response == 'Friend already added'){
                    console.log(`Sent: deleteuser|${username}`);
                    client.write(`deleteuser|${username}`);
                
                }else if(response == 'User deleted'){
                    client.removeListener('data', onData);
                    resolve("User Test" + ' \u2713')
                }else{
                    client.removeListener('data', onData);
                    reject("User Test" + ' \u2717' + "\n" + data.toString().trim());
                    
                }
            };
            client.on('data', onData);
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
            client.write(`createevent|${userID}|${title}|${date}|${startTime}|${endTime}|${publicityType}|${invitees}|${details}`);
            let onData = async (data) => {
                let response = data.toString().trim();

                if(response.includes('Event created')){
                    eventID = response.split(" ")[2];
                    console.log(`Sent: geteventinfo|${eventID}`);
                    client.write(`geteventinfo|${eventID}`);

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
                        console.log(`Sent: addinvitee|${eventID}|test`);
                        client.write(`addinvitee|${eventID}|test`);

                    }else if(response == "Invitee added"){
                        console.log(`Sent: addinvitee|${eventID}|test`);
                        client.write(`addinvitee|${eventID}|test`);

                    }else if(response == "Invitee already added"){
                        console.log(`Sent: removeinvitee|${eventID}|test`);
                        client.write(`removeinvitee|${eventID}|test`);

                    }else if(response == "Invitee removed"){
                        console.log(`Sent: removeinvitee|${eventID}|test`);
                        client.write(`removeinvitee|${eventID}|test`);

                    }else if(response == "Invitee not found"){
                        console.log(`Sent: deleteevent|${eventID}`);
                        client.write(`deleteevent|${eventID}`);
                        
                    }else if(response == 'Event deleted'){
                        console.log(`Sent: deleteevent|${eventID}`);
                        client.write(`deleteevent|${eventID}`);

                    }else if(response == 'Event not found'){  
                        console.log(`Sent: getevents|${userid}`);
                        client.write(`getevents|${userid}`);
        
                    }else if(response.length > 0){
                        client.removeListener('data', onData);
                        resolve("Event Test" + ' \u2713');
                    }else{
                        
                        err = "Event Test" + ' \u2717' + "\n" + data.toString().trim();
                        client.removeListener('data', onData);
                        reject(err);
                    }
                }
            };
            client.on('data', onData);
        }catch(err){
            err = "Event Test" + ' \u2717' + "\n" + err.toString().trim();
            reject(err);
        }
            
    });
        
}

async function CalendarTest(userID, year, month, client){
    return new Promise((resolve, reject) => {
        try{
            console.log(`Sent: getmonthevents ${userID} ${year} ${month}`);
            client.write(`getmonthevents|${userID}|${year}|${month}`);

            let onData = async (data) => {
                let response = data.toString().trim();
                console.log("Before parsing: " + response);
                try{
                    response = JSON.parse(data);
                }catch(e){
                    // PLACEHOLDER
                }
                console.log(response);
                
                if(response[0].title == "Birthday"){
                    console.log(`Sent: getmonthevents|${userID}|${year}|5`);
                    client.write(`getmonthevents|${userID}|${year}|5`);

                }else if(response == "No events found"){      
                    console.log(`Sent: getbusytimeinmonth|${userID}|test,Backend|2025|3`);  
                    client.write(`getbusytimeinmonth|${userID}|test,Backend|2025|3`);

                }else if(response[24].length > 0){
                    client.removeListener('data', onData);
                    resolve("Calendar Test" + ' \u2713');

                }else{
                    client.removeListener('data', onData);
                    reject("Calendar Test" + ' \u2717' + "\n" + response);

                }
            };

            client.on('data', onData);
        }catch(e){
            err = "Calendar Test" + ' \u2717' + "\n" + e.message.trim();
            reject(e.message.toString().trim());
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
const IP = '0.0.0.0';

client.connect(8000,IP,async () => {
    
    console.log('Connected');
    
    try{
        let usertest = await UserTest('testing', 'testing@gmail.com', 'testing', client);
        console.log(usertest);

        let eventtest = await EventTest('4', 'test', '2021/2/24,2023/5/3', '1500', '1600', 'private', 'admin,friend', 'NONE', client);
        console.log(eventtest);

        let calendartest = await CalendarTest('3', '2021', '2', client);
        console.log(calendartest);
        
    }catch(err){
        console.log(err);
    }finally{
        console.log('Closing connection');
        client.end();
    }
});

