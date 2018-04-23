//Aliyah Flores
//aff13

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#pragma pack(1)


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


int random_range(int low_value, int high_value) { //function to generate a random number for the CPU to use
	int num = rand() % (high_value - low_value + 1) + low_value;
	return num;
}

char* cpu() { //generates the computer's turn
	int number = random_range(1, 3);

	if(number == 1) {
		printf("The computer chooses rock. ");
		return "rock";
	}
	if(number == 2) {
		printf("The computer choose paper. ");
		return "paper";
	}
	if(number == 3) {
		printf("The computer chooses scissors. ");
		return "scissors";
	} else {
		return "error";
	}
} 


int main() {
	srand((unsigned int)time(NULL));
	int player_wins = 0;
	int cpu_wins = 0;
	char choice[50];
	int player_points;
	int cpu_points;

	while(player_wins != 1 && cpu_wins != 1) {
		player_points = 0;
		cpu_points = 0;

		while(player_points != 3 && cpu_wins != 3) {

			if(player_points == 3 || cpu_points == 3)
				break;

			char user_choice[50];
			char* cpu_choice;

			read_line("What is your choice? ", user_choice, sizeof(user_choice));
			cpu_choice = cpu();

			if(streq_nocase(user_choice, "rock") && !strcmp(cpu_choice,"scissors")) {
				player_points++;
				printf("You win!");
			}
			else if(streq_nocase(user_choice, "rock") && !strcmp(cpu_choice,"paper")) {
				cpu_points++;
				printf("You lose!");
			}
			else if(streq_nocase(user_choice, "rock") && !strcmp(cpu_choice,"rock")) {
				printf("It's a tie!");
			}
			else if(streq_nocase(user_choice, "scissors") && !strcmp(cpu_choice,"paper")) {
				player_points++;
				printf("You win!");
			}
			else if(streq_nocase(user_choice, "scissors") && !strcmp(cpu_choice,"rock")) {
				cpu_points++;
				printf("You lose!");
			}
			else if(streq_nocase(user_choice, "scissors") && !strcmp(cpu_choice,"scissors")) {
				printf("It's a tie!");	
			}
			else if(streq_nocase(user_choice, "paper") && !strcmp(cpu_choice,"rock")) {
				player_points++;
				printf("You win!");
			}
			else if(streq_nocase(user_choice, "paper") && !strcmp(cpu_choice,"scissors")) {
				cpu_points++;
				printf("You lose!");
			}
			else if(streq_nocase(user_choice, "paper") && !strcmp(cpu_choice,"paper")) {
				printf("It's a tie!");
			}
			else {
				printf("Huh? What's your choice? ");
			}
			printf(" You: %d Computer: %d ", player_points, cpu_points);
			printf("\n");
		}

		if(player_points > cpu_points) {
			printf("\nYou won the game! ");
			player_wins++;
		}
		if(player_points < cpu_points) {
			printf("\nYou lost the game! ");
			cpu_wins++;
		}

		printf("\nWould you like to play again? (Type y for yes, and n for no): " );
		scanf("%s", choice);

		if(streq_nocase(choice, "y")) {
			player_points = 0;
			cpu_points = 0;
			player_wins = 0;
			cpu_wins = 0;
		} else if (streq_nocase(choice, "n")) {
			break;
		}
	}
	return 0;
}