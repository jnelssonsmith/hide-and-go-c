 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 10/03/17
DESCRIPTION: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(void)
{
	FILE *stream;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
 
	stream = fopen("./ppm-files/feep.ppm", "r");
	if (stream == NULL)
		exit(EXIT_FAILURE);
 
	while ((read = getline(&line, &len, stream)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
	    printf("%s", line);
        int i;
        for(i = 0; i < strlen(line); i++) {
            if(line[i] == 35) {
                printf("Found comment\n");
            }
        }
	}
 
	free(line);
	fclose(stream);
	exit(EXIT_SUCCESS);
}