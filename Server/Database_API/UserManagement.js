function CreateUser(args, DB){
    return new Promise(async (resolve, reject) => {
        var username = args[0];
        var email = args[1];
        var password = args[2];
        
        DB.CreateUser(username, email, password , (err, result) => {
            if(err){
                reject(err);
            }else{
                resolve(result);
            }
        });
    });
}
function DeleteUser(args, DB){
    return new Promise(async (resolve, reject) => {
        username = args[0];
        try{
            DB.DeleteUser();
        }catch(err){
            reject(err);
            return;
        }
        resolve("User Deleted");
        
    });
}
function GetUserInfo(args, DB){
    return new Promise(async (resolve, reject) => {
        username = args[0];
        DB.GetUserInfo(username, (err, result) => {
            if(err){
                reject(err);
            }else{
                resolve(result[0].id + "," + result[0].username + "," + result[0].email + "," + result[0].friends + "\n");
            }
        });
    });
}
function AddFriend(args, DB){
    return new Promise(async (resolve, reject) => {
        username = args[0];
        friend = args[1];
        
        DB.GetUserInfo(username, (err, user) => {
            if(err){
                reject(err);
            }else{
                friends = user.friends;
                if(user[0].friends.toString() == "NONE"){
                    friends = `${friend}`;
                }
                
                DB.db.serialize(()=>{
                    try{
                        DB.UpdateTable("users", `friends=${friends}`, `id=${user.id.toString()}`);
                        resolve("Friend Added");
                    }catch(err){
                        reject(err);
                    }
                });
            }
        });

    });        
}
module.exports = {CreateUser, DeleteUser, GetUserInfo, AddFriend};