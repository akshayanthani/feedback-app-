#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum size for incoming form data
#define MAX_INPUT 1000

int main() {
    char *len_str;
    int len;
    char input[MAX_INPUT];
    char feedback[500];

    // Print HTTP headers required for the browser
    printf("Content-Type: text/html\n\n");
    printf("<html><head><title>Feedback Response</title></head><body>");

    // Read CONTENT_LENGTH environment variable
    len_str = getenv("CONTENT_LENGTH");
    if (len_str != NULL) {
        len = atoi(len_str);
        if (len > 0 && len < MAX_INPUT) {
            // Read POST data from stdin
            fread(input, 1, len, stdin);
            input[len] = '\0';

            // Assume input is: feedback=Some+text+here
            // Simple parse: find '=' and copy rest
            char *eq = strchr(input, '=');
            if (eq != NULL) {
                strcpy(feedback, eq + 1);

                // Replace '+' with ' ' (space), basic URL decode
                for (int i = 0; feedback[i]; i++)
                    if (feedback[i] == '+') feedback[i] = ' ';

                // Save feedback to a file
                FILE *file = fopen("feedback.txt", "a");
                if (file != NULL) {
                    fprintf(file, "%s\n", feedback);
                    fclose(file);
                    printf("<h2>Thank you for your feedback!</h2>");
                } else {
                    printf("<h2>Error saving feedback.</h2>");
                }
            } else {
                printf("<h2>No feedback found in input.</h2>");
            }
        } else {
            printf("<h2>Invalid input length.</h2>");
        }
    } else {
        printf("<h2>No input received.</h2>");
    }

    printf("<a href='/index.html'>Back to Home</a>");
    printf("</body></html>");
    return 0;
}
