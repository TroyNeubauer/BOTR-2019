
var variometer = $.flightIndicator('#variometer', 'variometer', {vario:-5, showBox:false});
var airspeed = $.flightIndicator('#airspeed', 'airspeed', {showBox: false});
var altimeter = $.flightIndicator('#altimeter', 'altimeter', {showBox: false});
var increment = 0;
var alt = 0;
var lastTime = new Date().getTime();
function dialsRender() {
	var currentTime = new Date().getTime();
	var speed = 80+80*Math.sin(increment/100);
	airspeed.setAirSpeed(speed);
	alt += speed * (currentTime - lastTime) / 1000;
	altimeter.setAltitude(alt);
	lastTime = currentTime;
	//altimeter.setPressure(1000+3*Math.sin(increment/50));
	increment++;
	variometer.setVario(1.75*Math.sin(increment/30));
}
