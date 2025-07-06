async function OnClickSignup() {
    window.location.href = "../Signup/Signup.html";
}

async function OnClickLogin() {
    var user = document.getElementById("user").value.trim();
    var pass = document.getElementById("pass").value.trim();
    var error = document.getElementById("error");
    if (user === undefined | user === "" | pass === undefined | pass === "") {
        error.textContent = "both fields must be filled";
    } else {
        var login = await fetch("/login?username=" + user + "&password=" + pass).then(res => res.json()).catch(err => error.textContent = "incorrect login credentials");
        if (login !== undefined) {
            if (login.includes("User not found") || login.includes("Incorrect login")) {
                error.textContent = "invalid login credentials";
            } else if (login.includes("User logged in")) {
                document.cookie = "user=" + user + "; path=/";
                window.location.href = "../Home/Home.html";
            } else if (login.includes("ERROR: ")) {
                error.textContent = login.substring(7);
            }
        }
    }
}
