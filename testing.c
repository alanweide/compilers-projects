int main() {
	int x;
	int y;
	x = 44;
	y = x + 73;
	int i,j;
	int z = 2;
	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			z *= -1;
		}
	}
	return z;
}
