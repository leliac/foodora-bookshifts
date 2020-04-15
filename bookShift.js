var system = require('system');
var argsLength = 2;
if(system.args.length !== argsLength) {
	console.log('Usage: phantomjs ' + system.args[0] + ' <shift url>');
}
var shiftUrl = system.args[1];

var page = require('webpage').create();
page.settings.userAgent = 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0';
page.onConsoleMessage = function(msg) {
        console.log(msg);
};

page.open(shiftUrl, function(status) {
	if(status !== 'success') {
		console.log('Failed to load \'' + shiftUrl + '\'');
		phantom.exit();
	}
	console.log('Loaded \'' + shiftUrl + '\'');

	page.evaluate(function() {
		var divId = 'apply_shift';
		var submitName = 'commit';
		var submit;

		submit = document.getElementById(divId).getElementsByTagName('form')[0].elements[submitName];
		if(typeof submit === 'undefined') {
			console.log('Submit element not found');
			return;
		}
		submit.click();
		console.log('Applied for shift');
	});

	var timeout = 5000;
	setTimeout(function() {
                phantom.exit();
        }, timeout);
});
