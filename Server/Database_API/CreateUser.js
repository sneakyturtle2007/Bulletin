
function CreateUser(args, db){
    return new Promise(async (resolve, reject) => {
        var username = args[0];
        var password = args[1];
        console.log('Username: ' + username);
        var result = db.CreateUser(username, password, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
            }
            if(result){
                resolve('true\n');
            }else{
                resolve('false\n');
            }
        });
    });
}

module.exports = {CreateUser};