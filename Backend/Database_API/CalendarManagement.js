
/*GetMonthEvents receives the userID, year, and month as arguments and returns a promise that resolves to a json stringified array of the events
  for the month. It first checks if the userID is a string, if it is, it gets the userID from the database. It then calls GetAllEvents
  to get all the events for the user. Then loops through the events and checks if the start or end date of the event matches the year and month
  and returns the events that match as a json stringified array.

    @param args - The array of arguments that contains the userID, year, and month
    @param DB - The database object
    @returns monthEvents - The json stringified array of the events for the month
    @returns "No events found" - If there are no events for the month
    @returns err.message - If there was an error
*/
async function GetMonthEvents(args, DB){
    let userID = await args[0];
    let year = args[1];
    let month = args[2];
    return new Promise( (resolve, reject) =>{
        try{
            if(isNaN(Number(userID))){
                DB.GetUserInfo(userID, (err, result) => {
                    if(err){
                        console.log("CalendarManagement.js/GetMonthEvents: Error getting user info");
                        reject(err.message);
                        return;
                    }
                    userID = result[0].id;
                });
            }
            DB.GetAllEvents(userID, async (err, result) => {
                if(err){
                    console.log("CalendarManagment.js/GetMonthEvents: Error getting events");
                    console.log(err.message);
                    reject(err.message);
                }else{
                    let events = result;
                    if(events.length != 0){
                        let monthEvents = [];
                        let eventDates;

                        for (let event of events) {
                            eventDates = event.date.split(",");
                            eventStart = eventDates[0].split("/");
                            eventEnd = eventDates[1].split("/");
                            if (eventStart[0] == year && eventStart[1] == month || eventEnd[0] == year && eventEnd[1] == month) {
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
        }catch(e){
            console.log("CalendarManagement.js/GetMonthEvents: Error getting month events");
            console.log(e.message);
            reject(e.message);
        }
        
    });
}
/*GetBusyTimeInMonth is the main function that is called to get the busy times of the month for two users. It first gets the user2 ID
  then calls GetMonthEvents for both users. It then checks if the monthEvents are empty, if they are, it returns an empty array.
  If they are not, it calls GetBusyTime to get the busy times of the month for both users. Finally, it
  returns a json stringified version of the busyTime array.

    @param args - The array of arguments that contains the userID, user2 name, year, and month
    @param DB - The database object
    @returns busyTime - The json stringified array of the busy times of the month
    @returns err.message - If there was an error
*/
function GetBusyTimeInMonth(args, DB){
    
    let userID = args[0];
    let otherUsers = args[1].split(",");
    let otherUserID;
    let year = 0;
    let month = 0;
    let daysInMonth = [31,[28,29],31,30,31,30,31,31,30,31,30,31];
    let lastDigitOfYear = 0;
    try{
        year = parseInt(args[2]);
        month = parseInt(args[3]);
        lastDigitOfYear = year%10;
    }catch(e){
        console.log("CalendarManagement.js/GetBusyTimeInMonth: Error getting month events");
        console.log(e.message);
        throw(e);
    }
    if(lastDigitOfYear % 2 != 0){
        daysInMonth[1] = daysInMonth[1][1];
    }else{
        daysInMonth[1] = daysInMonth[1][0];
    }
    let busyTime = new Array(daysInMonth[month +1]);
    for(let i = 0; i < busyTime.length; i++){
        busyTime[i] = [];
    }
    let user1Month;
    let otherUserMonths = [];
    return new Promise( async (resolve, reject) => {

        user1Month = await GetMonthEvents([userID, year, month], DB);
        try{
            for(let i = 0 ; i < otherUsers.length; i++){
                otherUsers[i] = otherUsers[i].trim();
                console.log(i);
                DB.GetUserInfo(otherUsers[i], async (err, result) => {
                    if(err){
                        console.log("CalendarManagement.js/GetBusyTimeInMonth: Error getting user2 ID");
                        throw(err);
                    }
                    otherUserID = result[0].id;
                    otherUserMonths.push(await GetMonthEvents([otherUserID, year, month], DB));
                });
                
            }
            if(user1Month == "No events found"){
                user1Month = [];
            }else{
                user1Month = JSON.parse(user1Month);
            }
            for(let i = 0; i < otherUserMonths.length; i++){
                if(otherUserMonths[i] == "No events found"){
                    otherUserMonths[i] = [];
                }else{
                    otherUserMonths[i] = JSON.parse(otherUserMonths[i]);
                }
            }
            
            busyTime = GetBusyTime(user1Month, busyTime, month);
            for(let i = 0; i < otherUserMonths.length; i++){
                busyTime = GetBusyTime(otherUserMonths[i], busyTime, month);
            }
        }catch(e){
            console.log("CalendarManagement.js/GetBusyTimeInMonth: Error getting month events");
            console.log(e);
            reject(e);
            return;
        }
        resolve(JSON.stringify(busyTime));
    });
}
/* GetBusyTime goes through the provided array of events and finds the start and end day and month of each event
   Then it caps the start or end day to be confined within the days of the month. Finally, it loops through the days
   of the month and calls CalculateBusyTime if there are existing times in the day, or pushes a new array with the 
   start and end time of the event if there are no existing times.

    @param monthEvents - The array of events for the month
    @param busyTime - The array of the month with the busy times
    @param month - The month of the events
    @returns busyTime - The updated array of the month with the busy times
*/
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
                    busyTime[i] = CalculateBusyTime(busyTime[i], eventStart, eventEnd);
                }
            }
        }
    });
    
    return busyTime;
}
/* CalculaterBusyTime cycles through the main array of the day (time variable) and finds the index of where an existing time
   is greater than the event start(repeats this process for the event end, starting after the index of eventStart).
   Once the indexes are found, the function will remove all imbetween arrays as long as the fractional index doesn't 
   lie in the array being deleted, then the function will check to see if the indexes are fractional, if they are, it will 
   check what event index is fractional and combine the arrays accordingly, if not, then it will create a new array with 
   the event start time and end time at the eventStartIndex.

    @param time - The array of the day that contains the busy times
    @param eventStart - The start time of the event
    @param eventEnd - The end time of the event
    @returns time - The updated array of the day with the event times added
*/
function CalculateBusyTime(time, eventStart, eventEnd){
    let eventStartIndex = 0;
    let eventEndIndex = 0;
    let busyTimeLength = time.length;
    if(time.length == 1 && time[0][0] == eventStart && time[0][1] == eventEnd){
        return time;
    }
    for(let i = 0; i < busyTimeLength ; i++){
        eventStartIndex = i;
        console.log(`eventStartIndex: ${eventStartIndex}`);
        for(let k = 0; k < 2; k++){
            if(time[i][k] && eventStart <= time[i][k]){
                eventStartIndex = k > 0 ? eventStartIndex += 0.5 : i;
                i = busyTimeLength;
                break;
            }else if(i + 1 == busyTimeLength){
                eventStartIndex ++;
                break;
            }
        }

    }
    for(let i = parseInt(eventStartIndex); i < busyTimeLength; i++){
        eventEndIndex = i
        for(let k = 0; k < 2; k++){
            if(time[i][k] && eventEnd <= time[i][k]){
                eventEndIndex = k > 0 ? eventEndIndex += 0.5 : i;
                i = busyTimeLength; 
                break;
            }else if(i + 1 == busyTimeLength){
                eventEndIndex ++;
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
