
function Login(data, db){
    return new Promise((resolve, reject) => {
        var username = data[0];
        var password = data[1];
        console.log('Username: ' + username);
        CheckLogin(username, password, db, (err, result) => {
            if(err){
                console.log(err);
                reject(err);
            }
            if(result){
                resolve('Login successful\n');
            }else{
                resolve('Login failed\n');
            }
        });
    });
    
}
function CheckLogin(username, password, db,  callback){
    db.GetUserInfo(username, (err, user) => {
        if(err){
            console.log(err);
            callback(err, false);
        }
        if(user.length == 0){
            console.log('User not found');
            callback(null, false);
        }else if(user[0].password == password){
            console.log('User logged in');
            callback(null, true);
        }else{
            console.log('Incorrect login');
            callback(null, false);
        } 
    });
}

module.exports = {Login};