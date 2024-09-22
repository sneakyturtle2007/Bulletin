const sql = require('sqlite3').verbose();

let db = new sql.Database('./database.db',  (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the in-memory SQlite database.');
});
/*db.serialize(() =>{
    db.run('CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT NOT NULL, email TEXT NOT NULL DEFAULT "unknown", password TEXT NOT NULL)');
});*/
db.close((err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Close the database connection.');
});