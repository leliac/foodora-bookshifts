#!/bin/sh

dir=$(pwd)
mailFile=$dir/*.eml
schedulesFile=$dir/schedules.txt
cookiesFile=$dir/cookies.txt
shiftsFile=$dir/shifts.txt
availabilitiesFile=$dir/availabilities.txt
bookingsFile=$dir/bookings.txt

baseUrl="http://shyftplan.com/companies/11298-it-foodora-torino/shiftplans/"
loginUrl="https://shyftplan.com/login"

schedulesNum=3
timeout=0.5


sleep $timeout

cat $mailFile | grep $baseUrl | sed 's/"//g' | sed 's/=//' | cut -d'/' -f7 >> $schedulesFile
rm $mailFile

if [ $(cat $schedulesFile | wc -l) -eq $schedulesNum ]; then
	phantomjs --cookies-file=$cookiesFile $dir/login.js $loginUrl

	phantomjs --cookies-file=$cookiesFile $dir/getShifts.js $baseUrl $(sed -n '1p' $schedulesFile) $shiftsFile
	phantomjs --cookies-file=$cookiesFile $dir/getShifts.js $baseUrl $(sed -n '2p' $schedulesFile) $shiftsFile
	phantomjs --cookies-file=$cookiesFile $dir/getShifts.js $baseUrl $(sed -n '3p' $schedulesFile) $shiftsFile

	sed -i 's/shift-//' $shiftsFile
	sed -i 's/- //' $shiftsFile
	sed -i 's/://g' $shiftsFile

	$dir/selectShifts $availabilitiesFile $shiftsFile $bookingsFile

	numBookings=$(cat $bookingsFile | wc -l)
	i=1

	while [ $i -le $numBookings ]; do
		phantomjs --cookies-file=$cookiesFile $dir/bookShift.js $baseUrl$(sed -n $i'p' $bookingsFile)
		i=$((i+1))
	done

	> $schedulesFile
	> $cookiesFile
	> $shiftsFile
	> $bookingsFile
fi

exit 0
