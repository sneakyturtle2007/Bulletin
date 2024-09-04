const net = require('net');
const port = 443;
const host = 'localhost';

const server = net.createServer();
server.listen(port, host, () => {
    console.log('TCP Server is running on port ' + port + '.');
});