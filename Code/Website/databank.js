var dataBank = new Map();//maps names to number values
var rawDataInfo = new Map();//maps names to the properties that that name has

function add(longName, description, shortName, units) {
	rawDataInfo.set(longName, {description: description, shortName: shortName, units: units});
	putData(longName, NaN);
}

$(document).ready(function() {
	add("Mission Time", "Time since launch", "TIME T:", "s");
	add("Apogee Height", "The rocket's projected apogee based on its velocity", "TIME T:", "feet");
	add("Time to Perigee", "The time until the rocket's projected perigee", "TIME T:", "s");
	add("Semi Major Axis", "Desc...", "TIME T:", "miles");
	add("Eccentricity", "Desc...", "TIME T:", "");
	add("Inclination", "Desc...", "TIME T:", "degrees");
	add("Orbital Period", "Desc...", "TIME T:", "s");
	add("Total Velocity", "Desc...", "TIME T:", "feet/second");
	add("Terrain Elevation", "Desc...", "TIME T:", "feet");
	add("Latitude", "Desc...", "TIME T:", "degrees");
	add("Longitude", "Desc...", "TIME T:", "degrees");
	add("Heading", "Desc...", "TIME T:", "degrees");
	add("Vertical Speed", "Desc...", "TIME T:", "feet/second");
	add("Horizontal Speed", "Desc...", "TIME T:", "feet/second");
	add("Atmosphere Density", "Desc...", "TIME T:", "IDK");
	add("G's of Acceleration", "Desc...", "TIME T:", "g");


});

function putData(name, newValue) {
	date = new Date();
	dataBank.set(name, {value: newValue, time: date});
}

function getDataBankValue(name) {



}
