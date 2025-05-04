#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

int word_count(char *line , char *search_term, int case_sensitive);
void lowercase(char *str);
int check(char *line , char *search_term , int case_sensitive);

int main()
{
    char filename[500];
    char search_term[500];
    char case_option[10];
    int case_sensitive;
    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    int total_count = 0;
    int found_matches = 0;

    printf("Enter filename to search: ");
    if(!fgets(filename, sizeof(filename), stdin)) 
    {
        printf("Error reading filename.\n");
        return 1;
    }
    filename[strcspn(filename, "\n")] = 0; 

    printf("Enter search term: ");
    if(!fgets(search_term, sizeof(search_term), stdin)) 
    {
        printf("Error reading search term.\n");
        return 1;
    }
    search_term[strcspn(search_term, "\n")] = 0;

    printf("Case-sensitive search? (1/0): ");
    if (!fgets(case_option, sizeof(case_option), stdin)) 
    {
        printf("Error reading option.\n");
        return 1;
    }
    case_option[strcspn(case_option, "\n")] = 0;

    case_sensitive = atoi(case_option);  // ✅ FIXED: Read user input correctly
    if (case_sensitive != 0 && case_sensitive != 1) 
    {
        printf("Invalid input for case sensitivity. Use 1 or 0.\n");
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file\n");
        return 1;
    }
    
    while (fgets(line, sizeof(line), file)) 
    {
        line_number++;
        if (check(line, search_term, case_sensitive)) 
        {
            int count_in_line = word_count(line, search_term, case_sensitive);
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

int check(char *line , char *search_term , int case_sensitive)
{ 
    if (case_sensitive)
    {
        return strstr(line, search_term) != NULL;  // ✅ FIXED: Match exactly as typed
    }
    else
    {
        char lowerline[1024];
        char lowerterm[1024];
        strcpy(lowerline, line);
        strcpy(lowerterm, search_term);
        lowercase(lowerline);
        lowercase(lowerterm);
        return strstr(lowerline, lowerterm) != NULL;
    }
}

void lowercase(char *str)
{
    for (int i = 0; str[i]; i++) 
    {
        str[i] = tolower(str[i]);
    }
}

int word_count(char *line , char *search_term , int case_sensitive)
{
    int count = 0;
    char *p;
    int termlength = strlen(search_term);

    if (termlength == 0)
        return 0;

    char buffer_line[1024], buffer_term[1024];

    if (!case_sensitive)
    {
        strcpy(buffer_line, line);
        strcpy(buffer_term, search_term);
        lowercase(buffer_line);
        lowercase(buffer_term);
        p = buffer_line;
    }
    else
    {
        p = line;
        strcpy(buffer_term, search_term);
    }

    while ((p = strstr(p, buffer_term)) != NULL) 
    {
        count++;
        p += termlength;
    }

    return count;
}
