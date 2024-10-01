// Gets the required modules for the website
    const net = require('net');
    const https = require('https');
    const sql = require('sqlite3').verbose();
    const fs = require('fs');
    const path = require('path');
// importing local files
    const Login = require('./Login.js');
    const Database = require('./Database.js');
    Database.start();
// Gets the required modules for the server
    const express = require('express');
    const app = express();
    const Port_HTTPS = 443;

// Server Connections
    // Loads the website
        app.use(express.static('../Website'));

    // Starts the server on the port
        app.listen(Port_HTTPS, () => {
            console.log('HTTP Server is running on port ' + Port_HTTPS + '.');
        });

    // Static pages = Pages that don't change
        app.get('/', function (req, res) {
            res.sendFile(path.join(__dirname,'../Website', 'Main.html'));
        });
    // Dynamic pages = Pages that change based off of user input


// Conn Info/Create Server
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

// API SETUP
    
    // login
        app.get('/api/login', async function (req, res) {
            
            res.json(result);
        })