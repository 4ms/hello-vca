#include "uart_log.hh"

extern "C" __attribute__((used)) int _write(int fd, char *ptr, int len) {
	MetaModule::UartLog::write_stdout(ptr, len);
	return len;
}
