let date = new Date(Date.now());
let year = date.getFullYear();
let month = date.getMonth();
let datetitle = document.getElementById("calendarDate");
let eventindeces = [];

const months = [
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December",
];

async function Logout() {
    document.cookie = "";
    window.location.href = "../Main/Main.html";
}

function ToggleDropdown(panel, button) {
    var content = document.getElementById(panel);
    var buttonicon = document.getElementById(button);
    if (content.style.display === "none") {
        content.style.display = "block";
        buttonicon.textContent = "\u{25B2}";
    } else {
        content.style.display = "none";
        buttonicon.textContent = "\u{25BC}";
    }
}

function NavMonthForward() {
    month++;
    if (month > 11) {
        month = 0;
        year++;
    }
    datetitle.textContent = months[month] + " " + year;
    UpdateCalendar();
}

function NavMonthBack() {
    month--;
    if (month < 0) {
        month = 11;
        year--;
    }
    datetitle.textContent = months[month] + " " + year;
    UpdateCalendar();
}

function UpdateCalendar() {
    // clear calendar of previous dates
    for (i=0;i<42;i++) {
        var tempstring = "calendarbox" + i;
        document.getElementById(tempstring).textContent = "";
    }

    // fill with new dates
    var startingday = new Date(year,month,1).getDay();
    var endingday = new Date(year,month+1,0).getDate() + startingday - 1;

    for (i = 1; i <= new Date(year,month+1,0).getDate(); i++) {
        var tempstring = "calendarbox" + (startingday + i - 1);
        document.getElementById(tempstring).textContent = i;
    }
}

async function GetUserId() {
    var user = document.cookie.substring(5);
    var info = await fetch("/getuserinfo?username=" + user).then(res => res.json());
    var id = Number(info.substring(0,info.indexOf(",")));
    return id;
}

async function GetEvents(calendarboxnumber) {
    var user = await GetUserId();
    var eventsjsonned = await fetch("/getmonthevents?userid=" + user + "&year=" + year + "&month=" + (month+1)).then(res => res.json());
    let events = await JSON.parse(eventsjsonned);
    var calID = "calendarbox" + calendarboxnumber;
    var selectedDate = "2025/3/" + document.getElementById(calID).textContent;

    var eventdates;
    var startDate;
    var endDate;

    for (i = 0; i < events.length; i++) {
        eventdates = events[i].date;
        startDate = eventdates.substring(0,eventdates.indexOf(","));
        endDate = eventdates.substring(eventdates.indexOf(",") + 1);
        if (selectedDate === startDate || selectedDate === endDate) {
            console.log(i);
            eventindeces.push(i);
        }
    }
    DisplayEvents(events, startDate, endDate);
}

function DisplayEvents(events) {
    const parentElement = document.getElementById("eventslist");

    var titletext;
    var peopletext;
    var descriptiontext;

    console.log(eventindeces.length);
    for (i = 0; i < eventindeces.length; i++) {
        startday = (events[i].date).substring(0,(events[i].date).indexOf(","));
        endday = (events[i].date).substring((events[i].date).indexOf(",") + 1);

        titletext = events[eventindeces[i]].title;
        peopletext = events[eventindeces[i]].invitees;
        descriptiontext = events[eventindeces[i]].details;

        const root = document.createElement("li");

        const header = document.createElement("div");
        header.class = "eventheader";

        const title = document.createElement("h4");
        title.id = "eventtitle";
        title.textContent = titletext;

        const datetimecontainer = document.createElement("div");

        const date = document.createElement("label");
        date.id = "eventdate";
        date.textContent = startday + " - " + endday;

        const time = document.createElement("label");
        time.id = "eventtime";

        const peoplesection = document.createElement("div");
        peoplesection.class = "eventpeople";

        const dropdownheader1 = document.createElement("div");
        dropdownheader1.class = "dropdownheader";
        dropdownheader1.id = "peopledropdown";

        const dropdowntitle1 = document.createElement("label");
        dropdowntitle1.id = "dropdowntitle";
        dropdowntitle1.textContent = "Invitees";

        const dropdownbutton1 = document.createElement("button");
        dropdownbutton1.id = "peoplebutton";
        dropdownbutton1.class = "dropdownbutton";
        dropdownbutton1.textContent = "\u{25B2}";
        dropdownbutton1.onclick = "ToggleDropdown('peoplepanel,'peoplebutton')";

        const dropdowncontent1 = document.createElement("div");
        dropdowncontent1.class = "dropdowncontent";
        dropdowncontent1.id = "peoplepanel";
        dropdowncontent1.style = "display: none";

        const peoplelist = document.createElement("ul");
        peoplelist.id = "peoplelist";

        // come back to this and add the list of people that were invited to join the event;

        const eventdescription = document.createElement("div");
        eventdescription.class = "eventdescription";

        const dropdownheader2 = document.createElement("div");
        dropdownheader2.class = "dropdownheader";
        dropdownheader2.id = "descriptiondropdown";

        const dropdowntitle2 = document.createElement("label");
        dropdowntitle2.id = "dropdowntitle";
        dropdowntitle2.textContent = "Description";

        const dropdownbutton2 = document.createElement("button");
        dropdownbutton2.class = "dropdownbutton";
        dropdownbutton2.id = "descriptionbutton";
        dropdownbutton2.textContent = "\u{25B2}";
        dropdownbutton2.onclick = "ToggleDropdown('descriptionpanel','descriptionbutton')";

        const dropdowncontent2 = document.createElement("div");
        dropdowncontent2.class = "dropdowncontent";
        dropdowncontent2.id = "descriptionpanel";
        dropdowncontent2.style = "display: none";

        const descriptionbox = document.createElement("p");
        descriptionbox.class = "nospacing";
        descriptionbox.id = "descriptionbox";
        
        parentElement.appendChild(root);

        root.appendChild(header);
        root.appendChild(peoplesection);
        root.appendChild(eventdescription);
            
        header.appendChild(title);
        header.appendChild(datetimecontainer);

        datetimecontainer.appendChild(date);
        datetimecontainer.appendChild(time);

        peoplesection.appendChild(dropdownheader1);
        peoplesection.appendChild(dropdowncontent1);

        dropdownheader1.appendChild(dropdowntitle1);
        dropdownheader1.appendChild(dropdownbutton1);

        dropdowncontent1.appendChild(peoplelist);

        eventdescription.appendChild(dropdownheader2);
        eventdescription.appendChild(dropdowncontent2);

        dropdownheader2.appendChild(dropdowntitle2);
        dropdownheader2.appendChild(dropdownbutton2);

        dropdowncontent2.appendChild(descriptionbox);
    }
}
