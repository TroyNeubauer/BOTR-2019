var scene = new THREE.Scene();
//var camera = new THREE.PerspectiveCamera( 75, 300/300, 0.1, 200 );
var camera = new THREE.OrthographicCamera(-10, 10, 10, -10, -15, 100);
var renderer = new THREE.WebGLRenderer();
renderer.setClearColor(0xffffff, 1);
renderer.setSize(300, 300);
document.body.appendChild(renderer.domElement);

const globe = new THREE.Group();
var geometry = new THREE.BoxGeometry(10, 10, 10, 8, 8, 8);
for (var i = 0; i < geometry.vertices.length; i++) {
    geometry.vertices[i].normalize().multiplyScalar(10);
}

var repeat = new THREE.Vector2(1/3, 1/2);
var offsets = [
    new THREE.Vector2(0, 0),
    new THREE.Vector2(0, 1/2),
    new THREE.Vector2(1/3, 0),
    new THREE.Vector2(1/3, 1/2),
    new THREE.Vector2(2/3, 0),
    new THREE.Vector2(2/3, 1/2)
];

// redefine vertex normals consistent with a sphere; reset UVs
for (var i = 0; i < geometry.faces.length; i++) {
	var face = geometry.faces[i];
	face.vertexNormals[0].copy(geometry.vertices[face.a]).normalize();
	face.vertexNormals[1].copy(geometry.vertices[face.b]).normalize();
	face.vertexNormals[2].copy(geometry.vertices[face.c]).normalize();

	var uvs = geometry.faceVertexUvs[0];
	for ( var j = 0; j < 3; j ++ ) {
	  uvs[i][j].multiply(repeat).add(offsets[face.materialIndex]);
	}
}


let path = 'textures/';
let format = '.png';
let urls = [
	path + 'px' + format, path + 'nx' + format,
	path + 'py' + format, path + 'ny' + format,
	path + 'pz' + format, path + 'nz' + format
];
var loader = new THREE.CubeTextureLoader();
let textureCube = loader.load(urls, THREE.CubeRefractionMapping);
let shader = THREE.ShaderLib.cube;
shader.uniforms.tCube.value = textureCube;

let material = new THREE.ShaderMaterial({
	fragmentShader: shader.fragmentShader,
	vertexShader: shader.vertexShader,
	uniforms: shader.uniforms,
	depthWrite: true,
	side: THREE.BackSide
});


var ccccc = new THREE.Mesh(geometry, material);
globe.add(ccccc);
scene.add(globe);

camera.position.z = 17;

function sleep(milliseconds) {
	var start = new Date().getTime();
	for (var i = 0; i < 1e7; i++) {
	  if ((new Date().getTime() - start) > milliseconds){
			break;
		}
	}
}

function navballRender() {
	//globe.rotation.x += 0.01;
	//globe.rotation.y += 0.01;
	rawDataInfo.forEach(function(value, key, map) {
		if(Math.random() < 0.12) {
			lastValue = getDataValue(key);
			newV = Math.random() < 10 ? -Math.random() * 5 : + Math.random() * 10;
			if(lastValue != null) newV += lastValue;
			putData(key, newV);
		}
	});

	renderer.render(scene, camera);
};
