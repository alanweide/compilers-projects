int main() {
	int x;
	int y;
	x = 44;
	y = x + 73;
	if (x*2 < y) {
		int z;
		z = x + y;
		y = z;
	} else {
		x = y - x;
	}
	return 4;
}