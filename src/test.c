#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int n;
	char *end;
	char buf[4];

	do {
	     if (!fgets(buf, sizeof buf, stdin))
	        break;

	     // remove \n
	     buf[strlen(buf) - 1] = 0;

	    n = strtol(buf, &end, 10);
	} while (end != buf + strlen(buf) || n < 0 || n > 10);

	printf("%s\n", buf);

}