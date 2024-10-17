
function CreateUser(args, db){
    return new Promise(async (resolve, reject) => {
        var username = args[0];
        var email = args[1];
        var password = args[2];
        
        //console.log('Username: ' + username);
        var result = db.CreateUser(username, email, password , (err, result) => {
            if(err){
                console.log(err);
                reject(err);
            }
            resolve(result);
        });
    });
}

module.exports = {CreateUser};