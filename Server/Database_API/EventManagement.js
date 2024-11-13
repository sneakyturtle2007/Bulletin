
function CreateEvent(args, DB){
    return new Promise(async (resolve, reject) =>{
        userid = args[0];
        title = args[1];
        date = args[2];
        startTime = args[3];
        endTime = args[4];
        publicityType = args[5];
        invitees = args[6];
        details = args[7];
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
        DB.db.serialize(() => {
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
                        console.log("EventManagement.js: Deleting invitees");
                        for(let i = 0; i < invitees.length; i++){
                            DB.GetUserInfo(invitees[i], (err, user) => {
                                if(err){
                                    console.log("EventManagement.js: Error getting user info");
                                    console.log(err.message);
                                    resolve(err);
                                }
                                let invited = user[0].invited.toString().split(',');
                                invited.splice(1, invited.indexOf(event[0].eventid));
                                let result = "";
                                for(let i = 0; i < invited.length; i++){
                                    if(i == invited.length - 1){
                                        result += invited[i];
                                        break;
                                    }
                                    result += invited[i] + ",";
                                }
                                DB.UpdateTable("users", `invited="'${result}'"`, `username="${invitees[i]}"`);
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
        userid = args[0];
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
        eventid = args[0];
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
        eventid = args[0];
        invitee = args[1];

        DB.GetEventInfo(eventid, (err, event) => {
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
                    DB.UpdateTable('events', `invitees="${invitees}"`, `id=${event[0].id}`);
                    resolve("Invitee added");
                    return;
                }catch(err){
                    resolve('Unable to add invitee');
                    return;
                }
            });

        })
    });
}

module.exports = {CreateEvent, DeleteEvent, GetEvents, AddInvitee, GetEventInfo, WipeAllEvents};