function UserTest(username, email, password, client){
    client.write(`createuser ${username} ${email} ${password}`);
    client.on('data', (data) => {
        value = 0;
        try{
            value = Number(data.toString().trim().split(',')[0])
        }catch(err){
            console.log(err);
        }

        if(data.toString().trim() == 'User created'){
            client.write(`createuser example ${email} ${password}`);

        }else if(data.toString().trim() == 'Email taken'){
            client.write(`createuser ${username} example@gmail.com ${password}`);

        }else if(data.toString().trim() == 'Username taken'){
            client.write(`login ${email} ${password}`);
        
        }else if(data.toString().trim() == 'User logged in'){
            client.write(`getuserinfo ${username}`);
        
        }else if(value  > 0){
            client.write(`addfriend ${username} example`);
            
        }else if(data.toString().trim() == 'Friend added'){
            client.write(`addfriend ${username} example`);
        
        }else if(data.toString().trim() == 'Friend already added'){
            client.write(`deleteuser ${username}`);
        
        }else if(data.toString().trim() == 'User deleted'){
            console.log("User Test" + ' \u2713');

        }else{
            console.log("User Test" + ' \u2717');
            console.log(data.toString().trim());
        }
    });
}

const {Socket} = require('net');
const client = new Socket();

client.connect(22,'127.0.0.1' , () => {
    console.log('Connected');
    UserTest('testing', 'example@gmail.com', 'testing', client);

});