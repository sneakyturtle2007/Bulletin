<h1 align="center">Bulletin</h1>

<p align="center">
    <img alt="Static Badge" src="https://img.shields.io/badge/License-GNU%20AGPL%20v3.0%20%2B%20CopyRight-blue?&link=https%3A%2F%2Fgithub.com%2Fsneakyturtle2007%2FBulletin%2Fblob%2Fmain%2FLICENSE">
    <img alt="Static Badge" src="https://img.shields.io/badge/Node.js%C2%AE-v22.17.0-green?logo=nodedotjs&link=https://nodejs.org/en/download">
    <img alt="Static Badge" src="https://img.shields.io/badge/GCC-c23-%23283593?logo=c&link=https://gcc.gnu.org/">
</p>

<h2>Summary</h2>

<p>Bulletin is a calendar app meant to handle the troublesome task of organizing events with other people. It takes the cumbersome task of organizing free time and automates it. So you can focus on creating a schedule instead of planning to make one.</p>
<div>
    <h2>Quick Start</h2>
    <h3>Server (DEPRICATED)-</h3>
    <details> <summary><h4>Linux -</h4></summary>
    <ol>
        <li>Install <a href="https://nodejs.org/en/download">Node.js</a></li>
        <img alt="Image depicting what settings to choose for the nodejs install. The settings are, the newest long term support version available for linux using nvm with npm." src="https://github.com/user-attachments/assets/b556d5dd-a82d-44bb-8171-fe8404588286">
        <br><br>
        <li>Install the required npm packages</li>
        <code>npm install net https express path sqlite3</code>
        <br><br>
        <li>Run</li>
        <code>node ServerMain.js // sudo may be required to setup the server TCP & HTTPS  port. </code>
    </ol>
</div>

<div>
    <details>
        <summary><h4>Windows -</h4></summary>
        <ol>
            <li>Install <a href="https://nodejs.org/en/download">Node.js</a></li>
            <img alt="Image depicting what settings to choose for the nodejs install. The settings are, the newest long term support version available for windows using Chocolatey with npm." src="https://github.com/user-attachments/assets/93823955-df56-4834-94e0-b485a09df550">
            <br><br>
            <li>Install the required npm packages</li>
            <code>npm install net https express path sqlite3</code>
            <br><br>
            <li>Run</li>
            <code>node ServerMain.js // Elevated privileges may be required to setup the server TCP & HTTPS Port. </code>
        </ol>
    </details>
</div>

<h3>Server C-Edition (0.1_alpha) - </h3>
<div>
    <details closed>
        <summary><h4>Linux -</h4></summary>
        <ol>
            <li>Install necessary packages</li>
            <br>
            <p>
                Debian based systems -
                <br>
                <code>sudo apt install gcc cmake make</code>
            </p>
            <p>
                Arch based systems -
                <br>
                <code>sudo pacman -S gcc cmake make</code>
            </p>
            <br>
            <li>Go to the <code>C-remake/</code> directory.</li>
            <br>
            <li>Compile & run</li>
            <br>
            <ul>
                <details closed>
                    <summary>Monolithic -</summary>
                    <ol>
                        <li>Go to the <code>monolithic/</code> directory.</li>
                        <br>
                        <li>Delete everything aside from the <code>database.db</code> file in the <code>bin/</code> directory.</li>
                        <br>
                        <li>Run <code>cmake -S ./ -B ./bin</code> from within the <code>C-remake/monolithic/</code> directory.</li>
                        <br>
                        <li>Go the <code>bin/</code> directory and run <code>make</code>, or <code>make -d</code> for debug.</li>
                        <br>
                        <li>Run <code>./bulletin</code>.</li>
                    </ol>
                </details>
                <details close>
                    <summary>Micro Services (UNIMPLEMENTED) -</summary>
                </details>
            </ul>
        </ol>
    </details>
</div>
<h2>Server(C-Edition) Dependencies </h2>
<div>
    <ol>
        <li><a href="https://github.com/json-c/json-c">Json-c</a></li>
        <li><a href="https://sqlite.org/index.html">Sqlite</a></li>
    </ol>
</div>
<h2>License and Copyright</h2>

© 2025 Luis Blake (sneakyturtle2007), Lai Sengvilay (Laiknight), NoraBorealis10288, Colten Roberts (johnAppDev), and contributors.  
This project is licensed under the GNU Affero General Public License v3.0 or later,  
with additional terms specified in the [LICENSE](./LICENSE) file.

[View the GNU AGPL v3.0 License on ChooseALicense](https://choosealicense.com/licenses/agpl-3.0/)



