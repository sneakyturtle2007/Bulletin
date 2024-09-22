const sql = require('sqlite3').verbose();

let db = new sql.Database('./database.db',  (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the SQlite database.');
});

function ShowTable(){

}

/*db.serialize(() =>{
    db.run('CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT NOT NULL, email TEXT NOT NULL DEFAULT "unknown", password TEXT NOT NULL)');
});*/
db.serialize(() =>{
   db.each('SELECT name FROM sqlite_master WHERE type="table";', (err,tables) => {
       if(err){
          console.log(err.message);
       }
       console.log(tables);
   })
});

db.close((err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Close the database connection.');
}); 