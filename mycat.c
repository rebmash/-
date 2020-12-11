#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
void copy(int r, int w)
{
	char buffer[4096];
	int k;
	while ((k = read(r, buffer, 4096)) > 0)
	{
		write(w, buffer, k);
	}
}
int main(int ac, char *av[])
{
	if (ac == 1)
	{
		copy(0, 1);
	}
	else
	{
		for (int i = 1; i < ac; i++)
		{
			int fd = open(av[i], O_RDONLY);
			if (fd < 0)
			{
				//perror("cat");
				return 0;
			}
			copy(fd, 1);
			close(fd);
		}
	}
}


