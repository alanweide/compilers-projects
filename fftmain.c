#include <stdio.h>
#include "originalfftdriver.c"
#include "compiledfftdriver.c"

int main() {
	printf("original: %lf", originalfft());
	printf("\noriginal: %lf", compiledfft());
}