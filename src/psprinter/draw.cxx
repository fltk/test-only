

#include <stdio.h
#include <string.h

#include <FL/Fl_PS_Printer.H
#include <FL/Fl.H


///////////////////////////////  text ///////////////////////////////////

void Fl_PS_Printer::transformed_draw(const char* str, int n, double x, double y)

  if (!n||!str||!*str)return
  fprintf(output, "GS\n")
  fprintf(output,"%g %g moveto\n", x , y)
  fprintf(output, "[1 0 0 -1 0 0] concat\n")
  int i=1
  fprintf(output, "\n(")
  for(int j=0;j<n;j++)
    if(i>240)
      fprintf(output, "\\\n")

      i=0
    
    i++
    switch (*str) 
      case '(': case ')'
        fprintf(output, "\\%c" , *str)
        break
      default
        fprintf(output, "%c", *str)
    
    str++
  
  fprintf(output, ") show\n")
  fprintf(output, "GR\n")




/
// End of "$Id$
/







 







