#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <string.h>

static uint fxor = 0;

double now()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

unsigned int xorbuf(unsigned int *buffer, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		fxor ^= buffer[i];
	}
}

long filesize(const char *filename)
{
	struct stat st;
	stat(filename, &st);
	return st.st_size;
}

int main(int argc, const char *argv[])
{
	double startT, endT;
	long blockSize;
	if (argc != 3)
	{
		printf("Usage: ./run3 <filename> <blockSize>\n");
		exit(1);
	}
	if ((blockSize = atol(argv[2])) <= 0)
	{
		printf("must enter a valid block size\n");
		exit(1);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("File Not Found.\r\n");
		exit(1);
	}
	long fsize = filesize(argv[1]);

	startT = now();
	unsigned int *buf = (unsigned int *)malloc(blockSize);
	for (long i = 0; i < fsize; i += blockSize)
	{
		size_t bytes_read = read(fd, buf, blockSize);
		xorbuf(buf, bytes_read / sizeof(unsigned int));
		printf("reading... %.2f%% \r", (double)((i + blockSize) * 100) / (double)(fsize));
	}
	endT = now();

	free(buf);
	double runtime = endT - startT;
	double performance = (fsize) / (1024 * 1024 * runtime);

	printf("\nXOR: %08x\n", fxor);
	printf("BlockSize: %ld, Performance: %f MB/s \n\n", blockSize, performance);
	close(fd);
	return 0;
}