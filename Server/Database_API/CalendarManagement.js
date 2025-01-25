
function GetMonthEvents(args, DB){
    return new Promise( (resolve, reject) =>{
        let userID = args[0];
        let year = args[1];
        let month = args[2];
        
        
        console.log("CalendarManagement.js: Getting events for month");
        DB.db.serialize(() => {
            DB.GetAllEvents(userID, async (err, result) => {
                if(err){
                    console.log("CalendarManagment.js/GetMonthEvents: Error getting events");
                    console.log(err.message);
                    resolve("Error getting events");
                }else{
                    let events = result;
                    console.log("Amount of events: " + events.length);
                    if(events.length != 0){
                        let monthEvents = [];
                        let eventDates;

                        for (let event of events) {
                            eventDates = event.date.split(",");
                            if (eventDates[0].split("/")[0] == year && eventDates[0].split("/")[1] == month) {
                                console.log("Found event for month: ", event);
                                monthEvents.push(event);
                            }
                        }
                        if(monthEvents.length != 0){
                            resolve(JSON.stringify(monthEvents));
                        }else{
                            resolve("No events found");
                        }
                    }
                        
                        
                        
                }
            });
        });
    });
}
module.exports = {GetMonthEvents};