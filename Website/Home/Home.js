let date = new Date(Date.now());
let year = date.getFullYear();
let month = date.getMonth();
let datetitle = document.getElementById("calendarDate");

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
    console.log(id);
    return id;
}

async function GetEvents(calendarbox) {
    var user = await GetUserId();
    var events = await fetch("/getmonthevents?userid=" + user + "&year=" + year + "&month=" + month).then(res => res.json());
    console.log(events);
    // add events that are on the day chosen to a list
    // display each event to 
}
