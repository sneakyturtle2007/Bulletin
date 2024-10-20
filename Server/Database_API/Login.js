function Login(args, db){
    return new Promise((resolve, reject) => {
        var username = args[0];
        var password = args[1];
        console.log('Username: ' + username);
        CheckLogin(username, password, db, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
            }
            if(result){
                resolve(result);
            }else{
                resolve(result);
            }
        });
    });
}
function CheckLogin(username, password, db,  callback){
    db.GetUserInfo(username, (err, user) => {
        if(err){
            console.log(err);
            callback(err, "Error: + " + err.ToString());
        }
        if(user.length == 0){
            console.log("User not found");
            callback(null, "User not found");
        }else if(user[0].password == password){
            console.log("User logged in");
            callback(null, "User logged in");
        }else{
            console.log("Incorrect login");
            callback(null, "Incorrect login");
        } 
    });
}

module.exports = {Login};