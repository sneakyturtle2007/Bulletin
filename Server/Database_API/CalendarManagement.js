
function GetMonthEvents(args, DB){
    let userID = args[0];
    let year = args[1];
    let month = args[2];
    return new Promise( (resolve, reject) =>{
        console.log("CalendarManagement.js: Getting events for month");
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
                        eventDates = eventDates[0].split("/");
                        if (eventDates[0] == year && eventDates[1] == month) {
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
}
/*function GetFreeDaysInMonth(args, DB){
    let userID1 = args[0];
    let user2 = args[1];
    let year = args[2];
    let month = args[3]; 
    return new Promise( (resolve, reject) => {
        let user1Month = GetMonthEvents([userID1, date.])
    });
}*/
module.exports = {GetMonthEvents};