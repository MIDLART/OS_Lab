#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef enum 
{
	ok,
	invalid_arguments,
	not_open
} status_code;

status_code print_file_info(char* file_path, int* total)
{
	struct stat file_stat;
	if (stat(file_path, &file_stat) != 0)
	{
		return invalid_arguments;
	}
	
	*total += file_stat.st_blocks;

	if (S_ISDIR(file_stat.st_mode))
	{
		printf("d");
	}
	else if (S_ISREG(file_stat.st_mode))
	{
		printf("-");
	}
	else if (S_ISLNK(file_stat.st_mode))
	{
		printf("l");
	}
	else if (S_ISFIFO(file_stat.st_mode))
	{
		printf("p");
	}
	else if (S_ISSOCK(file_stat.st_mode))
	{
		printf("s");
	}
	else if (S_ISCHR(file_stat.st_mode))
	{
		printf("c");
	}
	else if (S_ISBLK(file_stat.st_mode))
	{
		printf("b");
	}
	else
	{
		printf("-");
	}

	printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
	printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
	printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");

	printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
	printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
	printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");

	printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
	printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
	printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

	printf("\t%lu", file_stat.st_nlink);
	printf("\t%s", getpwuid(file_stat.st_uid)->pw_name);
	printf("\t%s", getgrgid(file_stat.st_gid)->gr_name);
	printf("\t%lld", (long long)file_stat.st_size);
	
	char time_str[20];
	strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat.st_mtime));
	printf("\t%s", time_str);

	int i = 0;
	while (file_path[i] != '\0')
	{
		i++;
	}
	
	while (file_path[i] != '/')
	{
		i--;
	}
	i++;

	printf("\t%s\n", file_path + i);

	return ok;
}

status_code ls(char* dir_name) 
{
	int total = 0;
	
	DIR *dir;
	struct dirent *ds;

	dir = opendir(dir_name);
	if (!dir) {
		return not_open;
	}

	while ((ds = readdir(dir)) != NULL) 
	{
		char path[PATH_MAX];
		snprintf(path, sizeof(path), "%s/%s", dir_name, ds->d_name);
		
		if (print_file_info(path, &total) == invalid_arguments) 
		{
			return invalid_arguments;
		} 
		// printf("%s\n", ds->d_name);
	}
	
	total /= 2;
	
	printf("Total: %d\n", total);

	closedir(dir);
	
	return ok;
}

int main (int argc, char* argv[]) 
{
	if (argc < 2) 
	{
		printf("Invalid arguments\n");
		return invalid_arguments;
	}
	
	for (int i = 1; i < argc; i++) 
	{
		if (ls(argv[i]) != ok)
		{
			printf("Invalid arguments\n");
			return invalid_arguments;
		}
	}
	
	return ok;
}
