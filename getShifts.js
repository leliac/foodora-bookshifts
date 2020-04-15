var system = require('system');
var argsLength = 4;
if(system.args.length !== argsLength) {
        console.log('Usage: phantomjs ' + system.args[0] + ' <base url> <schedule id> <shifts file>');
        phantom.exit();
}
var scheduleUrl = system.args[1] + system.args[2];
var scheduleId = system.args[2];
var shiftsFile = system.args[3];

var fs = require('fs');
var timeout = 5000;

var page = require('webpage').create();
page.settings.userAgent = 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0';
page.onConsoleMessage = function(msg) {
        console.log(msg);
};
page.onCallback = function(shiftData) {
	console.log(scheduleId + ' ' + shiftData);
	fs.write(shiftsFile, scheduleId + ' ' + shiftData + '\n', 'a');
}

page.open(scheduleUrl, function(status) {
	if(status !== 'success') {
		console.log('Failed to load \'' + scheduleUrl + '\'');
		phantom.exit();
	}
	console.log('Loaded \'' + scheduleUrl + '\'');

	window.setTimeout(function() {
		page.evaluate(function() {
			var weekdayClass = 'fc-event-container', shiftClass = 'fc-time-grid-event', shiftLocationClass = 'hidden-xs';
			var weekdays, shifts;
			var shiftLocation, shiftId, shiftTime, shiftData;
			var i, j;

			shiftLocation = document.getElementsByClassName(shiftLocationClass)[1].innerHTML;
			shiftLocation = shiftLocation.replace(/\s/g, '_');
			weekdays = document.getElementsByClassName(weekdayClass);
			console.log('Found ' + weekdays.length + ' weekdays');
			if(weekdays.length !== 7) {
				return;
			}
			for(i = 0; i < weekdays.length; i++) {
				shifts = weekdays[i].getElementsByClassName(shiftClass);
				console.log('Found ' + shifts.length + ' shifts on weekday ' + i);
				for(j = 0; j < shifts.length; j++) {
					shiftId = shifts[j].getAttribute('id');
					shiftTime = shifts[j].getElementsByTagName('span')[0].innerHTML;
					shiftData = shiftLocation + ' ' + i + ' ' + shiftId + ' ' + shiftTime;
					window.callPhantom(shiftData);
				}
			}
		});
		phantom.exit();
	}, timeout);
});
