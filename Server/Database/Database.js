const sql = require('sqlite3').verbose();


function Start(){
    let db = new sql.Database('./database.db',  (err) => {
        if (err) {
            console.error(err.message);
        }
        console.log('Connected to the SQlite database.');
    });  
}
function CloseDatabase(){
    db.close((err) => {
        if (err) {
            console.error(err.message);
            console.log('Error closing the database connection');
        }
        console.log('Successfully closed the database connection.');
    });
}
// Table Management
    function DescribeTable(name, callback){
        db.all(`PRAGMA table_info(${name});`, (err,table) => {
            if(err){
                console.log(err.message);
                callback(err, null);
            }
            callback(null, table);
        });
    }

    function ShowTables(){
        db.all('SELECT name FROM sqlite_master WHERE type="table";', (err,tables) => {
            if(err){
                console.log(err.message);
            }
            console.log(tables);
        });
    }
    function CreateTable(name, listofparams){
        db.run(`CREATE TABLE ${name} (${listofparams});`, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`${name} Table created`);
        });
    }
    function DropTable(name){
        db.run(`DROP TABLE ${name};`, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`${name} Table dropped`);
        });
    }
// Information Management
    function DisplayTableContents(name, callback){
        
        db.all(`SELECT * FROM ${name};`, (err,rows) => {
            if(err){
                console.log(err.message);
                callback(err, null);
            }
            //console.log(rows);
            callback(null,rows);
        });
    }
    function DeleteFromTable(name, condition){
        
        db.run(`DELETE FROM ${name} WHERE ${condition};`, (err) => {
            if(err){
                console.log(err.message);
            }
            
            console.log(`Deleted ${condition} from ${name}`);
        });
        
    }
    function UpdateTable(name, values, condition){
        
        db.run(`UPDATE ${name} SET ${values} WHERE ${condition};`, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`Updated ${name}`);
        });
    }
    
    // Users Table
        function GetUserInfo(username, callback){
            db.serialize(() => {
                db.all(`SELECT * FROM users WHERE username = ?;`, [username], (err,rows) => {
                    if(err){
                        console.log(err.message);
                        callback(err, null);
                    }
                    callback(null, rows);
                    console.log(rows);
                });
            });
                
        }
        function CreateUser(username, password){
            
            let userExists = GetUserInfo(username, (err, rows) => {
                if(err){
                    console.log(err.message);
                    console.log("Error in getting user info");
                }
                if(rows.length > 0){
                    console.log("User already exists");
                    userExists = true;
                    return userExists;
                }else{
                    userExists = false;
                    db.run('INSERT INTO users (username, password) VALUES (?, ?);',[username, password], (err) => {
                        if(err){
                            console.log(err.message);
                        }
                        console.log(`Inserted ${username} into users`);
                    });
                    return userExists;
                }
            });
            if(userExists == false){
                
            }
        
        }  
            
        function DeleteUser(username){
            let condition = `username="${username}"`;
            DeleteFromTable('users', condition);
        }


function test(){
    db.serialize(() => {
        CreateTable('test', 'id INTEGER PRIMARY KEY, name TEXT');
        console.log("created table");
        ShowTables();

        DropTable('test');
        console.log("dropped table");
        ShowTables();

        CreateUser("admin", "admin");
        //DeleteUser('admin');

        DisplayTableContents('users', (err, rows) => {
            if(err){
                console.log(err.message);
            }
            console.log(rows);
        });

        let value = DescribeTable('users', (err, table) => {
            if(err){
                console.log(err.message);
            }else{
                //console.log(table);
                return table;
            }
        });
    });
}


module.export = {ShowTables, CreateTable, DropTable, DisplayTableContents,
                DeleteFromTable, UpdateTable, GetUserInfo, CreateUser, DeleteUser,
                DescribeTable, CloseDatabase, Start};