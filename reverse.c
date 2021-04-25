#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024

// Note how we only need to traverse the list in reverse
// Thus we only need a pointer to the previous node
struct data_node {
    char line[MAX_LINE];
    struct data_node* pPrev;
} typedef data_node;


data_node* append(data_node* pPrev, char* line) {
    data_node* new_node = (data_node*)malloc(sizeof(data_node));

    if (!new_node) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    strcpy(new_node->line, line);
    new_node->pPrev = pPrev;

    return new_node;
}

void reverse_print(data_node* pLast, FILE* out_stream) {
    if (out_stream == stdout) {
        fprintf(out_stream, "Printing data:\n");
    }
    while (pLast != NULL) {
        fprintf(out_stream, "%s\n", pLast->line);
        pLast = pLast -> pPrev;
    }
}

// Unified error handling in one function
void print_help(char* help_reason) {
    // Changed the required error message a bit. Personal preference towards verbose
    // documentation required me to add more information into the 'help' message
    fprintf(stderr, "Error: %s\n\n", help_reason);
    fprintf(stdout, "Usage: './reverse [in_file] [out_file]'\n");
    fprintf(stdout, "If 'out_file' is not given, the program defaults to stdout.\n");
    fprintf(stdout, "If 'in_file' is not given, the program reads from stdin until ");
    fprintf(stdout, "a blank line is recieved.\n");
    exit(1);
}

data_node* read_data(data_node* pSeed, FILE* in_stream) {

    printf("Reading data...\n");
    char line[MAX_LINE];
    for (;;) {
        if (fgets(line, MAX_LINE, in_stream) != NULL &&
            ((in_stream != stdin) || strcmp(line, "\n"))) {
            // Newline trimming idea stolen from here:
            // https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
            // Basically just find the position of first newline char and
            // after that replace char in that position with end string char
            line[strcspn(line, "\n")] = 0;
            pSeed = append(pSeed, line);

        } else {
            return pSeed;
        }
    }
}

int main(int argc, char* argv[]) {

    data_node* pSeed = NULL;
    FILE* in_stream = stdin;
    FILE* out_stream = stdout;

    switch (argc) {
        case 1:
            // Pass if only 1 argument.
            break;

        case 2:
            in_stream = fopen(argv[1], "r");
            printf("Assigned file '%s' as input stream.\n", argv[1]);
            break;
        
        case 3:
            if (!strcmp(argv[1], argv[2])) {
                print_help("Input and output file must differ.");
            }

            in_stream = fopen(argv[1], "r");
            printf("Assigned file '%s' as input stream.\n", argv[1]);
            out_stream = fopen(argv[2], "w");
            printf("Assigned file '%s' as output stream.\n", argv[2]);
            break;
    
        default:
            print_help("Too many arguments!");
            break;
        }
    
    if (!in_stream) {
        fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
        exit(1);
    }

    if (!out_stream) {
        fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
        exit(2);
    }

    pSeed = read_data(pSeed, in_stream);
    reverse_print(pSeed, out_stream);

    if (in_stream != stdin) {
        fclose(in_stream);
    }

    if (out_stream != stdout) {
        fclose(out_stream);
    }
}