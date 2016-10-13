int zz = 0;
int
main ()
{
  int i = 5, b, c[55];
  for (b=0;b<55;b++) c[b] = 0;
  do
    {
      i--;
      b = 4 + 9 - i;
      int j = -i+1;
      while (j < i+1) { j++; c[4*j+5*i-3] = j-i-1+b; }
    }
  while (i > 0);
  for (b=0;b<55;b++) zz+=c[b];
  return zz; /* result should be 150 */
}
