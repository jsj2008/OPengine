var OP = require('./node_modules/OPengine/OPengine');

// function Initialize() {
// 	print('SCRIPT INITIALIZED');	
// }

// function Update(elapsed) {
	
// 	OP.input.Update();

// 	if(OP.input.KeyIsDown(OP.KEYS.B)) return 1;

var amnt = 0;
var dir = 1;

function clearToBlack(r, g, b) {
    //print('rgb:' + r + ' ' + g + ' ' + b);
	amnt += dir ? 0.01 : -0.01;
	if(amnt > 1.0 || amnt < 0.0) {
		dir = !dir;
	}
	OP.render.Clear(Math.sin(amnt),Math.cos(amnt),1);	

	//print('.');
		
	OP.render.Present();

	return { test: 'Test' };
}

function run(arg) {
    //print(arg.test);
}
// 	return 0;
// }

// function Destroy() {

// }

// OP.Start(Initialize, Update, Destroy);