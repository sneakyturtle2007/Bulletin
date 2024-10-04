

var db;

function Login(db, connection){
    db = db;
    connection.write('Enter username');
    connection.on('data', function(data){

        var username = data.toString().trim();
        connection.write('Enter password');

        connection.on('data', function(data){
            var password = data.toString().trim();
            if(CheckLogin(username, password)){
                connection.write('Logged in');
                connection.on('data', function(data){
                    if(data == 'exit'){
                        connection.end();
                    }
                });
            }else{
                connection.write('Login failed');
            }
        });
    });
}
function CheckLogin(username, password){
    db.GetUserInfo(username, (err, user) => {
        if(err){
            console.log(err);
            return null;
        }
        if(user.length == 0){
            console.log('User not found');
            return null;
        }else if(user[0].password == password){
            console.log('User logged in');
            return true;
        }else{
            console.log('Incorrect password');
            return false;
        } 
    });
}

module.export = {Login};