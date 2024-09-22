const net = require('net');
const https = require('https');
const sql = require('sqlite3').verbose();
const fs = require('fs');
const path = require('path');

const express = require('express');
const app = express();
const Port_HTTPS = 443;

app.use(express.static('../Website'));

app.listen(Port_HTTPS, () => {
    console.log('HTTP Server is running on port ' + Port_HTTPS + '.');
});

app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname,'../Website', 'Main.html'));
});



const Port_TCP = 22;
const host = "127.0.0.1";
const server = net.createServer();

var sockets = [];

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

server.listen(Port_TCP, host, () => {
    console.log('TCP Server is running on port ' + Port_TCP + '.');
});


server.on('error', (err) => {
    console.log('Error: ', err);
});