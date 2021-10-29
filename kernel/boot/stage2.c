#ifdef _WIN32
#define call
#else
#define call asm("pushw $0");
#endif

void a(void) {
}

void entry(void) {
    a();
}