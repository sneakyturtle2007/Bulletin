const sql = require('sqlite3').verbose();

class Database{

    constructor(){
        this.db = new sql.Database('./Database/database.db',  (err) => {//'database.db'
            if (err) {
                console.error(err.message);
            }else{
                console.log('Connected to the SQlite database.');
            }
        });
    }
    // Database Management
    CloseDatabase(){
        this.db.close((err) => {
            if (err) {
                console.error(err.message);
                console.log('Error closing the database connection');
            }
            console.log('createdUserfully closed the database connection.');
        });
    }
    // Table Management
        DescribeTable(name, callback){
            this.db.all(`PRAGMA table_info(${name});`, (err,table) => {
                if(err){
                    console.log(err.message);
                    callback(err, null);
                }
                callback(null, table);
            });
        }

        ShowTables(){
            this.db.all('SELECT name FROM sqlite_master WHERE type="table";', (err,tables) => {
                if(err){
                    console.log(err.message);
                }
                console.log(tables);
            });
        }
        CreateTable(name, listofparams){
            this.db.run(`CREATE TABLE ${name} (${listofparams});`, (err) => {
                if(err){
                    console.log(err.message);
                }
                console.log(`${name} Table created`);
            });
        }
        DropTable(name){
            this.db.run(`DROP TABLE ${name};`, (err) => {
                if(err){
                    console.log(err.message);
                }
                console.log(`${name} Table dropped`);
            });
        }
    // Information Management
        DisplayTableContents(name, callback){
            
            this.db.all(`SELECT * FROM ${name};`, (err,rows) => {
                if(err){
                    console.log(err.message);
                    callback(err, null);
                }
                //console.log(rows);
                callback(null,rows);
            });
        }
        DeleteFromTable(name, condition){
            
            this.db.run(`DELETE FROM ${name} WHERE ${condition};`, (err) => {
                if(err){
                    console.log(err.message);
                }
                
                console.log(`Deleted ${condition} from ${name}`);
            });
            
        }
        UpdateTable(name, values, condition){
            
            this.db.run(`UPDATE ${name} SET ${values} WHERE ${condition};`, (err) => {
                if(err){
                    console.log(err.message);
                }
                console.log(`Updated ${name}`);
            });
        }
        
    // Users Table
        GetUserInfo(username, callback){
            this.db.serialize(() => {
                if(username.includes('@')){
                    this.db.all(`SELECT * FROM users WHERE email = ?;`, [username], (err,user) => {
                        if(err){
                            console.log(err.message);
                            callback(err, null);
                        }
                        callback(null, user);
                        //console.log(user);
                    });
                }else{
                    this.db.all(`SELECT * FROM users WHERE username = ?;`, [username], (err,user) => {
                        if(err){
                            console.log(err.message);
                            callback(err, null);
                        }
                        callback(null, user);
                        //console.log(user);
                    });    
                }
                
            });
                
        }

        CreateUser(username, email, password, callback){
                this.GetUserInfo(username, (err, rows) => {
                    let createdUser;
                    if(err){
                        console.log(err.message);
                        console.log("Error in getting user info");
                        callback(err, "Error in getting user info");
                    }
                    if(rows.length > 0 ){
                        console.log("Username taken");
                        createdUser = false;
                        callback(null, "Username taken");
                    }else{
                        this.GetUserInfo(email, (err, rows) => {
                            if(err){
                                console.log(err.message);
                                console.log("Error in getting user info");
                                callback(err, "Error in getting user info");
                            }
                            if(rows.length > 0 ){
                                console.log("Email taken");
                                createdUser = false;
                                callback(null, "Email taken");
                            }
                            createdUser = true;
                            this.db.run('INSERT INTO users (username, email, password) VALUES (?, ?, ?);',[username, email, password], (err) => {
                                if(err){
                                    console.log(err.message);
                                }
                                console.log(`Inserted ${username} into users`);
                            });
                            callback(null, "User created");
                        });
                    }
                });
        }  
            
        DeleteUser(username){
            let condition = `username="${username}"`;
            this.DeleteFromTable('users', condition);
        }


    test(){
        this.db.serialize(() => {
            /*this.CreateTable('test', 'id INTEGER PRIMARY KEY, name TEXT');
            console.log("created table");
            this.ShowTables();

            this.DropTable('test');
            console.log("dropped table");
            this.ShowTables();*/

            //CreateUser("admin", "admin");
            this.DeleteUser('test2');

            this.DisplayTableContents('users', (err, rows) => {
                if(err){
                    console.log(err.message);
                }
                console.log(rows);
            });

            let value = this.DescribeTable('users', (err, table) => {
                if(err){
                    console.log(err.message);
                }else{
                    //console.log(table);
                    return table;
                }
            });
        });
    }
}

module.exports = Database;