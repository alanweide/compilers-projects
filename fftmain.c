#include <stdio.h>
#include "originalfftdriver.c"
#include "compiledfftdriver.c"

int main() {
	printf("original: %lf\n", originalfft());
	printf("compiled: %lf\n", compiledfft());
}