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
function AddFriend(args, db){
    return new Promise(async (resolve, reject) => {
        username = args[0];
        friend = args[1];
        db.serialize(()=>{
            db.GetUserInfo(username, (err, user) => {
                if(err){
                    reject(err);
                }else{
                    user.friends.push(friend);
                    db.serialize(()=>{
                        try{
                            db.UpdateTable("users", user.friends, `userid=${user.userid}`);
                            resolve("Friend Added");
                        }catch(err){
                            reject(err);
                        }
                    });
                }
            });
        });
    });        
}
module.exports = {CreateUser, DeleteUser, GetUserInfo, AddFriend};