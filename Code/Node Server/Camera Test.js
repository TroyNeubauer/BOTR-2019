var fs = require("fs");
var SerialPort = require('serialport');
var Buffer = require('buffer').Buffer;
const rimraf = require('rimraf');

var readline = require('readline');

var rl = readline.createInterface({
	input: process.stdin,
	output: process.stdout
});

var payloadBuffer = Buffer.alloc(50 * 1024);//50KB
var payloadIndex = 0;
var stream = fs.createWriteStream("./Camera Test.dat");
var stream = fs.createWriteStream("./Camera Test.dat");
var bytes = 0;
var packetCount = 0;

try {
	fs.mkdirSync("./images/");
} catch(error) {
	//console.error("Failed to create packets directory! " + error);
}
rimraf('./images/*', function () {  });

var openSerialFunc;
var lastByte = -1;
function openSerialFunction() {
	console.log("trying to open serial port");
	serialPort = new SerialPort("COM12", {
		baudRate: 115200
	});
	serialPort.on('error', (err) => console.log("Serial port error: " + err) );
	serialPort.on('open', () => {
		console.log("Serial port is now open!");
		clearInterval(openSerialFunc);
	});

	serialPort.on("close", () => {
		console.log("Serial port closed");
		stream.end();
		setTimeout(() => process.exit(), 1000);
	});

	serialPort.on('data', function (data) {
		stream.write(data);
		bytes += data.length;
		/*for(var i = 0; i < data.length; i++) {//Process each byte
			var byte = data.readUInt8(i);
			if(lastByte != -1) {
				handleByte(lastByte, byte);
			}
			lastByte = byte;
		}*/
	});
}

var inFrame = false;
function handleByte(last, byte) {
	if(last == 0xFF && byte == 0xD8) {
		inFrame = true;
		payloadIndex = 0;
		payloadBuffer.writeUInt8(last, payloadIndex++);
		payloadBuffer.writeUInt8(byte, payloadIndex++);
	} else if(last == 0xFF && byte == 0xD9) {
		inFrame = false;
		payloadBuffer.writeUInt8(last, payloadIndex++);
		payloadBuffer.writeUInt8(byte, payloadIndex++);
		var toSend = new Uint8Array(payloadIndex);
		for(var i = 0; i < payloadIndex; i++) {
			toSend[i] = payloadBuffer[i];
		}
		fs.writeFile("./images/img" + (packetCount++) + ".jpg", toSend, function(err) {
			if(err) {
				console.error("Failed to write packet file! " + err);
			}
		});
	} else {
		if(inFrame) {
			payloadBuffer.writeUInt8(byte, payloadIndex++);
		} else {
			if(byte == 10) {
				console.log();
			} else {
				process.stdout.write(String.fromCharCode(byte));
			}
		}
	}
}

openSerialFunc = setInterval(openSerialFunction, 100);

rl.question("", function(answer) {
	serialPort.close();
});
