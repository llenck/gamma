#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
	int diff;
	if (argc < 2 || (diff = atoi(argv[1])) == 0) {
		fprintf(stderr, "Usage: %s <diff>\n", argv[0]);
		return 1;
	}

	int val;
	int kekfd = open("/tmp/gamma", O_RDWR | O_CLOEXEC);
	if (kekfd < 0) {
		val = 10;
		kekfd = open("/tmp/gamma", O_RDWR | O_CLOEXEC | O_CREAT, 0644);
		if (kekfd < 0) {
			perror("Failed to open /tmp/gamma");
			return errno;
		}
	}
	else {
		errno = 0;
		if (read(kekfd, &val, sizeof(val)) != sizeof(val)) {
			perror("Failed to read old value");
			return errno ?: 1;
		}
	}

	val += diff;

	if (lseek(kekfd, 0, SEEK_SET) < 0) {
		perror("Failed to seek to byte 0");
		return errno ?: 1;
	}
	if (write(kekfd, &val, sizeof(val)) != sizeof(val)) {
		perror("Failed to write updated value");
		return errno ?: 1;
	}

	char tmp[16];
	int ones = val / 10, tenths = val % 10;
	sprintf(tmp, "%d.%d", ones, tenths);

	char* args[] = { "xrandr", "--output", "HDMI-0", "--gamma", tmp, NULL };
	execvp("xrandr", args);

	// never reached
	return 0;
}
