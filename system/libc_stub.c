// #define GCC_OPTIMIZE_OFF __attribute__((optimize("-O0")))
// #include <stdio.h>

void _init(void) {
}

void _fini(void) {
}

void _exit(int x) {
	// puts("_exit\n");
	while (1)
		;
}

int _kill(int x) {
	// puts("_kill\n");
	return -1;
}

int _getpid() {
	return -1;
}

int *__errno() {
	static int errno = 0;
	return &errno;
}

void *__dso_handle = (void *)&__dso_handle;

int _open(const char *filename, int flags, int mode) {
	return -1;
}

int _close(int fd) {
	return -1;
}

int _lseek(int fd, int offset, int whenc) {
	return -1;
}

int _read(int fd, char *ptr, int len) {
	return -1;
}

int _fstat(int x, void *p) {
	return 0;
}

int _isatty(int x) {
	return -1;
}

int _stat() {
	return -1;
}

int _sbrk(int incr) {
	return 0;
}

int __aeabi_atexit(void *arg, void (*func)(void *), void *d) {
	return -1;
}
