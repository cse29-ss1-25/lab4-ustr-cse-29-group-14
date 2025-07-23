#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ustr.h"

/*
Run in a loop, taking in a a string, a start index and an end index
separated by a single space

Prints out the substring of the input string from index start to index end
*/
int main() {
    char input[256];
    while (fgets(input, sizeof(input), stdin)) {
        // Remove newline character if present
        size_t l = strlen(input);
        if (l > 0 && input[l - 1] == '\n') {
            input[l - 1] = '\0';
        }
        // Parse out from input the string and the space-separated 
        // start and end indices
        char str[256];
        int start, end;
        sscanf(input, "%255s %d %d", str, &start, &end);

        UStr s = new_ustr(str);
        UStr sub = substring(s, start, end);
        print_ustr(sub);
        printf("\n");
        free_ustr(sub);
        free_ustr(s);
    }
    return 0;
}
