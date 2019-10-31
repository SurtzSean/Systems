#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <pwd.h>
#include<grp.h>

void do_ls(char []);
int countDirEntries(char []);
struct dirent *getDirEntryArray(char dirname[],int *cnt);
int compareDirEntries (struct dirent *i, struct dirent *j);
void show_stat_info(char *, struct stat *);
void mode_to_letters(int mode, char str[10]);
void do_ls_two(char [], struct stat info);

int main(int argc,char *argv[])
{
	if(argc == 1)
	{
		do_ls(".");
	}
    else if (argc == 2 && strcmp(argv[1], "-l")==0){
        struct stat info;
        do_ls_two(".", info);
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

void do_ls_two(char dirname[],struct stat info)
{
		int cnt;
		struct dirent *entries = getDirEntryArray(dirname,&cnt);
		qsort(entries, cnt, sizeof(struct dirent),(void*) compareDirEntries);
		for (int i=0; i < cnt; i++)
		{
            if( stat(entries[i].d_name, &info) != -1 )
            show_stat_info(entries[i].d_name, &info);
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

void show_stat_info(char *fname, struct stat *buf)
{
//printf("   mode: %o\n", buf->st_mode);
int mode = buf->st_mode;
char str[10];
mode_to_letters(mode, str);
printf(" %s ", str); 
printf(" %d ", buf->st_nlink);
printf(" %s ",getpwuid(buf->st_uid)->pw_name);  
//printf("  group: %d\n", buf->st_gid);  
printf(" %s ", getgrgid(buf->st_gid)->gr_name);
printf(" %d ", (int)buf->st_size); 
//printf("modtime: %d\n", ctime((int)buf->st_mtime);
char date[20];
strcpy(date,ctime(&buf->st_mtime));
for(int i = 0; date[i]!='\0';i++){
    //printf("[%d]",date[i]);
    if(date[i] == '\n') date[i] = '\0';
}
printf(" %s ",date);
printf(" %s ", fname );   
printf("\n");
}
void mode_to_letters(int mode, char str[10]){
	strcpy(str,"----------");
	if(S_ISDIR(mode)) str[0]='d';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';
	if(mode & S_IRUSR) str[1]='r';
	if(mode & S_IWUSR) str[2]='w';
	if(mode & S_IXUSR) str[3]='x';
	if(mode & S_IRGRP) str[4]='r';
	if(mode & S_IWGRP) str[5]='w';
	if(mode & S_IXGRP) str[6]='x';
	if(mode & S_IROTH) str[7]='r';
	if(mode & S_IWOTH) str[8]='w';
	if(mode & S_IXOTH) str[9]='x';
}