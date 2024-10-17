// Gets the required modules for the website
    const net = require('net');
    const https = require('https');
    const sql = require('sqlite3').verbose();
    const fs = require('fs');
    const express = require('express');
    const path = require('path');
// importing local files
    const Director = require('./Database_API/Director.js');
    const Login = require('./Database_API/Login.js');
    const Database = require('./Database/Database.js');

// Starting Database & API Director
const db = new Database();

// Starting Express
const app = express();
app.use(express.static(path.join(__dirname, "../Website")));
app.use(express.json());

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../Website/Main/Main.html'));
});


// Connection Info/Create Server
    const Port_TCP = 22;
    const host = "127.0.0.1";
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
                    console.log(response);
                    console.log(response.toString());
                    socket.write(response.toString());
                }catch(err){
                    console.log('Error: ' + err);
                    socket.write('Error: ' + err);
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


    //MIGHT BE USED FOR REMOVING THE BUGS FROM YOUR SKIN
    server.on('error', (err) => {
        console.log('Error: ', err);
    });

