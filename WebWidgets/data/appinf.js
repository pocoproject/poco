function $(id) {
	return document.getElementById(id);
}

function getIDFromEventArgs(ev) {
	var elem = ev.target || ev.srcElement;
	return elem.id;
}

var BrowserDetect = {
	init: function () {
		this.browser = this.searchString(this.dataBrowser) || "An unknown browser";
		this.version = this.searchVersion(navigator.userAgent)
			|| this.searchVersion(navigator.appVersion)
			|| "an unknown version";
		this.OS = this.searchString(this.dataOS) || "an unknown OS";
	},
	searchString: function (data) {
		for (var i=0;i<data.length;i++)	{
			var dataString = data[i].string;
			var dataProp = data[i].prop;
			this.versionSearchString = data[i].versionSearch || data[i].identity;
			if (dataString) {
				if (dataString.indexOf(data[i].subString) != -1)
					return data[i].identity;
			}
			else if (dataProp)
				return data[i].identity;
		}
	},
	searchVersion: function (dataString) {
		var index = dataString.indexOf(this.versionSearchString);
		if (index == -1) return;
		return parseFloat(dataString.substring(index+this.versionSearchString.length+1));
	},
	dataBrowser: [
		{ 	string: navigator.userAgent,
			subString: "OmniWeb",
			versionSearch: "OmniWeb/",
			identity: "OmniWeb"
		},
		{
			string: navigator.vendor,
			subString: "Apple",
			identity: "Safari"
		},
		{
			prop: window.opera,
			identity: "Opera"
		},
		{
			string: navigator.vendor,
			subString: "iCab",
			identity: "iCab"
		},
		{
			string: navigator.vendor,
			subString: "KDE",
			identity: "Konqueror"
		},
		{
			string: navigator.userAgent,
			subString: "Firefox",
			identity: "Firefox"
		},
		{
			string: navigator.vendor,
			subString: "Camino",
			identity: "Camino"
		},
		{		// for newer Netscapes (6+)
			string: navigator.userAgent,
			subString: "Netscape",
			identity: "Netscape"
		},
		{
			string: navigator.userAgent,
			subString: "MSIE",
			identity: "Explorer",
			versionSearch: "MSIE"
		},
		{
			string: navigator.userAgent,
			subString: "Gecko",
			identity: "Mozilla",
			versionSearch: "rv"
		},
		{ 		// for older Netscapes (4-)
			string: navigator.userAgent,
			subString: "Mozilla",
			identity: "Netscape",
			versionSearch: "Mozilla"
		}
	],
	dataOS : [
		{
			string: navigator.platform,
			subString: "Win",
			identity: "Windows"
		},
		{
			string: navigator.platform,
			subString: "Mac",
			identity: "Mac"
		},
		{
			string: navigator.platform,
			subString: "Linux",
			identity: "Linux"
		}
	]

};
BrowserDetect.init();


function createXMLHttpRequest() {
	try { return new ActiveXObject("Msxml2.XMLHTTP"); } catch(ex) {}
	try { return new ActiveXObject("Microsoft.XMLHTTP"); } catch(ex) {}
	try { return new XMLHttpRequest(); } catch(ex) {}
	alert ("XMLHttpRequest not supported");
	return null;
}

// This function returns an integer telling which buttons were pressed (IEStyle):
// 1 ... left
// 2 ... right
// 4 ... middle
// Values can be summed up to identify multiple buttons were pressed
function buttonsPressed(ev)
{
	var button = 0;

	if (typeof(ev.which) == "number")
	{
		button = ev.which;
		// Netscape: left = 1, middle = 2, right = 3, 
		if (button == 2) 
		{
			button = 4;
		}
		else if (button == 3) 
		{
			button = 2;
		}
		
	}
	else if (typeof(ev.button) == "number")
	{
		button = ev.button;
		if (BrowserDetect.browser == "Opera" && BrowserDetect.version < "8") {
			// can't detect if two buttons were pressed
			if (button == 3) {
				button = 4;
			}
			else if (button > 3) {
				button = button + 1;
			}
		}
	}
	if (button == 0)
	{
		// assume left click
		button = 1;
	}
	return button;
}

// returns an object containing members
// scrollLeft, scrollTop: the position inside the Document
// left, top,: position inside the Window  
function mousePos(evt)
{
    if(!evt) evt = window.event;
    var pos = new Object();
    pos.left = evt.clientX;
    pos.top = evt.clientY;
    var b = (window.document.compatMode && window.document.compatMode == "CSS1Compat") ? 
   window.document.documentElement : window.document.body || null;
    if (b)
    {
        pos.scrollLeft= pos.left + b.scrollLeft;
        pos.scrollTop = pos.top + b.scrollTop;
    }
    else if(document.layers)
    {
        // Netscape 4.
        pos.scrollLeft = evt.pageX;
        pos.scrollTop = evt.pageY;
        pos.left = evt.pageX - window.pageXOffset;
        pos.top = evt.pageY - window.pageYOffset;
    }
    return pos;
}


function getSelectedText() {
	if (window.getSelection) {
		return window.getSelection();
	}
	else if (document.getSelection) {
		return document.getSelection();
	}
	else if (document.selection) {
		return document.selection.createRange().text;
	}
	return "";
}


// expects no return data from the server
function eventCallBack(ev, callbackAddress) {
	var req = createXMLHttpRequest();
	req.open("GET", callbackAddress+"?id="+getIDFromEventArgs(ev)+"&type="+ev.type, false);
	req.send(null);
}


function mouseEventCallBack(ev, callbackAddress) {
	var req = createXMLHttpRequest();
	var pos = mousePos(ev);
	req.open("GET", callbackAddress+"?id="+getIDFromEventArgs(ev)+"&type="+ev.type+"&buttons="+buttonsPressed(ev)+"&posX="+pos.scrollLeft+"&posY="+pos.scrollTop, false); 
	req.send(null);
}


function mouseMoveEventCallBack(ev, callbackAddress) {
	var req = createXMLHttpRequest();
	var pos = mousePos(ev);
	req.open("GET", callbackAddress+"?id="+getIDFromEventArgs(ev)+"&type="+ev.type+"&posX="+pos.scrollLeft+"&posY="+pos.scrollTop+"&windowPosX="+pos.left+"&windowPosY="+pos.top, false); 
	req.send(null);
}

function keyboardEventCallBack(ev, callbackAddress) {
	var req = createXMLHttpRequest();
	var key = ev.which;
	if (!key) key = ev.keyCode;
		
	req.open("GET", callbackAddress+"?id="+getIDFromEventArgs(ev)+"&type="+ev.type+"&key="+key+"&alt="+ev.altKey+"&shift="+ev.shiftKey+"&ctrl="+ev.ctrlKey, false); 
	req.send(null);
}
	
