#include <stdio.h>
#include "originalfftdriver.c"
#include "compiledfftdriver.c"


int main() {
	cout << "original: " << originalfft();
	cout << "\ncompiled: " << compiledfft();
}