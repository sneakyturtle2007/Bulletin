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
        DeleteFromTable(name, condition){
            this.db.serialize(() => {
                this.db.run(`DELETE FROM ${name} WHERE ${condition};`, (err) => {
                    if(err){
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
                            this.db.run('INSERT INTO users (username, email, password) VALUES (?, ?, ?);',[username, email, password], (err) => {
                                if(err){
                                    callback(err, null);
                                    throw err;
                                }
                                //console.log(`Inserted ${username} into users`);
                            });
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
            this.db.serialize(()=>{
                this.db.all("SELECT * FROM events", (err, res) => {
                    for(let i = 0; i < res.length; i++){
                        this.DeleteFromTable('events', `eventid=${res[i].eventid}`);
                    }
                });
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
                    this.db.serialize(() => {
                        this.GetAllEvents(userid, (err, events) => {
                            if(err){
                                console.log("Error when getting events in CreateEvent function " +err.message);
                                callback("Error getting events", null);
                                return;
                            }
                            let currentEvent;
                            for(let i = 0; i < events.length; i++){
                                if(events[i].eventid > eventID){
                                    eventID = events[i].eventid;
                                    currentEvent = events[i];
                                }
                            }
                            
                            this.InviteInvitees(eventID, currentEvent.invitees, (err, res) => {
                                if(err){
                                    console.log("Error in InviteInvitees function");
                                }
                                callback(err, null);
                                return;                
                            });
                            callback(null, `Event created ${eventID}` );
                            return;
                        });
                    });
                    
                });
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
                                console.log(user[0].invited);
                                this.UpdateTable('users', `invited=${user[0].invited}`, `id=${user[0].id}`);
                            }
                        });
                    }
                    //this.UpdateTable('users', `invited=${user[0].invited}`, `id=${user[0].id}`);
                    callback(null, "Invitees added");
                });
            });
        }

        GetEventInfo(eventid, callback){
            this.db.serialize(() => {
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
            });
            
        }
        GetAllEvents(userid, callback){
            this.db.serialize(() =>{
                this.db.all(`SELECT * FROM events WHERE userid=${userid};`, (err, events) =>{
                    if(err){
                        console.log(err.message);
                        callback("Error getting events", null);
                        return;
                    }
                    callback(null, events);
                });
            });
        }
        DeleteEvent(eventid){
            this.DeleteFromTable('events', `eventid=${eventid}`);
        }
    // Testing
        UnitTests(){
            this.db.serialize(() => {

                try{
                    this.CreateTable('test', 'id INTEGER PRIMARY KEY, name TEXT');
                    console.log("Creating Table Test" + ' \u2713');
                }catch(err){
                    console.log("Creating Table Test" + ' \u2717');
                    console.log(err.message);
                }
                try{
                    this.DropTable('test');
                    console.log("Dropping Table Test" + ' \u2713');
                }catch(err){
                    console.log("Dropping Table Test" + ' \u2717');
                    console.log(err.message);
                }

                try{
                    this.CreateUser("admin","admin@gmail.com", "admin", (err, message) => {
                        if(err){
                            throw err;
                        }
                        this.db.serialize(() =>{
                            try{
                                this.db.serialize(() =>{ 
                                    this.CreateUser("test2","admin@gmail.com", "admin", (err, message) => {
                                            if(err){
                                                throw err;
                                            }
                                        });
                                        this.GetUserInfo('admin', (err, rows) => {
                                            if(err){
                                                throw err;
                                            }
                                            if(rows.length > 1){
                                                console.log("ese");
                                                throw new Error("Too many users with the same username");
                                            }
                                        });
                                });  
                            }catch(err){
                                throw err;
                            }
                            try{
                                this.DeleteUser('test2');
                                console.log("Deleting User Test" + ' \u2713');
                            }catch(err){
                                console.log("Deleting User Test" + ' \u2717');
                                console.log(err.message);
                            }
                        });
                    });
                    console.log("Creating User Test" + ' \u2713');
                }catch(err){
                    console.log("Creating User Test" + ' \u2717');
                    console.log(err.message);
                }

                try{
                    this.DisplayTableContents('users', (err, rows) => {
                        if(err){
                            throw err;
                        }
                    });
                    console.log("Displaying Users Test" + ' \u2713');
                }catch(err){
                    console.log("Displaying Users Test" + ' \u2717');
                    console.log(err.message);
                }

                try{
                    this.DescribeTable('users', (err, table) => {
                        if(err){
                            throw err;
                        }
                    });
                    console.log("Describing Users Test" + ' \u2713');
                }catch(err){
                    console.log("Describing Users Test" + ' \u2717');
                    console.log(err.message);
                } 

                try{
                    this.CreateEvent(1, 'test', '2021/03/03', '12:00', '13:00', 'test', 'public', 'test', 'test', (err, message) => {
                        if(err){
                            throw err;
                        }else{
                            console.log("Creating Event Test" + ' \u2713');
                        } 
                        this.db.serialize(() =>{
                            try{
                                this.DeleteEvent('test');
                                console.log("Dropping Event Test" + ' \u2713');
                            }catch(err){
                                console.log("Dropping Event Test" + ' \u2717');
                                console.log(err.message);
                            }
                        });
                    });
                }catch(err){
                    console.log("Creating Event Test" + ' \u2717');
                    console.log(err.message);
                }
            
            });
        }
}
//db = new Database('database.db');
//db.UnitTests();
module.exports = Database;