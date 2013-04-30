int main() {
  unsigned char * i = (unsigned char *) 0xb8000;
  i[0] = 'X';
  i[2] = 'Y';
  for(;;) {}
}
