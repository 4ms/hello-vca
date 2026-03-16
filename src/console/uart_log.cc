#include "uart_log.hh"
#include <cstdarg>
#include <cstdio>

namespace MetaModule
{
void UartLog::init() {
	log_uart.init();
}

void UartLog::putchar(char c) {
	log_uart.putchar(c);
}

void UartLog::log(const char *format, ...) {
	va_list va;
	va_start(va, format);
	vprintf(format, va);
	va_end(va);
}

void UartLog::puts(const char *ptr) {
	while (*ptr) {
		UartLog::putchar(*ptr++);
	}
}

void UartLog::write_uart(const char *ptr, size_t len) {
	for (auto idx = 0u; idx < len; idx++) {
		UartLog::putchar(*ptr++);
	}
}

void UartLog::write_stdout(const char *ptr, size_t len) {
	UartLog::write_uart(ptr, len);
}

// This is used for bypassing write() and going direct to UART
extern "C" void _putchar(char c) {
	UartLog::putchar(c);
}

extern "C" int puts(const char *s) {
	int i = 0;
	while (*s) {
		UartLog::putchar(*s++);
		i++;
	}
	return i;
}

} // namespace MetaModule
