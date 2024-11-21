
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
    return new Promise(async (resolve, reject) =>{
        let eventID = args[0];
        console.log("EventManagement.js: Deleting event");
        
        let invitees;
        DB.GetEventInfo(eventID, (err, event) => {
            if(err){
                console.log("EventManagement.js: Error getting event info");
                console.log(err.message);
                resolve(err);
                return;
            }
            DB.db.serialize(() => {
                invitees = event[0].invitees.toString().split(',');
                if(invitees.length > 0){
                    for(let i = 0; i < invitees.length; i++){
                        DB.GetUserInfo(invitees[i], (err, user) => {
                            if(err){
                                console.log("EventManagement.js: Error getting user info");
                                console.log(err.message);
                                resolve(err);
                            }
                            let invited = user[0].invited.toString().split(',');

                            //console.log("\nInvited List: " + invited); DEBUG STATEMENT

                            invited.splice(invited.indexOf(event[0].eventid.toString()), 1);

                            //console.log("\nInvited List After: " + invited); DEBUG STATEMENT

                            let invitedEdited= "";
                            if(invited.length > 0){
                                for(let i = 0; i < invited.length; i++){
                                    if(i == invited.length - 1){
                                        invitedEdited += invited[i];
                                        break;
                                    }
                                    invitedEdited += invited[i] + ",";
                                }
                            }else{
                                invitedEdited = "NONE"
                            }
                            
                            //console.log("Trying to delete event from invited list: " + invitedEdited); DEBUG STATEMENT

                            DB.UpdateTable("users", `invited="${invitedEdited}"`, `username="${invitees[i]}"`);
                        });
                        
                    }
                }
                
                try{
                    DB.DeleteEvent(eventID);
                }catch(err){
                    console.log(err.message);
                    resolve("Error deleting event");
                    return;
                }  
            });
            
        });
    
        resolve("Event deleted");
    });
}
function WipeAllEvents(DB){
    return new Promise(async (resolve, reject) =>{
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
    return new Promise(async (resolve, reject) =>{
        let userid = args[0];
        DB.GetAllEvents(userid, (err, result) => {
            if(err){
                console.log("Error getting events");
                console.log(err.message);
                resolve("Error getting events");
                return;
            }else{
                resolve(result);
            }
        });
    });
}
function GetEventInfo(args, DB){
    return new Promise(async (resolve, reject) =>{
        let eventid = args[0];
        DB.GetEventInfo(eventid, (err, result) => {
            if(err){
                console.log("Error getting event info");
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
        DB.GetEventInfo(eventID, (err, event) => {
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
                resolve("Invitees not found");
                return;
            }else{
                invitees = invitees.split(",");
                //invitees = invitees.slice(0,invitees.indexOf(invitee)-1) + invitees.slice(invitees.indexOf(invitee)+1, invitees.Count);
                invitees = invitees.splice(invitees.indexOf(invitee),1);
                for(let i = 0; i < invitees.length; i++){
                    if(i == invitees.length -1){
                        result += invitees[i];
                        break;
                    }
                    result += invitees[i] + ",";
                }
            }  
            DB.db.serialize(()=>{
                try{
                    DB.UpdateTable('events', `invitees="${result}"`, `eventid=${event[0].eventid}`);
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


module.exports = {CreateEvent, DeleteEvent, GetEvents, AddInvitee,RemoveInvitee, GetEventInfo, WipeAllEvents};