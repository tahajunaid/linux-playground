#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_LENGTH 1024

int main(int argc, char * argv[]) {
  FILE * input;
  FILE * output;
  int show_duplicates = 0;

  if (argc == 1) {
    printf("Enter Text:");
    input = stdin;
    output = stdout;
  }
  if (argc == 2) {
    if (strcmp(argv[1], "-d") == 0) {
      show_duplicates = 1;
      printf("Enter Text:");
      input = stdin;
      output = stdout;
    } else {
      input = fopen(argv[1], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = stdout;
    }
  }
  if (argc == 3) {
    if (strcmp(argv[1], "-d") == 0) {
      show_duplicates = 1;
      input = fopen(argv[2], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = stdout;
    } else if (strcmp(argv[2], "-d") == 0) {
      show_duplicates = 1;
      input = fopen(argv[1], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = stdout;
    } else {
      input = fopen(argv[1], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = fopen(argv[2], "w");
      if (output == NULL) {
        perror("Error opening file");
        return 1;
      }
    }
  }

  if (argc == 4) {
    if (strcmp(argv[1], "-d") == 0) {
      show_duplicates = 1;
      input = fopen(argv[2], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = fopen(argv[3], "w");
      if (output == NULL) {
        perror("Error opening file");
        return 1;
      }
    } else if (strcmp(argv[2], "-d") == 0) {
      show_duplicates = 1;
      input = fopen(argv[1], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = fopen(argv[3], "w");
      if (output == NULL) {
        perror("Error opening file");
        return 1;
      }
    } else {
      show_duplicates = 1;
      input = fopen(argv[1], "r");
      if (input == NULL) {
        perror("Error opening file");
        return 1;
      }
      output = fopen(argv[2], "w");
      if (output == NULL) {
        perror("Error opening file");
        return 1;
      }
    }
  }

  // Read and process the input file line by line
  char current_line[LINE_LENGTH];
  char previous_line[LINE_LENGTH];
  if(fgets(current_line, LINE_LENGTH, input) == NULL){
	  return 1;
  }
  while (fgets(current_line, LINE_LENGTH, input) != NULL) {
    // If the -d flag is provided, only print lines that are duplicates
    if (show_duplicates) {
      if (strcmp(current_line, previous_line) == 0) {
        //printf("%s", current_line);
		fputs(current_line, output);
      }
    } else {
      // Otherwise, print all unique lines
      if (strcmp(current_line, previous_line) != 0) {
        //printf("%s", current_line);
		fputs(current_line, output);
      }
    }
	if (show_duplicates) {
		if(strcmp(current_line, previous_line) != 0)
			strcpy(previous_line, current_line);
	}else{
		strcpy(previous_line, current_line);
	}
	
  }

  // Close the input file if it was opened
  if (input != stdin) {
    fclose(input);
  }
  if (output != stdout) {
    fclose(output);
  }

  return 0;
}