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
        this.db.close((err) => {
            if (err) {
                console.log('Error closing the database connection');
                throw err;
            }
            //console.log('Successfully closed the database connection.');
        });
    }
    // Table Management
        DescribeTable(name, callback){
            this.db.all(`PRAGMA table_info(${name});`, (err,table) => {
                if(err){
                    callback(err, null);
                }
                callback(null, table);
            });
        }

        ShowTables(){
            this.db.all('SELECT name FROM sqlite_master WHERE type="table";', (err,tables) => {
                if(err){
                    throw err;
                }
                console.log(tables);
            });
            return true;
        }
        CreateTable(name, listofparams){
            this.db.run(`CREATE TABLE ${name} (${listofparams});`, (err) => {
                if(err){
                    throw err;
                }
                //console.log(`${name} Table created`);
            });
            return true;
        }
        DropTable(name){
            this.db.run(`DROP TABLE ${name};`, (err) => {
                if(err){
                    throw err;
                }
                //console.log(`${name} Table dropped`);
            });
            return true;
        }
    // Information Management
        DisplayTableContents(name, callback){
            
            this.db.all(`SELECT * FROM ${name};`, (err,rows) => {
                if(err){
                    callback(err, null);
                    return;
                }
                //console.log(rows);
                callback(null,rows);
            });
        }
        DeleteFromTable(name, condition){
            
            this.db.run(`DELETE FROM ${name} WHERE ${condition};`, (err) => {
                if(err){
                    throw err;
                }
                //console.log(`Deleted ${condition} from ${name}`);
            });
            
        }
        UpdateTable(name, values, condition){
            
            this.db.run(`UPDATE ${name} SET ${values} WHERE ${condition};`, (err) => {
                if(err){
                    throw err;
                }
                //console.log(`Updated ${name}`);
            });
        }
        
    // users Table
        GetUserInfo(username, callback){
            this.db.serialize(() => {
                if(username.includes('@')){
                    this.db.all(`SELECT * FROM users WHERE email = ?;`, [username], (err,user) => {
                        if(err){
                            console.log(err.message);
                            callback(err, null);
                            return;
                        }
                        callback(null, user);
                        //console.log(user);
                    });
                }else{
                    this.db.all(`SELECT * FROM users WHERE username = ?;`, [username], (err,user) => {
                        if(err){
                            console.log(err.message);
                            callback(err, null);
                            return;
                        }
                        callback(null, user);
                        //console.log(user);
                    });    
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
                                //console.log("Email taken");
                                callback(null, "Email taken");
                                return;
                            }
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
        CreateEvent(userID, title, date, startTime, endTime, location, publicityType, invitees, details, callback){
            this.db.run('INSERT INTO events (userID, title, date, startTime, endTime, location, publicityType, invitees, details) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);',[userID, title, date, startTime, endTime, location, publicityType, invitees, details], (err) => {
                if(err){
                    console.log(err.message);
                    callback(err, null);
                    return false;
                }
                console.log(`Inserted ${title} into events`);
                callback(null, "Event created");
                return true;
            });
        }
        GetEventInfo(title, callback){
            this.db.all(`SELECT * FROM events WHERE title = ?;`, [title], (err,event) => {
                if(err){
                    console.log(err.message);
                    callback(err, null);
                    return;
                }
                callback(null, event);
                //console.log(event);
            });
        }
        DeleteEvent(title){
            this.DeleteFromTable('events', `title="${title}"`);
        }
        GetAllEvents(userid, callback){
            this.db.all(`SELECT * FROM events WHERE userid="${userid};`, (err, events) =>{
                if(err){
                    console.log(err.message);
                    callback(err, null);
                    return;
                }
                callback(null, events);
            });
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
db = new Database('database.db');
db.UnitTests();
module.exports = Database;