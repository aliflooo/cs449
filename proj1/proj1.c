#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#pragma pack(1)
srand((unsigned int)time(NULL));

/* Steps to Implement
1. Have the player enter their choice (as a word)
	Use read_line function; 
	Use streq_nocase
	User must only enter "rock", "paper", or "scissors"
2. Have the computer randomly pick its choice
3. If the two match and it's a tie, THE ROUND DOES NOT COUNT
4. Otherwise, give the winner a point
5. When one player reaches 3 points, they win and the game is over
6. Ask the user if they want to play again
	If yeah, go back to the beginning */

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


int random_int() { //function to generate a random number for the CPU to use
	int num = rand() % (high_value - low_value + 1) + low_value;
	return num;
}

void cpu() { //generates the computer's turn
	int comp_number = random_int();

	if(comp_number == 1)
		printf("The computer chooses rock. ");
	if(comp_number == 2)
		printf("The computer choose paper. ");
	if(comp_number == 3)
		printf("The computer chooses scissors. ");
}

//ask for user's choice
	//only accept rock, paper, scissors
	//if they enter anything else, don't exit, ask them again
//if someone wins, ask if they want to play again
	//y for yes and n for no
	//if yes, go back to 1, otherwise break
int player() {
	char choice[100];
	int number = 0;
	
	if(streq_nocase(choice, "rock") == 0) 
		return 1;
	if(streq_nocase(choice, "paper") == 0) 
		return 2;
	if(streq_nocase(choice, "scissors") == 0) 
		return 3;
	else 
		printf("Huh? What's your choice? ");
}

void game(char* guess) {
	int player_num = player(guess);
	int cpu_num = cpu();

 	printf("Computer chooses : %s\n", cpu_num);

 	if(player_num == cpu_num) {

 	} else if((comp_number - player_number) % 5 < 3) {
 		printf("Compute rwins");
 	} else {
 		printf("Player wins");
 	}
}

void main() {
	char* player_choice;

	printf("What is your choice? ");
	scanf("%s", player_choice);
	game(player_choice);	
		
}