
function CreateEvent(args, DB){
    return new Promise(async (resolve, reject) =>{
        userid = args[0];
        title = args[1];
        date = args[2];
        startTime = args[3];
        endTime = args[4];
        location = args[5];
        publicityType = args[6];
        invitees = args[7];
        details = args[8];

        DB.CreateEvent(userid, title, date, startTime, endTime, location, publicityType, invitees, details, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
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
        userid = args[0];
        eventTitle = args[1];
        try{
            DB.DeleteFromTable('events', eventTitle + "AND userid=" + userid);
        }catch(err){
            reject(err);
            return;
        }  
        resolve("Event deleted");
    });
}

function GetEvents(args, DB){
    return new Promise(async (resolve, reject) =>{
        userid = args[0];
        DB.GetEvents(userid, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
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
                console.log(err);
                reject(err);
            }else{
                resolve(result[0]);
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

module.exports = {CreateEvent, DeleteEvent, GetEvents, AddInvitee, GetEventInfo};