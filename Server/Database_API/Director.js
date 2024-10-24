const login = require('./Login.js');
const usermanagement = require('./UserManagement.js');
const eventmanagement = require('./EventManagement.js');

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
                result = await usermanagement.CreateUser(args, db);
                return result;
                break;
            case 'getuserinfo':
                result = await usermanagement.GetUserInfo(args, db);
                return result;
                break;
            case 'addfriend':
                result = await usermanagement.AddFriend(args, db);
                return result;
                break;
            case 'removefriend':
                result = await usermanagement.RemoveFriend(args, db);
                return result;
                break;
            case 'deleteuser':
                result = await usermanagement.DeleteUser(args, db);
                return result;
                break;
            case 'createevent':
                result = await eventmanagement.CreateEvent(args, db);
                return result;
                break;
            case 'addinvitee':
                result = await eventmanagement.AddInvitee(args, db);
                return result;
                break;
            case 'getevents':
                result = await eventmanagement.GetEvents(args, db);
                return result;
                break;
            case 'geteventinfo':
                result = await eventmanagement.GetEventInfo(args, db);
                return result;
                break;
            case 'deleteevent':
                result = await eventmanagement.DeleteEvent(args, db);
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