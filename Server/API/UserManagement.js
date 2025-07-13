/* CreateUser creates a new user in the database.

    @param args - The array of arguments that contains the username, email, and password
    @param DB - The database object
    @returns result - The result of the query
    @returns err.message - If there was an error
*/
function CreateUser(args, DB){
    return new Promise( (resolve, reject) => {
        let username = args[0];
        let email = args[1];
        let password = args[2];
        
        DB.CreateUser(username, email, password , (err, result) => {
            if(err){
                console.log(err.message);
                reject(err.message);
            }else{
                resolve(result);
            }
        });
    });
}
/* DeleteUser deletes the user from the database.

    @param args - The array of arguments that contains the username
    @param DB - The database object
    @returns "User deleted" - If the user was deleted
    @returns err.message - If there was an error
*/  
function DeleteUser(args, DB){
    return new Promise( (resolve, reject) => {
        let username = args[0];
        try{
            DB.DeleteUser(username);
            resolve("User deleted");
        }catch(err){
            console.log(err.message);
            reject(err.message);
            return;
        }
    });
}
/* GetUserInfo gets the user's information from the database and returns it as a string.

    @param args - The array of arguments that contains the userID
    @param DB - The database object
    @returns "<userID>,<username>,<email>,(<friends>),(<invited>)" - The user's information
    @returns "User not found" - If the user was not found
    @returns err.message - If there was an error
*/
function GetUserInfo(args, DB){
    let userID = args[0];
    let separator = ",";
    return new Promise( (resolve, reject) => {

        DB.GetUserInfo(userID, (err, result) => {
            if(err){
                console.log(err.message);
                reject(err.message);
            }else{
                if(result.length > 0){
                    resolve(result[0].id + separator + result[0].username + separator + result[0].email + separator + "(" + result[0].friends + ")"+ separator + "(" + result[0].invited + ")");
                }else{
                    resolve("User not found");
                }
            }
        });
    });
}
/* AddFriend adds a friend to the user's friend list. It first gets the user's friend list and then adds the friend to the list.
    It then updates the user's friend list in the database.

    @param args - The array of arguments that contains the username and the friend to add
    @param DB - The database object
    @returns "Friend added" - If the friend was added
    @returns "Friend not found" - If the friend was not found
    @returns "Friend already added" - If the friend was already added
    @returns err - If there was an error
*/
function AddFriend(args, DB){
    let username = args[0];
    let friend = args[1];

    return new Promise( (resolve, reject) => {
        
        DB.GetUserInfo(friend, (err, user) =>{
            if(err){
                console.log(err.message);
                reject(err);
                return;
            }else if(user.length > 0){

                DB.GetUserInfo(username, (err, user) => {
                    if(err){
                        reject(err.message);
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
                                reject(err.message);
                            }
                        });
                    }
                });
            }else{
                resolve("Friend not found");
                return;
            }
        });
        
        
        
    });        
}
/* RemoveFriend removes a friend from the user's friend list. It first gets the user's friend list and then removes the friend from the list. 
   It then updates the user's friend list in the database.

    @param args - The array of arguments that contains the username and the friend to remove
    @param DB - The database object
    @returns "Friend removed" - If the friend was removed
    @returns "Friend not found" - If the friend was not found
    @returns err.message - If there was an error

*/
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
