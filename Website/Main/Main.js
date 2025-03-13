function OnClickSignup() {
    window.location.href = "../Signup/Signup.html";
}

function OnClickLogin() {
    var user = document.getElementById("user").value.trim();
    var pass = document.getElementById("pass").value.trim();
    var error = document.getElementById("error");
    if (user === undefined | user === "" | pass === undefined | pass === "") {
        error.textContent = "both fields must be filled";
    } else {
        // server call
        // "login|user|pass"
        // if that fails (if/else | try/catch):
        error.textContent = "incorrect login credentials";
        // else:
        window.location.href = "../Home/Home.html";
    }
}
