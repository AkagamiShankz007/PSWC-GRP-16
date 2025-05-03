#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

// Convert a string to lowercase (for case-insensitive comparison)
void to_lowercase(char *str) {
    for (; *str; ++str) *str = tolower((unsigned char)*str);
}

// Returns 1 if line contains search_term, considering case sensitivity
int contains(const char *line, const char *search_term, int case_sensitive) {
    if (case_sensitive) 
    {
        return strstr(line, search_term) != NULL;
    } 
    else 
    {
        // Make copies for lowercase comparison
        char line_lc[MAX_LINE_LENGTH];
        char term_lc[MAX_LINE_LENGTH];
        strncpy(line_lc, line, MAX_LINE_LENGTH);
        line_lc[MAX_LINE_LENGTH - 1] = '\0';
        strncpy(term_lc, search_term, MAX_LINE_LENGTH);
        term_lc[MAX_LINE_LENGTH - 1] = '\0';
        to_lowercase(line_lc);
        to_lowercase(term_lc);
        return strstr(line_lc, term_lc) != NULL;
    }
}

// Count occurrences of search_term in line, considering case sensitivity
int count_occurrences(const char *line, const char *search_term, int case_sensitive) {
    int count = 0;
    const char *p = line;
    size_t term_len = strlen(search_term);
    if (term_len == 0) return 0;

    if (!case_sensitive) {
        char line_lc[MAX_LINE_LENGTH];
        char term_lc[MAX_LINE_LENGTH];
        strncpy(line_lc, line, MAX_LINE_LENGTH);
        line_lc[MAX_LINE_LENGTH - 1] = '\0';
        strncpy(term_lc, search_term, MAX_LINE_LENGTH);
        term_lc[MAX_LINE_LENGTH - 1] = '\0';
        to_lowercase(line_lc);
        to_lowercase(term_lc);
        p = line_lc;

        while ((p = strstr(p, term_lc)) != NULL) {
            count++;
            p += term_len;
        }
    } else {
        while ((p = strstr(p, search_term)) != NULL) {
            count++;
            p += term_len;
        }
    }
    return count;
}

int main() {
    char filename[256];
    char search_term[256];
    char case_option[10];
    int case_sensitive = 1;

    printf("Enter filename to search: ");
    if (!fgets(filename, sizeof(filename), stdin)) {
        printf("Error reading filename.\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0; // remove newline

    printf("Enter search term: ");
    if (!fgets(search_term, sizeof(search_term), stdin)) {
        printf("Error reading search term.\n");
        return 1;
    }
    search_term[strcspn(search_term, "\n")] = 0; // remove newline

    printf("Case-sensitive search? (yes/no): ");
    if (!fgets(case_option, sizeof(case_option), stdin)) {
        printf("Error reading option.\n");
        return 1;
    }
    case_option[strcspn(case_option, "\n")] = 0; // remove newline
    if (strcasecmp(case_option, "no") == 0) {
        case_sensitive = 0;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int total_count = 0;
    int found_matches = 0;

    while (fgets(line, sizeof(line), file)) 
    {
        line_number++;
        if (contains(line, search_term, case_sensitive)) 
        {
            int count_in_line = count_occurrences(line, search_term, case_sensitive);
            printf("Line %d: %s", line_number, line);
            printf("Occurrences in line: %d\n", count_in_line);
            total_count += count_in_line;
            found_matches = 1;
        }
    }

    if (!found_matches) {
        printf("No matches found for \"%s\" in %s.\n", search_term, filename);
    } else {
        printf("\nTotal occurrences of \"%s\": %d\n", search_term, total_count);
    }

    fclose(file);
    return 0;
}
