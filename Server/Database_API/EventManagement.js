/* EventManagement.js contains functions that are used to manage events. The functions include CreateEvent, DeleteEvent, GetEvents, GetEventInfo, AddInvitee, RemoveInvitee, and WipeAllEvents.
    CreateEvent creates an event in the database. It first gets the arguments passed by the client, then creates the event in the database.
    If there was an error creating the event, it returns the error message. If the event was successfully created, it returns the result message.

    @param args - The arguments userid, title, date, startTime, endTime, publicityType, invitees, and details passed by the client
    @param DB - The database object
    @returns result - If the event was successfully created
    @returns err.message - If there was an error creating the event
*/
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
                console.log(err.message);
                reject(err.message);
            }else{
                resolve(result);
            }
        });
    });
}   
/* DeleteEvent deletes the event from the database. It first gets the event's info, then uninvites all the invitees from the event.
    If there are no invitees, it deletes the event from the database. If there was an error getting the event info, it returns the error message.
    If there was an error uninviting the invitees, it returns the error message. If there was an error deleting the event, it returns the error message.

    @param args - The arguments eventID passed by the client
    @param DB - The database object
    @returns "Event deleted" - If the event was successfully deleted
    @returns err.message - If there was an error getting the event info, uninviting the invitees, or deleting the event
*/
function DeleteEvent(args, DB){
    return new Promise( (resolve, reject) =>{
        let eventID = args[0];
        
        let invitees;
        DB.db.serialize(() => {

            DB.GetEventInfo(eventID, async (err, event) => {

                if(err){
                    console.log("EventManagement.js/DeleteEvent: Error getting event info");
                    console.log(err.message);
                    reject(err.message);
                    return;
                }
                
                invitees = event[0].invitees.toString().split(',');
            
                try{
                    if(invitees.length > 0){
                        for(let i = 0; i < invitees.length; i++){
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
                    reject(err.message);
                    return;
                }  
                resolve("Event deleted");
            
            });
        });
    });
}
/* WipeAllEvents wipes all the events from the database. It first gets all the events from the database, then deletes all the events from the database.
    If there was an error getting the events, it returns the error message. If there was an error deleting the events, it returns the error message.

    @param DB - The database object
    @returns "Events wiped" - If the events were successfully wiped
    @returns err.message - If there was an error getting the events or deleting the events
*/
function WipeAllEvents(DB){
    return new Promise( (resolve, reject) =>{
        try{
            DB.WipeAllEvents();
            resolve("Events wiped");
        }catch(err){
            reject(err.message);
        }
    });
}
/* GetEvents gets all the events for the user from the database. It first gets all the events for the user from the database, then returns the events as a string.
    If there was an error getting the events, it returns the error message.    

    @param args - The arguments userid passed by the client
    @param DB - The database object
    @returns JSON.stringify(result) - The events for the user
    @returns "No events found" - If there are no events for the user
    @returns err.message - If there was an error getting the events
*/
function GetEvents(args, DB){
    return new Promise( (resolve, reject) =>{
        let userid = args[0];
        DB.GetAllEvents(userid, (err, result) => {
            if(err){
                console.log("Error getting events");
                console.log(err.message);
                reject(err.message);
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
/* GetEventInfo gets the event's info from the database. It first gets the event's info from the database, then returns the event's info as a string.
    If there was an error getting the event's info, it returns "Error getting event info".
    
    @param args - The arguments eventID passed by the client
    @param DB - The database object 
    @returns "<title>,<details>,(<date>),<startTime>,<endTime>,<publicityType>,(<invitees>),<userid>" - The event's info
    @returns "Error getting event info" - If there was an error getting the event's info
*/
function GetEventInfo(args, DB){
    return new Promise(async (resolve, reject) =>{
        let eventid = args[0];
        let separator = ",";
        DB.GetEventInfo(eventid, (err, result) => {
            if(err){
                console.log("EventManagement.js/GetEventInfo: Error getting event info");
                console.log(err.message);
                reject("Error getting event info");
                return;
            }else{
                resolve(result[0].title + separator + result[0].details + separator + "(" + result[0].date + ")"+ separator + result[0].startTime + separator + result[0].endTime + separator + result[0].publicityType + separator + "(" + result[0].invitees + ")" + separator + result[0].userid);
            }
        });

    });
}
/* AddInvitee adds the invitee to the event's invitee list. It first gets the event's info, then checks if the invitee is already in the invitee list.
   If the invitee is already in the invitee list, it returns "Invitee already added". If there are no invitees for the event, it sets the invitee list to the invitee.
   If the invitee is not in the invitee list, it adds the invitee to the invitee list. Then, it updates the invitee list in the database if there were no errors.
   Finally, it returns "Invitee added" if there was no error, and returns the error if there was one.

    @param args - The arguments eventID and invitee passed by the client
    @param DB - The database object
    @returns "Invitee added" - If the invitee was successfully added
    @returns "Invitee already added" - If the invitee was already added
    @returns "Event not found" - If the event was not found
    @returns err.message - If there was an error getting user info or updating the events table in the database
*/
function AddInvitee(args, DB){
    return new Promise(async (resolve, reject) => {
        let eventID = args[0];
        let invitee = args[1];
        
        DB.GetUserInfo(invitee, (err, user) => {
            if(err){
                console.log(err.message);
                reject(err.message);
                return;
            }
        });
        DB.GetEventInfo(eventID, (err, event) => {
            if(err){
                console.log(err.message);
                reject(err.message);
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
                    reject(err.message);
                    return;
                }
            });
        });
    
    });
}
/* RemoveInvitee removes the invitee from the event's invitee list. It first gets the event's info, then checks if the invitee is in the invitee list.
   If the invitee is not in the invitee list, it returns "Invitee not found", if there are no invitees for the event it returns "No invitees to remove". 
   If the invitee is in the invitee list, it removes the invitee from the invitee list and updates the invitee list in the database. 
   Finally, it returns "Invitee removed" if there was no error, and returns the error if there was one.

    @param args - The arguments eventID and invitee passed by the client
    @param DB - The database object
    @returns "Invitee removed" - If the invitee was successfully removed
    @returns "Invitee not found" - If the invitee was not found
    @returns "No invitees to remove" - If there are no invitees to remove
    @returns err.message - If there was an error getting the event info

*/
function RemoveInvitee(args, DB){
    return new Promise(async (resolve, reject) => {
        let eventID = args[0];
        let invitee = args[1];
        DB.GetEventInfo(eventID, async (err, event) => {
            if(err){
                console.log(err.message);
                reject(err.message);
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
            }  
            DB.db.serialize(()=>{
                try{
                    invitees = invitees.join(",");
                    DB.UpdateTable('events', `invitees="${invitees}"`, `eventid=${event[0].eventid}`);
                    resolve("Invitee removed");
                    return;
                }catch(err){
                    reject(err.message);
                    return;
                }
            });
        });
    });
}
/* Uninvite removes the eventID frome the user's invited list. It first gets the user's info, then checks if 
   the user is invited to the event. If the user is not invited, it returns "User not invited". If the user is invited,
    it removes the eventID from the user's invited list and updates the user's invited list in the database. Finally, it
    returns "User uninvited" if there no error occured, and returns the error if there was one.

    @param args - The arguments username and eventID passed by the client
    @param DB - The database object
    @returns "User uninvited" - If the user was successfully uninvited
    @returns "User not invited" - If the user was not invited
    @returns err.message - If there was an error

*/
function Uninvite(args, DB){
    return new Promise(async (resolve, reject) =>{
        let username = args[0];
        let eventID = args[1];
        DB.GetUserInfo(username, (err, user) => {
            if(err){
                console.log('\n' + 'Error in Uninvite: Error getting user info');
                console.log(err.message);
                reject(err.message);
                return;
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
                    reject(err.message);
                }
            });
        });
    });
}

module.exports = {CreateEvent, DeleteEvent, GetEvents, AddInvitee,RemoveInvitee, GetEventInfo, WipeAllEvents};