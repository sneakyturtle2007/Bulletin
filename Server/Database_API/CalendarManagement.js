
function GetMonthEvents(args, DB){
    return new Promise(async (resolve, reject) =>{
        let month = args[0];
        let year = args[1];
        let userID = args[2];
        console.log("CalendarManagement.js: Getting events for month");
        DB.db.serialize(() => {
            DB.GetAllEvents(userID, (err, result) => {
                if(err){
                    console.log("Error getting events");
                    console.log(err.message);
                    resolve("Error getting events");
                    return;
                }else{
                    let events = result;
                    let monthEvents = [];
                    let event;
                    let eventDates;
                    for(let i = 0; i < events.length; i++){
                        event = events[i];
                        eventDates = event.date.split(",");
                        if(eventDates[0].split("/")[2] == year && eventDates[0].split("/")[0] == month){
                            monthEvents.push(event);
                        }
                    }
                }
            });
        });
    });
}
module.exports = {};