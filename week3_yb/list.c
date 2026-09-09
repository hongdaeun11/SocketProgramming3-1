#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

main()
{
	DIR *dp;
	struct dirent *dir;

	if ((dp = opendir(".")) == NULL) {
		fprintf(stderr, "directory open error\n");
		exit(-1);
	}

	while ((dir = readdir(dp)) != NULL) {
		if (dir->d_ino == 0) continue;
		//if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) continue;
		if((dir->d_name)[0] != '.') printf("%s\n", dir->d_name);	

		//printf("%s\n", dir->d_name);
	}

	closedir(dp);
}
