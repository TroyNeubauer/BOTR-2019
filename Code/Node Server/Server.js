

var http = require("http"),
    url = require("url"),
    path = require("path"),
    fs = require("fs")
    port = 8192;

server = http.createServer(function(request, response) {

  var uri = url.parse(request.url).pathname, filename = path.join("C:/Users/Troy Neubauer/Documents/Rocketry/Code/Website", uri);

  var contentTypesByExtension = {
    '.html': "text/html",
    '.css':  "text/css",
    '.js':   "text/javascript"
  };

  fs.exists(filename, function(exists) {
    if(!exists) {
      response.writeHead(404, {"Content-Type": "text/plain"});
      response.write("404 Not Found\n");
      response.end();
      return;
    }

    if (fs.statSync(filename).isDirectory()) filename += '/index.html';

    fs.readFile(filename, "binary", function(err, file) {
      if(err) {
        response.writeHead(500, {"Content-Type": "text/plain"});
        response.write(err + "\n");
        response.end();
        return;
      }

      var headers = {};
      var contentType = contentTypesByExtension[path.extname(filename)];
      if (contentType) headers["Content-Type"] = contentType;
      response.writeHead(200, headers);
      response.write(file, "binary");
      response.end();
    });
  });
});
server.listen(port);


var SerialPort = require('serialport');
var serialPort = new SerialPort('COM6', {
    baudRate: 9600
});


/*serialPort.on('data', function (data) {
    console.log('Data:', data);
});*/


serialPort.on('readable', function () {
    console.log('Readable:', serialPort.read());
});


var readline = require('readline');

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});





rl.question("Press enter to stop the server ", function(answer) {
  serialPort.close();
  server.close();
  rl.close();
});
