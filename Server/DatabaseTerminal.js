const sql = require('sqlite3').verbose();

let db = new sql.Database('./Server/database.db', (err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Connected to the in-memory SQlite database.');
});
while(true){
    
}
db.close((err) => {
    if (err) {
        console.error(err.message);
    }
    console.log('Close the database connection.');
});