var system = require('system');
var argsLength = 2;

if(system.args.length !== argsLength) {
	console.log('Usage: phantomjs ' + system.args[0] + ' <url>');
	phantom.exit();
}

var page = require('webpage').create();
page.settings.userAgent = 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:51.0) Gecko/20100101 Firefox/51.0';
page.onConsoleMessage = function(msg) {
	console.log(msg);
};
var url = system.args[1];

page.open(url, function(status) {
	if(status !== 'success') {
		console.log('Failed to load \'' + url + '\'');
		phantom.exit();
	}
	console.log('Loaded \'' + url + '\'');

	page.evaluate(function() {
		var formClass = 'js-session-form';
		var usernameInputName = 'user[email]';
		var passwordInputName = 'user[password]';
		var rememberMeInputName = 'user[remember_me]';
		var submitInputName = 'commit';
		var username = 'username';
		var password = 'password';

		var form = document.getElementsByClassName(formClass);
		console.log('Found ' + form.length + ' form(s) of class \'' + formClass + '\'');
		if(form.length !== 1) {
			phantom.exit();
		}

		var usernameInput = form[0].elements[usernameInputName];
		if(typeof usernameInput === 'undefined') {
			console.log('Input element with name \'' + usernameInputName + '\' not found');
			phantom.exit();
		}
		console.log('Found input element with name \'' + usernameInputName + '\'');

		var passwordInput = form[0].elements[passwordInputName];
		if(typeof passwordInput === 'undefined') {
			console.log('Input element with name \'' + passwordInputName + '\' not found');
			phantom.exit();
		}
		console.log('Found input element with name \'' + passwordInputName + '\'');

		var rememberMeInput = form[0].elements[rememberMeInputName];
		if(typeof rememberMeInput === 'undefined') {
			console.log('Input element with name \'' + rememberMeInputName + '\' not found');
			phantom.exit();
		}
		console.log('Found input element with name \'' + rememberMeInputName + '\'');

		var submitInput = form[0].elements[submitInputName];
		if(typeof submitInput === 'undefined') {
			console.log('Input element with name \'' + submitInputName + '\' not found');
			phantom.exit();
		}
		console.log('Found input element with name \'' + submitInputName + '\'');

		usernameInput.value = username;
		console.log('Set username to \'' + usernameInput.value + '\'');
		passwordInput.value = password;
		console.log('Set password to \'' + passwordInput.value + '\'');
		rememberMeInput.checked = true;
		console.log('Checked remember me');
		submitInput.click();
		console.log('Attempted to log in');
	});

	var timeout = 5000;
	setTimeout(function() {
		phantom.exit();
	}, timeout);
});

