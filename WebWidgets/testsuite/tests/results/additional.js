function printMessage() {
	alert("Clicked on the middle");
}


function printMessage2() {
	alert("I Repeat: You clicked on the middle");
}


function copyToTxt1(ev) {
	$("txt1").value = $(getIDFromEventArgs(ev)).value;
}