#include "findfile.h"
#define MAX_NAME 5

const char *filepath = ".";

char result[1024];						//containing the message about the file informaton
char filename[MAX_NAME][64];			//filenames to be searched as double array
int found[MAX_NAME];					//correspond to the index in array filename[], all will be set as -1 (not found) initially
int size;								//first n-bytes

static int display_info(const char *fpath, const struct stat *sb,
	int tflag, struct FTW *ftwbuf)
{

	//looping through the array filename[]
	int counter = 0;
	while(counter < MAX_NAME) {
		//comparing if file on disk with the same name as filename[counter] exist
		if(strcmp(filename[counter], fpath + ftwbuf->base) == 0)
		{

			//append status message to result
			char *str1 = "FOUND!\t\t";
			char *str2 = filename[counter];
			strcat(result, str1);
			strcat(result, str2);

			//open, read first n bytes from the file
			FILE *fp;
			char values[11];
			fp = fopen(fpath, "r");
			fread(values, 1, size, fp);
			fclose(fp);

			//append the first n bytes information to the result
			char *str3 = "\t\t\tfirst ";
			char *str4;
			sprintf(str4, "%d", size);
			char *str5 = " bytes: ";
			strcat(result, str3);
			strcat(result, str4);
			strcat(result, str5);
			strcat(result, values);
			strcat(result , "\n");

			//marking the array found[] with the index in array filename[]
			found[counter] = counter;
		}
		counter++;
	}

	return 0;           //To tell nftw() to continue
}


void findfile(char *buf, char *array, int bytes) {

	size = bytes;
	bzero(result, 1024);

	//initialize all value as -1 (not found)
	for(int i = 0; i < MAX_NAME; i++)
		found[i] = -1;

	//split string passed as argument to tokens with space and newline as delimiter
	const char s[] = " \n";						//delimiter
	char *search = strtok(array, s);			//first token
	printf("Files to be searched are:\n");

	int counter = 0;							//loop until last token
	while(search != NULL && counter < 5)
	{
		strcpy(filename[counter], search);		//adding each token to the array filename[]
		printf("%s\n", filename[counter]);

		search = strtok(NULL, s);				//continue to next token
		counter++;
	}
	printf("--------------------------\n");

	//traverse the filesystem at starting point defined at filepath
	int flags = 0;			//nftw flags
	if (nftw(filepath, display_info, 20, flags) == -1)
	{
		perror("nftw");
		exit(EXIT_FAILURE);
	}

	//every index still marked as -1 in array found means that element in array filename at that index
	//is not found
	for(int i = 0; i < MAX_NAME; i++) {
		if(found[i] == -1 && filename[i][0] != '\0') {
			char *str1 = "NOT FOUND!\t";
			char *str2 = filename[i];
			strcat(result, str1);
			strcat(result, str2);
			strcat(result, "\n");
		}
	}

	//copy to the array belongs to whichever method calling findfile()
	strcpy(buf, result);
}

