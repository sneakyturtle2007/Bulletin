const sql = require('sqlite3').verbose();

let db = new sql.Database('./database.db',  (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the SQlite database.');
});


// Table Management
    function ShowTables(){
        db.serialize(()=>{
            db.all('SELECT name FROM sqlite_master WHERE type="table";', (err,tables) => {
                if(err){
                    console.log(err.message);
                }
                console.log(tables);
            });
        });
        
    
    }
    function CreateTable(name, listofparams){
        let sql = `CREATE TABLE ${name} (${listofparams});`;
        db.serialize(()=>{
            db.run(sql, (err) => {
                if(err){
                    console.log(err.message);
                }
                console.log(`${name} Table created`);
            });
        });
            
    }
    function DropTable(name){
        let sql = `DROP TABLE ${name};`;
        db.run(sql, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`${name} Table dropped`);
        });
    }
// Information Management
    function InsertIntoTable(name, values){
        let sql = `INSERT INTO ${name} VALUES (${values});`;
        db.run(sql, (err) => {
            if(err){
                console.log(err.message);
            }
            console.log(`Inserted into ${name}`);
        });
    }
 
CreateTable('test', 'id INTEGER PRIMARY KEY, name TEXT');
console.log("created table");
ShowTables();

DropTable('test');
console.log("dropped table");
ShowTables();


db.close((err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Close the database connection.');
}); 

module.export = {ShowTables, CreateTable, DropTable, InsertIntoTable};