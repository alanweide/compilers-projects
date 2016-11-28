int main() {
	int x;
	int y;
	while (x + 4 < 0) {
		y = 1 + x;
	}
	if (y > 4 + x) {
		y = y - 1;
	}
	if (x < y-x) {
		x = y;
	} else {
		y = x;
	}
	return 0;
}
