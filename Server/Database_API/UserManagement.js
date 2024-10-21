function CreateUser(args, db){
    return new Promise(async (resolve, reject) => {
        var username = args[0];
        var email = args[1];
        var password = args[2];
        
        db.CreateUser(username, email, password , (err, result) => {
            if(err){
                reject(err);
            }else{
                resolve(result);
            }
        });
    });
}
function DeleteUser(args, db){
    return new Promise(async (resolve, reject) => {
        username = args[0];
        try{
            db.DeleteUser();
        }catch(err){
            reject(err);
            return;
        }
        resolve("User Deleted");
        
    });
}
function GetUserInfo(args, db){
    return new Promise(async (resolve, reject) => {
        username = args[0];
        db.GetUserInfo(username, (err, result) => {
            if(err){
                reject(err);
            }else{
                resolve(result);
            }
        });
    });
}
module.exports = {CreateUser, DeleteUser, GetUserInfo};