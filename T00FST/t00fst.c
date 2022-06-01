/* Larionova Polina */
#include <stdio.h>
#include <windows.h>

void main( void )
{
  int x;
  char buf[100];

  scanf("%i", &x);
  sprintf(buf, "Polin = %i", x);
  MessageBox(NULL, buf, "Caption", MB_OK | MB_ICONINFORMATION );
}