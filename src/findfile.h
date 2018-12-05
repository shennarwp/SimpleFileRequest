#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int display_info(const char *fpath, const struct stat *sb,
 	int tflag, struct FTW *ftwbuf);

void findfile(char *buf, char *array, int bytes);



