function CreateUser(args, DB){
    return new Promise( (resolve, reject) => {
        let username = args[0];
        let email = args[1];
        let password = args[2];
        
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
    return new Promise( (resolve, reject) => {
        let username = args[0];
        try{
            DB.DeleteUser(username);
            resolve("User deleted");
        }catch(err){
            reject(err);
            return;
        }
    });
}
function GetUserInfo(args, DB){
    let userID = args[0];
    return new Promise( (resolve, reject) => {
        DB.GetUserInfo(userID, (err, result) => {
            if(err){
                //console.log(err.message);
                resolve('User not found');
            }else{
                if(result.length > 0){
                    resolve(result[0].id + "," + result[0].username + "," + result[0].email + ",(" + result[0].friends + "),(" + result[0].invited + ")");
                }else{
                    resolve("User not found");
                }
            }
        });
    });
}
function AddFriend(args, DB){
    let username = args[0];
    let friend = args[1];

    return new Promise( (resolve, reject) => {
        
        DB.GetUserInfo(friend, (err, user) =>{
            if(err){
                resolve("Friend not found");
                return;
            }
        });
        DB.GetUserInfo(username, (err, user) => {
            if(err){
                reject(err);
            }else{
                friends = user[0].friends.toString();
                if(friends == "NONE"){
                    friends = `${friend}`;
                }else if(!friends.includes(friend)){
                    friends = `${friends}`+ `,${friend}`;
                }else{
                    resolve("Friend already added");
                    return;
                }
                
                DB.db.serialize(()=>{
                    try{
                        DB.UpdateTable("users", `friends="${friends}"`, `id=${user[0].id}`);
                        resolve("Friend added");
                    }catch(err){
                        reject(err);
                    }
                });
            }
        });
        
        
    });        
}
function RemoveFriend(args, DB){
    return new Promise( (resolve, reject) => {
        let username = args[0];
        let friend = args[1];
        
        DB.GetUserInfo(username, (err, user) => {
            if(err){
                console.log(err.message);
                reject(err);
                return;
            }else{
                friends = user[0].friends.toString();
                if(friends == "NONE"){
                    resolve("Friend not found");
                    return;
                }else if(friends.includes(friend)){
                    friends = friends.replace(friend + ",", "");
                }else{
                    resolve("Friend not found");
                    return;
                }
                
                DB.db.serialize(()=>{
                    try{
                        DB.UpdateTable("users", `friends="${friends}"`, `id=${user[0].id}`);
                        resolve("Friend removed");
                    }catch(err){
                        reject(err.message);
                    }
                });
            }
        });
    });
}
module.exports = {CreateUser, DeleteUser, GetUserInfo, AddFriend, RemoveFriend};
