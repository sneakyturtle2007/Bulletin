function Login(args, db){
    return new Promise((resolve, reject) => {
        var username = args[0];
        var password = args[1];
        db.GetUserInfo(username, (err, user) => {
            if(err){
                console.log(err.message);
                reject(err.message);
                return;
            }
            if(user.length == 0){
                resolve("User not found");
            }else if(user[0].password == password){
                resolve("User logged in");
            }else{
                resolve("Incorrect login");
            } 
        });

    });
}


module.exports = {Login};