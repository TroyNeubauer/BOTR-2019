

function createFromHeaderFile(fileName, structName) {
	return new Promise(function (resolve, reject) {
	    const xhr = new XMLHttpRequest();
	    xhr.timeout = 2000;
	    xhr.onreadystatechange = function(e) {
	      if (xhr.readyState === 4) {
	        if (xhr.status === 200) {
	          resolve(xhr.response);
	        } else {
	          reject(xhr.status);
	        }
	      }
	    }
	    xhr.ontimeout = function () {
	      reject("timeout");
	    }
	    xhr.open('GET', fileName, true)
	    xhr.send();
	  })
}
