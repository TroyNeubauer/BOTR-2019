/*
var variometer = $.flightIndicator('#variometer', 'variometer', {vario:-5, showBox:false});
var airspeed = $.flightIndicator('#airspeed', 'airspeed', {showBox: false});
var altimeter = $.flightIndicator('#altimeter', 'altimeter', {showBox: false});
var increment = 0;
var alt = 0;
var lastTime = new Date().getTime();
var horizontalVel = 34;
var range = 100;
function dialsRender() {
	var currentTime = new Date().getTime();
	var verticalVel = 52.3 + range * Math.sin(increment/100);
	airspeed.setAirSpeed(Math.abs(Math.sqrt(verticalVel * verticalVel + horizontalVel * horizontalVel)));
	alt += verticalVel * (currentTime - lastTime) / 1000;
	altimeter.setAltitude(alt);
	lastTime = currentTime;
	//altimeter.setPressure(1000+3*Math.sin(increment/50));
	increment++;
	variometer.setVario(verticalVel/100);
	range += (0.4 - Math.random()) / 5;
}
*/

var attitude = $.flightIndicator('#attitude', 'attitude', {roll:50, pitch:-20, size:200, showBox : true, img_directory: "img/"});

var increment = 0;
setInterval(function() {
    attitude.setRoll(30*Math.sin(increment/10));
    attitude.setPitch(50*Math.sin(increment/20));

    increment++;
}, 50);
