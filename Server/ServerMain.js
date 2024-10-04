// Gets the required modules for the website
    const net = require('net');
    const https = require('https');
    const sql = require('sqlite3').verbose();
    const fs = require('fs');
    const path = require('path');
// importing local files
    const Login = require('./Database_API/Login.js');
    const Database = require('./Database/Database.js');

// Starting Database 
const db = new Database();

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
        socket.on('data', function(data){
            console.log('Received: ' + data);
            if(data == 'Login'){
                console.log("logging in");
                Login.Login(db, socket);
            }
            socket.write('Echo: ' + data);
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

