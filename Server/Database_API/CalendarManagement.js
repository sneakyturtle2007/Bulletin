

function GetMonthEvents(args, DB){
    let userID = args[0];
    let year = args[1];
    let month = args[2];
    return new Promise( (resolve, reject) =>{
        if(isNaN(Number(userID))){
            DB.GetUserInfo(userID, (err, result) => {
                if(err){
                    console.log("CalendarManagement.js/GetMonthEvents: Error getting user info");
                    reject(err.message);
                }
                userID = result[0].id;
            });
        }
        console.log("CalendarManagement.js: Getting events for month: " + month + " and year: " + year + " for user: " + userID);
        DB.GetAllEvents(userID, async (err, result) => {
            if(err){
                console.log("CalendarManagment.js/GetMonthEvents: Error getting events");
                console.log(err.message);
                reject(err.message);
            }else{
                let events = result;
                console.log("Amount of events: " + events.length);
                if(events.length != 0){
                    let monthEvents = [];
                    let eventDates;

                    for (let event of events) {
                        eventDates = event.date.split(",");
                        eventStart = eventDates[0].split("/");
                        eventEnd = eventDates[1].split("/");
                        if (eventStart[0] == year && eventStart[1] == month || eventEnd[0] == year && eventEnd[1] == month) {
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
    
    let userID = args[0];
    let user2_Name = args[1];
    let user2_ID;
    let year = args[2];
    let month = parseInt(args[3]) + 1;
    let daysInMonth = [31,[28,29],31,30,31,30,31,31,30,31,30,31];
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
                reject(err.message);
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
        busyTime = GetBusyTime(user1Month, busyTime, month);
        busyTime = GetBusyTime(user2Month, busyTime, month);
        console.log(JSON.stringify(busyTime));
        resolve(JSON.stringify(busyTime));
    });
}
function GetBusyTime(monthEvents, busyTime, month){
    let date;
    monthEvents.forEach(event => {
        let startDay = 0;
        let endDay = 0;
        let startMonth = 0;
        let endMonth = 0;       
        if(monthEvents.length != 0){
            date = event.date.split(",");
            date[0] = date[0].split("/");
            date[1] = date[1].split("/");
            startDay = parseInt(date[0][2]);
            endDay = parseInt(date[1][2]);
            startMonth = parseInt(date[0][1]);
            endMonth = parseInt(date[1][1]);
            if(month < endMonth){
            endDay = busyTime.length;
            }else if(month > startMonth){
                startDay = 0;
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
        }
    });
    
    return busyTime;
}
function CalculatebusyTime(time, eventStart, eventEnd){
    let eventStartIndex = 0;
    let eventEndIndex = 0;
    let busyTimeLength = time.length;
    for(let i = 0; i < busyTimeLength ; i++){
        eventStartIndex = i;

        for(let k = 0; k < 2; k++){
            if(time[i][k] && eventStart <= time[i][k]){
                eventStartIndex = k > 0 ? eventStartIndex += 0.5 : i;
                i = time.length;
                break;
            }
        }

    }
    for(let i = parseInt(eventStartIndex); i < busyTimeLength; i++){
        eventEndIndex = i;
        for(let k = 0; k < 2; k++){
            if(time[i][k] && eventEnd <= time[i][k]){
                eventEndIndex = k > 0 ? eventEndIndex += 0.5 : i;
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
