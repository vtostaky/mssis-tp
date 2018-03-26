#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdio.h>

#define IOCTL_START_VINCE 1
#define IOCTL_STOP_VINCE 0

int main()
{
        int fd;

        fd = open("/dev/vince", "r");
        if (ioctl(fd, IOCTL_START_VINCE) == 0) {
                printf("Hello!!!\n");
        }
	chmod("./test.c", S_IRUSR | S_IWUSR);
        if (ioctl(fd, IOCTL_STOP_VINCE) == 0) {
                printf("Goodbye!!!\n");
        }
        close(fd);
   return 0;
}
