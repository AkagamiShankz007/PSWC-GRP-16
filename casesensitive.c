#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to convert a string to lowercase (for case-insensitive search)
void to_lowercase(char *str) {
    for (; *str; str++) *str = tolower(*str);
}

// Function to search for 'needle' inside 'line' (basic substring check)
int contains(const char *line, const char *needle) {
    while (*line) {
        const char *l = line, *n = needle;
        while (*l && *n && *l == *n) { l++; n++; }
        if (!*n) return 1;  // If entire needle is found
        line++;
    }
    return 0;
}

void search_file(const char *filename, const char *term, int case_insensitive) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { printf("Error: Cannot open file %s\n", filename); return; }

    char line[1024], term_lower[1024];
    strcpy(term_lower, term);
    if (case_insensitive) to_lowercase(term_lower);

    int line_num = 0;
    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        if (case_insensitive) {
            char line_lower[1024];
            strcpy(line_lower, line);
            to_lowercase(line_lower);
            if (contains(line_lower, term_lower)) printf("%s:%d: %s", filename, line_num, line);
        } else if (contains(line, term)) {
            printf("%s:%d: %s", filename, line_num, line);
        }
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 3) { printf("Usage: ./mygrep [-i] search_term file\n"); return 1; }

    int case_insensitive = 0;
    const char *term = argv[1], *filename = argv[2];
    if (strcmp(argv[1], "-i") == 0) { case_insensitive = 1; term = argv[2]; filename = argv[3]; }
    
    search_file(filename, term, case_insensitive);
    return 0;
}
