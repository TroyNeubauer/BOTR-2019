

var SerialPort = require('serialport');
var serialPort = new SerialPort('COM6', {
    baudRate: 9600
});


serialPort.on('data', function (data) {
    console.log('Data:', data);
});


serialPort.on('readable', function () {
    console.log('Data:', port.read());
});
