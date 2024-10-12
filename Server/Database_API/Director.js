const login = require('./Login.js');

async function ProcessData(data, db, socket){
    if(data){
        var message = data.toString().trim();
        var command = message.split(' ')[0];
        var args = message.split(' ').slice(1);
        switch(command){
            case 'login':
                let result = await login.Login(args, db);
                return result;
                break;
            case 'exit':
                socket.end();
                break;
            default:
                return "Unknown Command\n";
        }
    }
}

module.exports = {ProcessData};