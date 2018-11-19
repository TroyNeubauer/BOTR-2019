
var variometer = $.flightIndicator('#variometer', 'variometer', {vario:-5, showBox:false});
var airspeed = $.flightIndicator('#airspeed', 'airspeed', {showBox: false});
var altimeter = $.flightIndicator('#altimeter', 'altimeter', {showBox: false});
var increment = 0;
function dialsRender() {
	airspeed.setAirSpeed(80+80*Math.sin(increment/100));
	altimeter.setAltitude(10*increment);
	//altimeter.setPressure(1000+3*Math.sin(increment/50));
	increment++;
	variometer.setVario(1.75*Math.sin(increment/30));
}
