var map = new ol.Map({
  target: 'map',
  layers: [
	new ol.layer.Tile({
	  source: new ol.source.OSM()
	})
  ],
  view: new ol.View({
	center: ol.proj.fromLonLat([-77.282246, 38.877830]),
	zoom: 16
  })
});
