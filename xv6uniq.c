#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

#define LINE_LENGTH 1024

int main(int argc, char *argv[]) {
  int input = 0;
  int output = 1;
  int show_duplicates = 0;

  if (argc == 1) {
    printf(1, "Enter Text:");
    input = 0;
    output = 1;
  }
  if (argc == 2) {
    if (strcmp(argv[1], "-d") == 0) {
      show_duplicates = 1;
      printf(1, "Enter Text:");
    } else {
      input = open(argv[1], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    }
  }
  if (argc == 3) {
    if (strcmp(argv[1], "-d") == 0) {
      show_duplicates = 1;
      input = open(argv[2], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    } else if (strcmp(argv[2], "-d") == 0) {
      show_duplicates = 1;
      input = open(argv[1], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    } else {
      input = open(argv[1], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
      output = open(argv[2], O_WRONLY);
      if (output < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    }
  }

  if (argc == 4) {
    if (strcmp(argv[1], "-d") == 0) {
      show_duplicates = 1;
      input = open(argv[2], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
      output = open(argv[3], O_WRONLY);
      if (output < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    } else if (strcmp(argv[2], "-d") == 0) {
      show_duplicates = 1;
      input = open(argv[1], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
      output = open(argv[3], O_WRONLY);
      if (output < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    } else {
      show_duplicates = 1;
      input = open(argv[1], O_RDONLY);
      if (input < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
      output = open(argv[2], O_WRONLY);
      if (output < 0) {
        printf(1, "Error opening file. \n");
        exit();
      }
    }
  }

  // Read and process the input file line by line
  char current_line[LINE_LENGTH];
  char previous_line[LINE_LENGTH];
  fgets(current_line, LINE_LENGTH, input);
  while (strcmp(fgets(current_line, LINE_LENGTH, input),"")!=0) {
    // If the -d flag is provided, only print lines that are duplicates
    if (show_duplicates) {
      if (strcmp(current_line, previous_line) == 0) {
        // printf("%s", current_line);
        printf(output, current_line);
      }
    } else {
      // Otherwise, print all unique lines
      if (strcmp(current_line, previous_line) != 0) {
        // printf("%s", current_line);
        printf(output, current_line);
      }
    }
    if (show_duplicates) {
      if (strcmp(current_line, previous_line) != 0)
        strcpy(previous_line, current_line);
    } else {
      strcpy(previous_line, current_line);
    }
  }

  if (input != 0) {
    close(input);
  }
  if (output != 1) {
    close(output);
  }

  exit();
}