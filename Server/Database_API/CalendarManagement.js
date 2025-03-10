
function GetMonthEvents(args, DB){
    let userID = args[0];
    let year = args[1];
    let month = args[2];
    return new Promise( (resolve, reject) =>{
        console.log("CalendarManagement.js: Getting events for month: " + month + " and year: " + year + " for user: " + userID);
        DB.GetAllEvents(userID, async (err, result) => {
            if(err){
                console.log("CalendarManagment.js/GetMonthEvents: Error getting events");
                console.log(err.message);
                reject("Error getting events");
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
    let user2_ID;
    let year = args[2];
    let month = args[3];
    let lastDigitOfYear = parseInt(year)%10;
    if(lastDigitOfYear % 2 != 0){
        daysInMonth[1] = daysInMonth[1][1];
    }else{
        daysInMonth[1] = daysInMonth[1][0];
    }
    let busyTime = new Array(daysInMonth[parseInt(month)]);
    for(let i = 0; i < busyTime.length; i++){
        busyTime[i] = [];
    }
    let user1Month;
    let user2Month;
    return new Promise( async (resolve, reject) => {
        

        
        DB.GetUserInfo(user2_Name, (err, result) => {
            if(err){
                console.log("CalendarManagement.js/GetBusyTimeInMonth: Error getting user2 ID");
                reject(err);
            }
            user2_ID = result[0].id;
        });
        
        user1Month = await GetMonthEvents([userID, year, month], DB);
        user2Month = await GetMonthEvents([user2_ID, year, month], DB);
            
        if(user1Month == "No events found"){
            user1Month = [];
        }else{
            user1Month = JSON.parse(user1Month);
        }
        if(user2Month == "No events found"){
            user2Month = [];
        }else{
            user2Month = JSON.parse(user2Month);
        }
        busyTime = GetBusyTime(user1Month, busyTime);
        busyTime = GetBusyTime(user2Month, busyTime);
        console.log(JSON.stringify(busyTime));
        resolve(JSON.stringify(busyTime));
    });
}
function GetBusyTime(month, busyTime){
    let date;
    month.forEach(event => {
        date = event.date.split(",");
        date[0] = date[0].split("/");
        date[1] = date[1].split("/");
        let startDay = parseInt(date[0][2]);
        let endDay = parseInt(date[1][2]);
        let startMonth = parseInt(date[0][1]);
        let endMonth = parseInt(date[1][1]);
        if(startMonth < endMonth){
            endDay = busyTime.length;
        }    
        for(let i = startDay -1; i < endDay; i++){
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
function CalculatebusyTime(time, eventStart, eventEnd){
    let eventStartIndex = 0;
    let eventEndIndex = 0;

    for(let i = 0; i < time.length ; i++){
        eventStartIndex = i;

        for(let k = 0; k < 2; k++){
            if(eventStart <= time[i][k]){
                eventStartIndex = k > 0 ? i += 0.5 : i;
                i = time.length;
                break;
            }
        }

    }
    for(let i = eventStartIndex; i < time.length; i++){
        eventEndIndex = i;
        for(let k = 0; k < 2; k++){
            if(eventEnd <= time[i][k]){
                eventEndIndex = k > 0 ? i += 0.5 : i;
                i = time.length; 
                break;
            }
        }
    }
    time.splice(parseInt(eventStartIndex+0.5), parseInt(eventEndIndex) - parseInt(eventStartIndex + 0.5));
    if(eventStartIndex % 1 == 0){
        eventEndIndex % 1 == 0 ? time.splice(eventStartIndex,0,[eventStart, eventEnd]) : time[parseInt(eventEndIndex)][0] = eventStart;    
    }else{
        if(eventEndIndex % 1 == 0){
            time[parseInt(eventStartIndex)][1] = eventEnd;
        }else{
            time[parseInt(eventStartIndex)][1] = time[parseInt(eventEndIndex)][1];
            time.splice(eventStartIndex + 1, 1);
        }
    }
    return time;
}
module.exports = {GetMonthEvents, GetBusyTimeInMonth};
