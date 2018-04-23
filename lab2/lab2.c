// Aliyah Flores
// aff13

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void read_line(const char* message, char* buffer, int length) {
	printf(message);
	fgets(buffer, length, stdin);
	if(strlen(buffer) != 0)
		buffer[strlen(buffer) - 1] = 0;
}

int streq_nocase(const char* a, const char* b) {
	for(; *a && *b; a++, b++)
		if(tolower(*a) != tolower(*b))
			return 0;

	return *a == 0 && *b == 0;
}

float weight_on_planet(int weight, const char* planet) {
	//returns what you'd weigh on that planet
	// -1 if it's not a planet
	float newWeight;

	if(streq_nocase(planet, "mercury")) {
		newWeight = weight * 0.38;
		return newWeight;
	} else if (streq_nocase(planet, "venus")) {
		newWeight = weight * 0.91;
		return newWeight;
	} else if (streq_nocase(planet, "mars")) {
		newWeight = weight * 0.38;
		return newWeight;
	} else if (streq_nocase(planet, "jupiter")) {
		newWeight = weight * 2.54;
		return newWeight;
	} else if (streq_nocase(planet, "saturn")) {
		newWeight = weight * 1.08;
		return newWeight;
	} else if (streq_nocase(planet, "uranus")) {
		newWeight = weight * 0.91;
		return newWeight;
	} else if (streq_nocase(planet, "neptune")) {
		newWeight = weight * 1.91;
		return newWeight;
	} else {
		return -1;
	}
}

int main() {
	//need to make char array that holds 100
	//need to read a line of input from the user
	//use streq_nocase to see what they typed in

	//in the loop, ask for a planet
	//type break; if they typed exit
	//if they typed earth, call them silly
	//otherwise, call the function you made to get the scaled weight
	//if it returned a value less than 0, then it's not a planet and you say so
	//otherwise, tell them how much they'd weight there (use "%.2f")

	char buffer[100]; // char array that holds 100
	printf("How much do you weigh?\n");
	fgets(buffer, sizeof(buffer), stdin);

	int ogweight;
	sscanf(buffer, "%d", &ogweight);
	//printf("The weight of this person is: %d\n", weight);

	while(1) {
		char planetchoice[100];
		read_line("Which planet would you like to visit? Type exit if you would like to exit the system.\n", planetchoice, sizeof(planetchoice));

		if(streq_nocase(planetchoice, "earth")) {
			printf("You're silly.\n");
		} else if(streq_nocase(planetchoice, "exit")) {
			break;
		} 
		else {
			float weightconvert = weight_on_planet(ogweight, planetchoice);
			if(weightconvert < 0) {
				printf("Uhhhh that's not a planet, my dude.\n");
			}
			else {
				printf("You would weigh %.2f there.\n", weightconvert);
			}
		} 
	}

	return 0;
}