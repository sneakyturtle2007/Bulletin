const login = require('./Login.js');
const createuser = require('./CreateUser.js');

async function ProcessData(data, db, socket){
    if(data){
        var message = data.toString().trim();
        var command = message.split(' ')[0];
        var args = message.split(' ').slice(1);
        let result;
        switch(command){
            case 'login':
                result = await login.Login(args, db);
                return result;
                break;
            case 'createuser':
                result = await createuser.CreateUser(args, db);
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