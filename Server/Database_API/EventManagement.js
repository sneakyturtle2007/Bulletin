
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
            }else{
                resolve(result);
            }
        });
    });
}   

function DeleteEvent(eventTitle, db){
    return new Promise(async (resolve, reject) =>{
        eventTitle = args[0];

        db.DeleteEvent(eventTitle, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
            }else{
                resolve(result);
            }
        });
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