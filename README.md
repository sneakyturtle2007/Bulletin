<h1 align="center">Bulletin</h1>

<p align="center">
    <a href="https://github.com/sneakyturtle2007/Bulletin/blob/main/LICENSE"><img alt="Static Badge" src="https://img.shields.io/badge/License-GNU%20AGPL%20v3.0%20%2B%20CopyRight-blue/"></a>
    <a href="https://nodejs.org/en/download"><img alt="Static Badge" src="https://img.shields.io/badge/Node.js%C2%AE-v22.17.0-green?logo=nodedotjs"></a>
    <a href="https://gcc.gnu.org/"><img alt="Static Badge" src="https://img.shields.io/badge/GCC-v12.2.0-%23283593"></a>
    <img alt="Static Badge" src="https://img.shields.io/badge/C-std:c23-%23283593?logo=c">
</p>

## Summary 

Bulletin is a calendar app meant to handle the troublesome task of organizing events with other people. It takes the cumbersome task of organizing free time and automates it. So you can focus on creating a schedule instead of planning to make one.

## Quick Start
### Server -
#### Linux - 
    
1. Install [Node.js](https://nodejs.org/en/download)

![Image depicting what settings to choose for the nodejs install. The settings are, the newest long term support version available for linux using nvm with npm.](https://github.com/user-attachments/assets/b556d5dd-a82d-44bb-8171-fe8404588286)

2. Install the required npm packages

```bash
npm install net https express path sqlite3
```

3. Run

```bash
node ServerMain.js // sudo may be required to setup the server TCP & HTTPS port. 
```

#### Windows -

1. Install [Node.js](https://nodejs.org/en/download)

![Image depicting what settings to choose for the nodejs install. The settings are, the newest long term support version available for windows using Chocolatey with npm.](https://github.com/user-attachments/assets/93823955-df56-4834-94e0-b485a09df550)

2. Install required npm packages

```bash
npm install net https express path sqlite3
```

3. Run

```bash
node ServerMain.js // Elevated privileges may be required to setup the server TCP & HTTPS Port.
```

### Server C-Edition (Work in progress) - 

#### Linux -

1. Install necessary packages
    
    Debian based systems - 
    ```bash
    sudo apt install gcc cmake make
    ```

    Arch based systems - 
    ```bash 
    sudo pacman -s gcc cmake make
    ```
2. Move the C-remake folder in the Server directory to its own folder


3. Compile & run

    a. Delete everything aside from the ```database.db``` file in the build directory.
    
    b. Run ```cmake -B ./build``` from the base C-remake directory.

    c. Go the build directory and run ```make```, or ```make -d``` for debug.
    
    d. Run ```./my_program```.

## License and Copyright

© 2025 Luis Blake (sneakyturtle2007), Lai Sengvilay (Laiknight), NoraBorealis10288, Colten Roberts (johnAppDev), and contributors.  
This project is licensed under the GNU Affero General Public License v3.0 or later,  
with additional terms specified in the [LICENSE](./LICENSE) file.

[View the GNU AGPL v3.0 License on ChooseALicense](https://choosealicense.com/licenses/agpl-3.0/)



