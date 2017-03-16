var sc = require('./build/Release/screencapture.node');

//var dir = "C:/work/ws/out/";
var save_dir = "/Users/yyb/out/";
// var icon_dir = "/Users/yyb/out/SealTalk_Ent_Test.app/Contents/Resources/app";
// var icon_dir = "/Users/yyb/work/electron/electron-quick-start";
var app = new sc.Main;

console.log('urls: ' + app.getFilePathFromClipboard());

app.screenCapture(save_dir, function(str){
	console.log(str);
});

