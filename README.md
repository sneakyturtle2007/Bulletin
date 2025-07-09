# Bulletin
Bulletin is a calendar app meant to handle the troublesome task of organizing events with other people. It takes the cumbersome task of organizing free time and automates it. So you can focus on creating a schedule instead of planning to make one.
## Usage
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

## License and Copyright

© 2025 Luis Blake (sneakyturtle2007), Lai Sengvilay (Laiknight), NoraBorealis10288, Colten Roberts (johnAppDev), and contributors.  
This project is licensed under the GNU Affero General Public License v3.0 or later,  
with additional terms specified in the [LICENSE](./LICENSE) file.

[View the GNU AGPL v3.0 License on ChooseALicense](https://choosealicense.com/licenses/agpl-3.0/)


