
function CreateEvent(args, db){
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

        db.CreateEvent(userid, title, date, startTime, endTime, location, publicityType, invitees, details, (err, result) => {
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

function DeleteEvent(eventTitle, db){
    return new Promise(async (resolve, reject) =>{
        userid = args[0];
        eventTitle = args[1];
        try{
            db.DeleteFromTable('events', eventTitle + "AND userid=" + userid);
        }catch(err){
            reject(err);
            return;
        }  
        resolve("Event deleted");
    });
}

function GetEvents(userid, db){
    return new Promise(async (resolve, reject) =>{
        userid = args[0];
        db.GetEvents(userid, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
            }else{
                resolve(result);
            }
        });
    });
}

module.exports = {CreateEvent, DeleteEvent, GetEvents};