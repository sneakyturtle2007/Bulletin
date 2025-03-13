function CancelCreation() {
    window.location.href = "../Main/Main.html";
}

function CreateAccount() {
    //Get all 4 input fields
	//check if username is taken
	//if yes: throw error
	//check if account has already been created with that email
	//if yes: throw error
	//check if passwords match
	//if no: throw error
	//if yes: create account
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
        // try "createuser|user|email|pass2"
            window.location.href = "../Home/Home.html";
        // catch user already exists
            // error.textContent = "that username or email is taken";
    }
}

function IsNullOrWhiteSpace(input) {
    if (input === undefined || input === "")
        return true;
    else
        return false;
}
