#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void do_ls(char []);
int countDirEntries(char []);
struct dirent *getDirEntryArray(char dirname[],int *cnt);
int compareDirEntries (struct dirent *i, struct dirent *j);

int main(int argc,char *argv[])
{
	if(argc == 1)
	{
		do_ls(".");
	}
	else
		while(--argc){
			printf("%s:\n",*++argv);
			do_ls(*argv);
		}
}
void do_ls(char dirname[])
{
		int cnt;
		struct dirent *entries = getDirEntryArray(dirname,&cnt);
		qsort(entries, cnt, sizeof(struct dirent),(void*) compareDirEntries);
		for (int i=0; i < cnt; i++)
		{
			if(i%5==0 && i!=0) printf("\n");
			printf("%s\t", entries[i].d_name);
		}
		free(entries);
}
struct dirent *getDirEntryArray(char dirname[],int *cnt)
{

	int entrycount = countDirEntries(dirname);
	
	*cnt=entrycount;

	struct dirent *dArray;
	dArray = (struct dirent *) malloc(sizeof(struct dirent)*entrycount);

	
	 
	DIR *dir_ptr;
	struct dirent *direntp;
	int entry = 0;

	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr,"ls1:cannot open %s\n",dirname);
		
	else
	{
		
		while((direntp = readdir(dir_ptr)) != NULL){
			
			if (!strcmp(direntp->d_name,".") ||
			 	!strcmp(direntp->d_name,".."))
				continue;
			memcpy(&dArray[entry],direntp,sizeof(struct dirent));
			entry++;
		}

		closedir(dir_ptr);
	}
	return dArray;
}

int countDirEntries(char dirname[])
{

	DIR *dir_ptr;
	struct dirent *direntp;
	int j = 0;
	if((dir_ptr = opendir(dirname)) == NULL)
	{
		fprintf(stderr,"ls1:cannot open %s\n",dirname);
		return 0;
	}
	else
	{
		while((direntp = readdir(dir_ptr)) != NULL){
			
			if (!strcmp(direntp->d_name,".") ||
			 	!strcmp(direntp->d_name,".."))
				continue;
			j++;
		}
		closedir(dir_ptr);
	}
	return j;
}

int compareDirEntries (struct dirent *i, struct dirent *j){
	return strcmp(i->d_name,j->d_name);
}