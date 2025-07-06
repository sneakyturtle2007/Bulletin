async function CancelCreation() {
    window.location.href = "../Main/Main.html";
}

async function CreateAccount() {
	var error = document.getElementById("error");
    var user = document.getElementById("user").value.trim();
    var email = document.getElementById("email").value.trim();
    var pass1 = document.getElementById("pass1").value.trim();
    var pass2 = document.getElementById("pass2").value.trim();
    if (IsNullOrWhiteSpace(user) ||
        IsNullOrWhiteSpace(email) ||
        IsNullOrWhiteSpace(pass1) ||
        IsNullOrWhiteSpace(pass2)) {
        error.textContent = "all fields must be filled";
    } else if (pass1 !== pass2) {
        error.textContent = "passwords must match";
    } else {
        var createuser = await fetch("/signup?username=" + user + "&email=" + email + "&password=" + pass2).then(res => res.json());
        if (createuser !== undefined) {
            if (createuser.includes("Username taken")) {
                error.textContent = createuser;
            } else if (createuser.includes("ERROR: ")){
                error.textContent = creatuser.substring(7);
            } else {
                document.cookie = "user=" + user + "; path=/";
                window.location.href = "../Home/Home.html";
            }
        }
    }
}

function IsNullOrWhiteSpace(input) {
    if (input === undefined || input === "")
        return true;
    else
        return false;
}
