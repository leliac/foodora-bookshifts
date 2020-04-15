#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ARGC_EXP 4
#define LOCATION_MAX 50

typedef struct {
	int schedule;
	char location[LOCATION_MAX];
	int id;
	short start;
	short end;
	char book;
} shift_t;

typedef struct {
	short start;
	short end;
	char location[LOCATION_MAX];
	short prefStart;
	short prefEnd;
} availability_t;

typedef struct {
	shift_t *data;
	int num;
} shifts_t;

typedef struct {
	availability_t *data;
	short num;
} availabilities_t;

void printShift(shift_t shift);

int main(int argc, char **argv) {
	availabilities_t availabilities[7];
	shifts_t shifts[7];
	short weekday, start, end, prefStart, prefEnd;
	int schedule, id, bestShift;
	short diffStart, diffEnd, diff, minDiff;
	char location[LOCATION_MAX];
	short weekdays[] = {5, 2, 6, 4, 3, 1, 0};
	int i, j;
	char line[LINE_MAX], *cp;
	FILE *f;

	if(argc != ARGC_EXP) {
		printf("Usage: %s <availabilities file> <shifts file> <bookings file>\n", argv[0]);
		return 1;
	}

	// Init availabilities
	for(i = 0; i < 7; i++) {
		availabilities[i].data = NULL;
		availabilities[i].num = 0;
	}

	// Read availabilities
	f = fopen(argv[1], "r");
	while(fgets(line, LINE_MAX, f) != NULL) {
		cp = line;
		sscanf(cp, "%hd", &weekday);
		for(cp += 1+1; sscanf(cp, "%hd %hd %s %hd %hd ", &start, &end, location, &prefStart, &prefEnd) == 5; cp += 16+4+strlen(location)+1) {
			availabilities[weekday].data = realloc(availabilities[weekday].data, (availabilities[weekday].num+1)*sizeof(availability_t));
			availabilities[weekday].data[availabilities[weekday].num].start = start;
			availabilities[weekday].data[availabilities[weekday].num].end = end;
			strcpy(availabilities[weekday].data[availabilities[weekday].num].location, location);
			availabilities[weekday].data[availabilities[weekday].num].prefStart = prefStart;
			availabilities[weekday].data[availabilities[weekday].num].prefEnd = prefEnd;
			availabilities[weekday].num++;
		}
	}
	fclose(f);

	// Print availabilities
	/*
	printf("Availabilities:\n");
	for(weekday = 0; weekday < 7; weekday++) {
		printf("%d\n", weekday);
		for(i = 0; i < availabilities[weekday].num; i++) {
			printf("%hd %hd %s %hd %hd\n",
			       availabilities[weekday].data[i].start,
			       availabilities[weekday].data[i].end,
			       availabilities[weekday].data[i].location,
			       availabilities[weekday].data[i].prefStart,
			       availabilities[weekday].data[i].prefEnd);
		}
	}
	printf("\n");
	*/

	// Init shifts
	for(weekday = 0; weekday < 7; weekday++) {
		shifts[weekday].data = NULL;
		shifts[weekday].num = 0;
	}

	// Read shifts
	f = fopen(argv[2], "r");
	while(fgets(line, LINE_MAX, f) != NULL) {
		sscanf(line, "%d %s %hd %d %hd %hd", &schedule, location, &weekday, &id, &start, &end);
		shifts[weekday].data = realloc(shifts[weekday].data, (shifts[weekday].num+1)*sizeof(shift_t));
		shifts[weekday].data[shifts[weekday].num].schedule = schedule;
		strcpy(shifts[weekday].data[shifts[weekday].num].location, location);
		shifts[weekday].data[shifts[weekday].num].id = id;
		shifts[weekday].data[shifts[weekday].num].start = start;
		shifts[weekday].data[shifts[weekday].num].end = end;
		shifts[weekday].data[shifts[weekday].num].book = 0;
		shifts[weekday].num++;
	}
	fclose(f);

	// Print shifts
	/*
	printf("Shifts:\n");
	for(weekday = 0; weekday < 7; weekday++) {
		printf("%d\n", weekday);
		for(i = 0; i < shifts[weekday].num; i++) {
			printShift(shifts[weekday].data[i]);
		}
	}
	printf("\n");
	*/

	// Select shifts
	for(weekday = 0; weekday < 7; weekday++) {
		for(i = 0; i < availabilities[weekday].num; i++) {
			minDiff = SHRT_MAX;
			for(j = 0; j < shifts[weekday].num; j++) {
				if(shifts[weekday].data[j].start >= availabilities[weekday].data[i].start &&
				   shifts[weekday].data[j].end <= availabilities[weekday].data[i].end &&
				   strcmp(shifts[weekday].data[j].location, availabilities[weekday].data[i].location) == 0) {
					diffStart = abs(shifts[weekday].data[j].start - availabilities[weekday].data[i].prefStart);
					diffEnd = abs(shifts[weekday].data[j].end - availabilities[weekday].data[i].prefEnd);
					diff = diffStart + diffEnd;
					if(diff < minDiff) {
						minDiff = diff;
						bestShift = j;
					}
				}
			}
			if(minDiff != SHRT_MAX) {
				shifts[weekday].data[bestShift].book = 1;
			}
		}
	}

	// Print bookings to file
	f = fopen(argv[3], "w");
	for(i = 0; i < 7; i++) {
		for(j = 0; j < shifts[weekdays[i]].num; j++) {
			if(shifts[weekdays[i]].data[j].book == 1) {
				fprintf(f, "%d/shifts/%d\n", shifts[weekdays[i]].data[j].schedule, shifts[weekdays[i]].data[j].id);
			}
		}
	}

	// Print bookings
	/*
	printf("Bookings:\n");
	for(weekday = 0; weekday < 7; weekday++) {
		printf("%hd\n", weekday);
		for(i = 0; i < shifts[weekday].num; i++) {
			if(shifts[weekday].data[i].book == 1) {
				printShift(shifts[weekday].data[i]);
			}
		}
	}
	*/

	// Free memory
	for(weekday = 0; weekday < 7; weekday++) {
		free(availabilities[weekday].data);
		free(shifts[weekday].data);
	}

	return 0;
}

void printShift(shift_t shift) {
	printf("%d %s %d %d %d %d\n",
	       shift.schedule,
	       shift.location,
	       shift.id,
	       shift.start,
	       shift.end,
	       shift.book);
	return;
}
