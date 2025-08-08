const http = require('http');
const Director = require('./Director.js');
const Database = require('./Database.js');

const DB_FILE = 'database.db'; // Ensure this path is correct relative to server.js
const PORT = 3000; // This should match the nodeAPIURL in your main.go

const db = new Database(DB_FILE);

const server = http.createServer(async (req, res) => {
    // Only accept POST requests for API commands
    if (req.method !== 'POST') {
        res.writeHead(405, { 'Content-Type': 'text/plain' });
        res.end('Method Not Allowed: Only POST requests are supported.');
        return;
    }

    // Read the request body
    let body = '';
    req.on('data', chunk => {
        body += chunk.toString(); // convert Buffer to string
    });

    req.on('end', async () => {
        try {
            console.log(`Received command from Go server: ${body}`);
            // Process the data using your Director.js
            const result = await Director.ProcessData(body, db, null); // Pass null for socket as it's HTTP
            console.log(`Sending response to Go server: ${result}`);

            res.writeHead(200, { 'Content-Type': 'text/plain' });
            res.end(result);
        } catch (error) {
            console.error('Error processing request:', error);
            res.writeHead(500, { 'Content-Type': 'text/plain' });
            res.end(`Internal Server Error: ${error.message}`);
        }
    });
});

server.listen(PORT, () => {
    console.log(`Node.js API server listening on port ${PORT}`);
    console.log(`Connecting to database: ${DB_FILE}`);
});

process.on('SIGINT', () => {
    console.log('Closing database connection...');
    db.CloseDatabase();
    process.exit();
});
