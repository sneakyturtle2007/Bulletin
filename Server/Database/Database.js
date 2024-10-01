const sql = require('sqlite3').verbose();

let db = new sql.Database('./database.db',  (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the SQlite database.');
});


// Table Management
    function DescribeTable(name, callback){
        db.all(`PRAGMA table_info(${name});`, (err,table) => {
            if(err){
                console.log(err.message);
                callback(err, null);
            }
            console.log(table);
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
            console.log(rows);
            callback(null,rows);
        });
    }
    function InsertIntoTable(name, values){
    
        db.run(`INSERT INTO ${name} VALUES (${values});`, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`Inserted into ${name}`);
        });
    }
    function DeleteFromTable(name, condition){
        
        db.run(`DELETE FROM ${name} WHERE ${condition};`, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`Deleted from ${name}`);
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
            db.all(`SELECT username=${username} FROM users;`, (err,rows) => {
                if(err){
                    console.log(err.message);
                    callback(err, null);
                }
                callback(null, rows);
                console.log(rows);
            });

        }
        function CreateUser(username, password){
           
            db.run(`INSERT INTO users (username, password) VALUES (${username}, ${password});`, (err) => {
                if(err){
                    console.log(err.message);
                }
                console.log(`Inserted into users`);
            });

        }



db.serialize(() => {
    CreateTable('test', 'id INTEGER PRIMARY KEY, name TEXT');
    console.log("created table");
    ShowTables();

    DropTable('test');
    console.log("dropped table");
    ShowTables();
    
    CreateUser('admin', 'admin');

    DisplayTableContents('users', (err, rows) => {
        if(err){
            console.log(err.message);
        }
        console.log(rows);
    });
});

let value = DescribeTable('users', (err, table) => {
    if(err){
        console.log(err.message);
    }else{
        //console.log(table);
        return table;
    }
});

db.close((err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Close the database connection.');
}); 

module.export = {ShowTables, CreateTable, DropTable, InsertIntoTable};