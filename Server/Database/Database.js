const sql = require('sqlite3').verbose();

class Database{

    constructor(databaseFile){
        this.db = new sql.Database(databaseFile,  (err) => {//'database.db''./Database/database.db'
            if (err) {
                throw err;
            }else{
                console.log('Connected to the SQlite database.');
            }
        });
    }
    // Database Management
    CloseDatabase(){
        this.db.serialize(() => {
            this.db.close((err) => {
                if (err) {
                    console.log('Error closing the database connection');
                    throw err;
                }
                //console.log('Successfully closed the database connection.');
            });
        });
        
    }
    // Table Management
        DescribeTable(name, callback){
            this.db.serialize(() => {
                this.db.all(`PRAGMA table_info(${name});`, (err,table) => {
                    if(err){
                        callback(err, null);
                    }
                    callback(null, table);
                });
            });
            
        }

        ShowTables(){
            this.db.serialize(() => {
                this.db.all('SELECT name FROM sqlite_master WHERE type="table";', (err,tables) => {
                    if(err){
                        throw err;
                    }
                    console.log(tables);
                });
            });
            
            return true;
        }
        CreateTable(name, listofparams){
            this.db.serialize(() => {
                this.db.run(`CREATE TABLE ${name} (${listofparams});`, (err) => {
                    if(err){
                        throw err;
                    }
                    //console.log(`${name} Table created`);
                });
            });
                
            return true;
        }
        DropTable(name){
            this.db.serialize(() => {
                this.db.run(`DROP TABLE ${name};`, (err) => {
                    if(err){
                        throw err;
                    }
                    //console.log(`${name} Table dropped`);
                });
            });
                
            return true;
        }
    // Information Management
        DisplayTableContents(name, callback){
            this.db.serialize(() => {
                this.db.all(`SELECT * FROM ${name};`, (err,rows) => {
                    if(err){
                        callback(err, null);
                        return;
                    }
                    //console.log(rows);
                    callback(null,rows);
                });
            });
            
        }
        DeleteFromTable(table, condition){
            this.db.serialize(() => {
                this.db.run(`DELETE FROM ${table} WHERE ${condition};`, (err) => {
                    if(err){
                        console.log(`Database.js/DeleteFromTable: ${err}`);
                        console.log(`Entered Command: DELETE FROM ${table} WHERE ${condition};`);
                        throw err;
                    }
                    //console.log(`Deleted ${condition} from ${name}`);
                });
            });
        
        }
        UpdateTable(table, values, condition){
            this.db.serialize(() => {
                this.db.run(`UPDATE ${table} SET ${values} WHERE ${condition};`, (err) => {
                    if(err){

                        console.log("Update Table Error: " + err.message);
                        console.log("Entered Command: " + `UPDATE ${table} SET ${values} WHERE ${condition};`);
                        throw err;
                    }
                    //console.log(`Updated ${name}`);
                });
            });
        }
        /*
            If you are inputing text as a value, enter it with quotations.
            Example: this.InsertIntoTable("users", "username, email, password", `"${username}", "${email}", "${password}"`);
        */
        InsertIntoTable(table, variables, values){
            this.db.serialize(()=>{
                this.db.run(`INSERT INTO ${table} (${variables}) VALUES (${values})`, (err)=>{
                    if(err){
                        console.log(`Database.js/InsertIntoTable: ${err}`);
                        throw err;
                    }
                });
            });
        }
    // users Table
        GetUserInfo(username, callback){
            this.db.serialize(() => {
                if(username){
                    if(!isNaN(Number(username))){
                        this.db.all(`SELECT * FROM users WHERE id = ${username};`, (err,user) => {
                            if(err){
                                console.log(err.message);
                                callback("Error getting user", null);
                                return;
                            }
                            callback(null, user);
                            return;
                            //console.log(user);
                        });
                    }
                
                    if(username.includes('@')){
                        this.db.all(`SELECT * FROM users WHERE email="${username}";`, (err,user) => {
                            if(err){
                                console.log(err.message);
                                callback("Error getting user", null);
                                return;
                            }
                            console.log("\n retrieved users: " + user);
                            console.log("\n used username: " +username);
                            callback(null, user);
                            
                            //console.log(user);
                        });
                    }else{
                        this.db.all(`SELECT * FROM users WHERE username = "${username}";`, (err,user) => {
                            if(err){
                                console.log(err.message);
                                callback("Error getting user", null);
                                return;
                            }
                            callback(null,user);
                            //console.log(user);
                        });    
                    }
                
                }else{
                    callback('No username or email provided', null);
                }
            });
        }

        CreateUser(username, email, password, callback){
            this.GetUserInfo(username, (err, rows) => {
                if(err){
                    //console.log("Error in getting user info");
                    callback(err, "Error in getting user info");
                    return;
                }
                if(rows.length > 0 ){
                    //console.log("Username taken");
                    callback(null, "Username taken");
                    return;
                }else{
                    this.GetUserInfo(email, (err, rows) => {
                        if(err){
                            //console.log("Error in getting user info");
                            callback(err, "Error in getting user info");
                            return;
                        }
                        if(rows.length > 0 ){
                            console.log("Email taken");

                            callback(null, "Email taken");
                            return;
                        }
                        console.log(email);
                        console.log("Creating user");
                        this.InsertIntoTable("users", "username, email, password", `"${username}", "${email}", "${password}"`);
                        callback(null, "User created");

                    });
                }
            });
        }  
        DeleteUser(username){
            this.DeleteFromTable('users', `username="${username}"`);
        }
    // events Table
        WipeAllEvents(){
            this.db.all("SELECT * FROM events", (err, res) => {
                for(let i = 0; i < res.length; i++){
                    this.DeleteFromTable('events', `eventid=${res[i].eventid}`);
                }
            });
        }
        CreateEvent(userid, title, date, startTime, endTime, publicityType, invitees, details, callback){
            this.db.serialize(() => {
                this.db.run(`INSERT INTO events (userID, title, date, startTime, endTime, publicityType, invitees, details) VALUES (${userid}, "${title}", "${date}", "${startTime}", "${endTime}", "${publicityType}", "${invitees}", "${details}");`, (err) => {
                    if(err){
                        console.log(err.message);
                        callback("Error creating event", null);
                        return;
                    }
                    let eventID = 0;
                    console.log(`Inserted ${title} into events`);
                    
                    this.GetLatestEvent(userid,(err,event) =>{
                        if(err){
                            console.log("Database.js/CreateEvent: " + err.message);
                            callback(err);
                        }
                        this.InviteInvitees(event.eventid, event.invitees, (err, res) => {
                            if(err){
                                console.log("Error in InviteInvitees function");
                            }
                            callback("Error inviting invitees", null);
                        });
                        callback(null, `Event created ${event.eventid}` );
                    });
                });
            });
            
        }
        DeleteEvent(eventid){
            this.DeleteFromTable('events', `eventid=${eventid}`);
        }
        GetEventInfo(eventid, callback){
            this.db.all(`SELECT * FROM events WHERE eventid=${eventid};`, (err,event) => {
                if(err){
                    console.log("debug");
                    console.log(err.message);
                    callback("Error getting event", null);
                    return;
                }
                if(!event.length == 0){
                    callback(null, event);
                }else{
                    callback("Event not found", null);
                }
                //console.log(event);
            });
        }
        GetAllEvents(userid, callback){
            this.db.all(`SELECT * FROM events WHERE userid=${userid};`, (err, events) =>{
                if(err){
                    console.log(err.message);
                    callback("Error getting events", null);
                    return;
                }
                callback(null, events);
            });
        }
        GetLatestEvent(userID, callback){
            this.db.get(`SELECT * FROM events WHERE eventid=(SELECT max(eventid) FROM events WHERE userid=${userID})`, (err, event) =>{
                if(err){
                    console.log("Database.js/GetLatestEvent: " + err.message);
                    callback("Error getting event", null);
                }
                callback(null, event);
            });
        }
        InviteInvitees(eventid, invitees, callback){
            this.db.serialize(() => {
                this.GetEventInfo(eventid, (err, event) => {
                    if(err){
                        console.log(err.message);
                        callback("Error inviting invitees", null);
                        return;
                    }
                    invitees = invitees.split(',');
                    console.log(invitees.length);
                    for(let i = 0; i < invitees.length; i++){
                        this.GetUserInfo(invitees[i], (err, user) => {
                            if(err){
                                console.log("testingtesting" + err.message);
                                callback(err, null);
                                return;
                            }
                            //console.log( user);
                            if(user[0]){
                                //console.log(user[0].invitees);
                                if(user[0].invited == "NONE"){
                                    user[0].invited = `${eventid}`;
                                }else if(!user[0].invited.includes(eventid)){
                                    user[0].invited = `${user[0].invited},${eventid}`;
                                }else{
                                    console.log("User already invited");
                                }
                                this.UpdateTable('users', `invited="${user[0].invited}"`, `id=${user[0].id}`);
                            }
                        });
                    }
                    //this.UpdateTable('users', `invited=${user[0].invited}`, `id=${user[0].id}`);
                    callback(null, "Invitees added");
                });
            });
        } 
}
/*db = new Database('database.db');
db.GetMonthEvents(2, "2021 + 2",(err, events) =>{
    if(err){
        console.log(err);
        return;
    }
    console.log(events);
});*/
module.exports = Database;