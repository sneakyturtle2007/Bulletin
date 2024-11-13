
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
        
        DB.db.serialize(() => {
            let invitees;
            DB.db.GetEventInfo(eventID, (err, event) => {
                if(err){
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
                                    console.log(err.message);
                                    resolve(err);
                                }
                                DB.UpdateTable("users", `invited=${user[0].invited.splice(1, user[0].invited.indexOf(event[0].eventid))}`, `username="${invitees[i]}"`);
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