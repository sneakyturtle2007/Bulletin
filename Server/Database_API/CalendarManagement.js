
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

function GetBusyTimeInMonth(args, DB){
    let daysInMonth = [31,[28,29],31,30,31,30,31,31,30,31,30,31];
    
    let userID = args[0];
    let user2_Name = args[1];
    let year = args[2];
    let month = args[3];
    let lastDigitOfYear = parseInt(year)%10;
    if(lastDigitOfYear % 2 != 0){
        daysInMonth[1] = daysInMonth[1][1];
    }else{
        daysInMonth[1] = daysInMonth[1][0];
    }
    let busyTime = new Array(daysInMonth[parseInt(month)]);
    return new Promise( async (resolve, reject) => {
        
        try{
            let user1Month = await GetMonthEvents([userID, year, month], DB);
            let user2Month = await GetMonthEvents([user2_Name, year, month], DB);
        }catch(err){
            reject(err);
        }
        
        let date;
        let day;
        let eventStart;
        let eventEnd;
        user1Month = JSON.parse(user1Month);
        user2Month = JSON.parse(user2Month);
        if(user1Month.length > user2Month.length){
            busyTime = GetBusyTime(user1Month);
        }else{
            busyTime = GetBusyTime(user2Month);
        }
        resolve(JSON.stringify(busyTime));
    });
}
function GetBusyTime(month, busyTime){
    month.forEach(event => {
        date = event.date.split(",");
        date[0] = date[0].split("/");
        date[1] = date[1].split("/");
        for(let i = parseInt(date[0][2]); i < parseInt(date[1][2]); i++){
            eventStart = event.startTime;
            eventEnd = event.endTime;
            
            if(busyTime[i].length == 0){
                busyTime[i].push([eventStart, eventEnd]);
            }else{
                busyTime[i] = CalculatebusyTime(busyTime[i], eventStart, eventEnd);
            }
        }
    });
    return busyTime;
}
function CalculatebusyTime(time_s, eventStart, eventEnd){
    let day = [];
    let eventStartIndex = 0;
    let eventEndIndex = 0;
    for(let i = 0; i < time_s.length ; i++){
        eventStartIndex = i;
        for(let k = 0; k < 2; k++){
            if(eventStart <= time_s[i][k]){
                eventStartIndex = k > 0 ? i += 0.5 : i;
                i = time_s.length;
                break;
            }
        }
    }
    for(let i = parseInt(eventStartIndex); i < time_s.length; i++){
        eventEndIndex = i;
        for(let k = 0; k < 2; k++){
            if(eventEnd <= time_s[i][k]){
                eventEndIndex = k > 0 ? i += 0.5 : i;
                i = time_s.length; 
                break;
            }
        }
    }
    time_s.splice(parseInt(eventStartIndex+0.5), parseInt(eventEndIndex) - parseInt(eventStartIndex + 0.5));
    if(eventStartIndex % 1 == 0){
        eventEndIndex % 1 == 0 ? time_s.splice(eventStartIndex,0,[eventStart, eventEnd]) : time_s[parseInt(eventEndIndex)][0] = eventStart;    
    }else{
        if(eventEndIndex % 1 == 0){
            time_s[parseInt(eventStartIndex)][1] = eventEnd;
        }else{
            time_s[parseInt(eventStartIndex)][1] = time_s[parseInt(eventEndIndex)][1];
            time_s.splice(eventStartIndex + 1, 1);
        }
    }
    return time_s;
}
module.exports = {GetMonthEvents, GetBusyTimeInMonth};
