/**
 * Init some variables for demo purposes
 */
var second = 0;

/**
 * Function that generates random data
 */
function generateChartData() {
  var chartData = [];
  return chartData;
}

/**
 * Create the chart
 */
var chart = AmCharts.makeChart( "chartdiv", {
  "type": "serial",
  "theme": "light",
  "zoomOutButton": {
    "backgroundColor": '#000000',
    "backgroundAlpha": 0.15
  },
  "dataProvider": generateChartData(),
  "categoryField": "category",
  "categoryAxis": {
    "dashLength": 1
    "gridAlpha": 0.15,
    "axisColor": "#DADADA"
  },
  "graphs": [ {
    "id": "g1",
    "valueField": "visits",
    "bullet": "round",
    "bulletBorderColor": "#FFFFFF",
    "bulletBorderThickness": 2,
    "lineThickness": 2,
    "lineColor": "#b5030d",
    "negativeLineColor": "#0352b5",
    "hideBulletsCount": 50
  } ],
  "chartCursor": {
    "cursorPosition": "mouse"
  },
  "chartScrollbar": {
    "graph": "g1",
    "scrollbarHeight": 40,
    "color": "#FFFFFF",
    "autoGridCount": true
  }
});

/**
 * Set interval to push new data points periodically
 */
// set up the chart to update every second
setInterval( function() {
  // normally you would load new datapoints here,
  // but we will just generate some random values
  // and remove the value from the beginning so that
  // we get nice sliding graph feeling

  // remove datapoint from the beginning
  //chart.dataProvider.shift();

  // add new one at the end
  second++;

  var visits = Math.round( Math.random() * 40 ) - 20;
  chart.dataProvider.push( {
    date: second,
    visits: visits
  } );
  chart.validateData();
}, 1000 );
