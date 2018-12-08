# Rocketry
This repository contains all of the code, resources, and models that I am responsible for the 2018 Battle of the Rockets Sounding Rocket competition. I look forward to team 1800 hopefully scoring all of the software points in this year’s competition. 

## Organization
This project is composed of many directories and sub directories to keep all of the different work I do neat and easy to find. 
- The root directory contains the CAD files for the payload. Below that the rest of the project is separated into two folders. Code which contains all of the code for this project, and Output which contains final .stl files that can be used for 3D printing. Within Code:
- Arduino houses the program that will eventually run on the rocket
- Node Server contains program for hosting the HTTP server and reading/writing between the website and the serialport connected to the radio.
- Website contains the HTML5 groundstation.
All other directories are used as temporary test projects unless otherwise specified. 

## Downloading
1. Download the latest source code
2. Unzip the zip file anywhere you wish
3. Download [node](https://nodejs.org/en/download/) (make sure the version you download matches your OS and platform)
4. Install Rickshaw globally with npm using this command: `npm install -g Rickshaw` (you must specify the -g option otherwise the paths won’t be correct)

Now you should be able to run the `Run Server.bat` file in the Node Server directory or execute `node Server.js` in command prompt. After that you should be able to go to `localhost:8192` in any web browser and see Mission Control!

Feel free to add new issues to the [Issue Tracker](https://github.com/TroyNeubauer/Rocketry/issues) if any of the above information doesn’t work for you. 
