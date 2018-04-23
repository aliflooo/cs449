//Aliyah Flores
//aff13

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){

  char string[100];
  int length = 0;
  char letter;
  FILE* file;

  file = fopen(argv[1], "r");

  if (file == NULL){
    printf("The file does not exist");
    return 0;
  }

  while(feof(file) == 0) {
    fread(&letter, sizeof(letter), 1, file);

    if (letter > 32 && letter < 126){
      string[length] = letter;
      length++;
      continue;
    }

    string[length] = '\0';

    if (4 <= length)
      printf("%s\n", string);

    length = 0;
    string[0] = '\0';

  }

  fclose(file);
  return 0;
}