void Inner(int Lower, int Middle, int Upper)
{
  int i;
  int A[Middle-Lower+1];
  int B[Upper-Middle+1];
  for (i=0; i< Middle-Lower+1;i++)
    A[i] = i;	
  for (i=0; i < Upper-Middle+1; i++)
    B[i] = i;	
  for (i=0; i < Upper-Middle+1; i++)
    B[i] = A[i % (Middle-Lower+1)];	
}

int main()
{
   Inner(1, 15, 30);
};
