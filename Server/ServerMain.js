// Gets the required modules for the website
    const net = require('net');
    const https = require('https');
    const express = require('express');
    const path = require('path');
// importing local files
    const Director = require('./Database_API/Director.js');
    const Database = require('./Database/Database.js');

// Starting Database & API Director
const db = new Database('./Database/database.db');

// Starting Express 
const app = express();
const webserver = https.createServer(app);
app.use(express.static(path.join(__dirname, "../Website")));
app.use(express.json());


app.get('/' , (req, res) => { // Sets what URL, after the base URL,the server will listen to and initializes the function that will be executed. 
    res.sendFile(path.join(__dirname, '../Website/Main/Main.html'));/* Sends the corresponding HTML file to the client. 
    __dirname is used because express required the full path, and this just makes it more compatible.*/
});
/* To add another URL to listen to, just repeat the function above with the desired URL 
in the single quotes in the app.get(), and the desired HTML file's path in the single quotes 
in the res.send(path.join()).

Example:
app.get('/example', (req, res) => {
    res.sendFile(path.join(__dirname, '../Website/Example/Example.html'));
}
*/



// Connection Info/Create Server
    const Port_TCP = 8000;
    const host = "0.0.0.0";
    const server = net.createServer();

// TCP Server
    var sockets = [];

    // OnServerConnect()
    server.on('connection', function(socket) {
        sockets.push(socket);
        console.log('Connected: ' + socket.remoteAddress + ':' + socket.remotePort);

        socket.on('end',function(){
            console.log('Disconnected: ' + socket.remoteAddress + ':' + socket.remotePort);
            sockets.splice(sockets.indexOf(socket), 1);
        });
        socket.on('data', async function(data){
            console.log('Received: ' + data);
            let response;
            if(data){
                try{
                    response = await Director.ProcessData(data, db, socket);
                    console.log('Response: ' + response);
                    
                    socket.write(response.toString() + '\n');
                }catch(err){
                    console.log('Error: ' + err);
                    socket.write(err.message);
                }
                
            }
        });
        socket.on('error', function(err){
            console.log('Error: ' + err);
        });

    });
    
    //Starts the server on the port and IP
    server.listen(Port_TCP, host, () => {
        console.log('TCP Server is running on port ' + Port_TCP + '.');
    });
    webserver.listen(443, () => {
        console.log('HTTPS Server is running on port 443.');
    }).on('error', (err) => {
        console.lot('Error: ', err);    
    });

    //MIGHT BE USED FOR REMOVING THE BUGS FROM YOUR SKIN
    server.on('error', (err) => {
        console.log('Error: ', err);
    });

