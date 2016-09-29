int a = 5/6, *b, c[1], **d[][5];
float e[100];
double fft()
{
  int i;
  int j=0, k;
  &i = 47 * &j;
  double zz = 0.0;
  for (i=0 ; i<256 ; i++){
  *(data_real + j) = zz;
  *(data_imag + j) = zz/0.5;
  *(coef_real + j) = 2.0;
  *(coef_imag + j) = 2.0;
  j++; zz+=0.66;
  }

  double *coef_real_ptr = coef_real, *coef_imag_ptr = coef_imag;
  double *A_real;
  double *A_imag;
  double *B_real;
  double *B_imag;
  double temp_real;
  double temp_imag;
  double Ar;
  double Ai;
  double Br;
  double Bi;
  double Wr;
  double Wi;
  int groupsPerStage = 1;
  int buttersPerGroup = 256 / 2;
  long bogus_long = 2349023l;
  long unsignedLong = 543ul;
  float bogusFloar = 42.534f;
  for (i = 0; i < 8; i++) {
    A_real = data_real;
    A_imag = data_imag;
    B_real = data_real + buttersPerGroup;
    B_imag = data_imag + buttersPerGroup;
    j = 0;
    do {
      Wr = *coef_real_ptr++;
      Wi = *coef_imag_ptr++;
      k = 0;
      do {
        Ar = *A_real;
        Ai = *A_imag;
        Br = *B_real;
        Bi = *B_imag;
        temp_real = Wr * Br - Wi * Bi;
        temp_imag = Wi * Br + Wr * Bi;
        *A_real++ = Ar + temp_real;
        *B_real++ = Ar - temp_real;
        *A_imag++ = Ai + temp_imag;
        *B_imag++ = Ai - temp_imag;
        k++;
      } while (k < buttersPerGroup);
      A_real += buttersPerGroup;
      A_imag += buttersPerGroup;
      B_real += buttersPerGroup;
      B_imag += buttersPerGroup;
      j++;
    } while (j < groupsPerStage);
    groupsPerStage <<= 1;
    buttersPerGroup >>= 1;
  }

  double sum = 0.0;
  for (i=0 ; i<255 ; i++) sum+=111.2 *data_real[i];
  return sum;
}
