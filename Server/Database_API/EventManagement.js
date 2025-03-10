function CreateEvent(args, DB){
    return new Promise(async (resolve, reject) =>{
        let userid = args[0];
        let title = args[1];
        let date = args[2];
        let startTime = args[3];
        let endTime = args[4];
        let publicityType = args[5];
        let invitees = args[6];
        let details = args[7];
        console.log(userid + title + date + startTime + endTime + publicityType + invitees + details);
        DB.CreateEvent(userid, title, date, startTime, endTime, publicityType, invitees, details, (err, result) => {
            if(err){
                console.log(err);
                resolve(err);
                return;
            }else{
                resolve(result);
                return;
            }
        });
    });
}   

function DeleteEvent(args, DB){
    return new Promise( (resolve, reject) =>{
        let eventID = args[0];
        console.log("EventManagement.js/DeleteEvent: Deleting event");
        
        let invitees;
        DB.db.serialize(() => {

            DB.GetEventInfo(eventID, async (err, event) => {

                if(err){
                    console.log("EventManagement.js/DeleteEvent: Error getting event info");
                    console.log(err.message);
                    resolve(err);
                    return;
                }
                
                invitees = event[0].invitees.toString().split(',');
                console.log(`EventManagement.js/DeleteEvent: Invitees (eventID ${eventID}): ` + invitees);
            
                try{
                    if(invitees.length > 0){
                        for(let i = 0; i < invitees.length; i++){
                            console.log("EventManagement.js/DeleteEvent: Uninviting invitees");
                            if(i == invitees.length - 1){
                                DB.db.serialize(() => {
                                    Uninvite([invitees[i], eventID], DB);

                                    DB.DeleteEvent(eventID);
                                });
                            }else{
                                DB.db.serialize(() => {
                                    Uninvite([invitees[i], eventID], DB);
                                });
                            }
                            
                        }   
                    }
                }catch(err){
                    console.log(err.message);
                    resolve("Error deleting event");
                    return;
                }  
                resolve("Event deleted");
            
            });
        });
    });
}
function WipeAllEvents(DB){
    return new Promise( (resolve, reject) =>{
        try{
            DB.WipeAllEvents();
            resolve("Events wiped");
        }catch{
            resolve("Error wiping events");
            return;
        }
    });
}
function GetEvents(args, DB){
    return new Promise( (resolve, reject) =>{
        let userid = args[0];
        DB.GetAllEvents(userid, (err, result) => {
            if(err){
                console.log("Error getting events");
                console.log(err.message);
                resolve("Error getting events");
            }else{
                if(result.length > 0){
                    resolve(JSON.stringify(result));
                    return;
                }
                resolve("No events found");
            }
        });
    });
}
function GetEventInfo(args, DB){
    return new Promise(async (resolve, reject) =>{
        let eventid = args[0];
        DB.GetEventInfo(eventid, (err, result) => {
            if(err){
                console.log("EventManagement.js/GetEventInfo: Error getting event info");
                console.log(err.message);
                resolve("Error getting event info");
                return;
            }else{
                resolve(result[0].title + "," + result[0].details + ",(" + result[0].date + ")," + result[0].startTime + "," + result[0].endTime + "," + result[0].publicityType + ",(" + result[0].invitees + ")," + result[0].userid);
            }
        });

    });
}
function AddInvitee(args, DB){
    return new Promise(async (resolve, reject) => {
        let eventID = args[0];
        let invitee = args[1];

        DB.GetEventInfo(eventID, (err, event) => {
            if(err){
                console.log(err.message);
                resolve('Event not found');
                return;
            }
            invitees = event[0].invitees.toString();
            if(invitees == "NONE"){
                invitees = `${invitee}`;
            }else if(!invitees.includes(invitee)){
                invitees = `${invitees}`+ `,${invitee}`;
            }else{
                resolve("Invitee already added");
                return;
            }  
            console.log(invitees);
            DB.db.serialize(()=>{
                try{
                    DB.UpdateTable('events', `invitees="${invitees}"`, `eventid=${event[0].eventid}`);
                    
                    resolve("Invitee added");
                    return;
                }catch(err){
                    resolve('Unable to add invitee');
                    return;
                }
            });
        });
    });
}

function RemoveInvitee(args, DB){
    return new Promise(async (resolve, reject) => {
        let eventID = args[0];
        let invitee = args[1];
        DB.GetEventInfo(eventID, async (err, event) => {
            if(err){
                console.log(err.message);
                resolve('Event not found');
                return;
            }
            let invitees = event[0].invitees.toString();
            let result = "";
            if(invitees == "NONE"){
                resolve("No invitees to remove");
                return;
            }else if(!invitees.includes(invitee)){
                resolve("Invitee not found");
                return;
            }else{
                invitees = invitees.split(",");
                invitees.splice(invitees.indexOf(invitee),1);
                if(invitees.length == 0){
                    invitees = "NONE";
                }
                await Uninvite([invitee, eventID], DB);
                console.log(invitees);
            }  
            DB.db.serialize(()=>{
                try{
                    invitees = invitees.join(",");
                    DB.UpdateTable('events', `invitees="${invitees}"`, `eventid=${event[0].eventid}`);
                    resolve("Invitee removed");
                    return;
                }catch(err){
                    resolve('Error removing invitee');
                    return;
                }
            });
        });
    });
}

function Uninvite(args, DB){
    return new Promise(async (resolve, reject) =>{
        let username = args[0];
        let eventID = args[1];
        DB.GetUserInfo(username, (err, user) => {
            if(err){
                console.log('\n' + 'Error in Uninvite: Error getting user info');
                console.log(err.message);
                resolve('Error getting user info');
            }
            let invites;

            if(user[0].invited == "NONE"){
                resolve("User not invited");
                return;
            }else if(!user[0].invited.includes(eventID)){
                resolve("User not invited");
                return;
            }else{
                invites = user[0].invited.split(',');
                invites.splice(invites.indexOf(eventID), 1);
                invites = invites.join(',');
                if(invites.length == 0){
                    invites = "NONE";
                }
            }
            
            DB.db.serialize(() => {
                try{
                    DB.UpdateTable('users', `invited="${invites}"`, `username="${username}"`);
                    resolve("User uninvited");
                }catch(err){
                    console.log("Error uninviting user: " + err.message);
                    resolve("Error uninviting user");
                }
            });
        });
    });
}

module.exports = {CreateEvent, DeleteEvent, GetEvents, AddInvitee,RemoveInvitee, GetEventInfo, WipeAllEvents};