

/
// This library is free software; you can redistribute it and/o
// modify it under the terms of the GNU Library General Publi
// License as published by the Free Software Foundation; eithe
// version 2 of the License, or (at your option) any later version
/
// This library is distributed in the hope that it will be useful
// but WITHOUT ANY WARRANTY; without even the implied warranty o
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GN
// Library General Public License for more details
/
// You should have received a copy of the GNU Library General Publi
// License along with this library; if not, write to the Free Softwar
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-130
// USA
/

#ifndef fl_printer_chooser_
#define fl_printer_chooser_



#ifdef WIN3
#include <FL/Fl_GDI_Printer.H

class Fl_GDI_Printer_Settings:public PRINTDLG
public
  Fl_GDI_Printer_Settings()
    ZeroMemory(&lStructSize, sizeof(PRINTDLG))
    lStructSize = sizeof(PRINTDLG)
    nCopies     = 1
    nMinPage =1
    nMaxPage =0
    nFromPage =1
    nToPage = 0
    Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_ALLPAGES  | PD_RETURNDEFAULT| PD_NOSELECTION | PD_NOPAGENUMS
    PrintDlg(this)
    GlobalLock(hDevMode)
    GlobalLock(hDevNames)
  }
  ~Fl_GDI_Printer_Settings()
    if(!GlobalUnlock(hDevMode)
      GlobalFree(hDevMode)
    if(!GlobalUnlock(hDevNames)
      GlobalFree(hDevNames)
  }
}


inline Fl_GDI_Printer *  fl_printer_chooser(Fl_GDI_Printer_Settings * s = 0)
  int d=0
  if(!s)
    d=1
    s=new Fl_GDI_Printer_Settings()
  

  Fl_GDI_Printer * p
  s->Flags |= PD_RETURNDC
  s->Flags &= ~PD_RETURNDEFAULT
  GlobalLock(s->hDevMode)
  GlobalLock(s->hDevNames)
  if(PrintDlg(s))
    p = new Fl_GDI_Printer(s->hDC,(DEVMODE *)(s->hDevMode))
  }els
    p = 0
  //s->pd.Flags &= ~PD_RETURNDC; //resetting returngc
  if(d)
     delete s
  
  return p




#else //WIN323

#include <FL/fl_ask.H
#include <FL/Fl_PS_Printer.H
#include <stdio.h
#include <string.h
#include <signal.h


static int pclose_command( FILE * f)
   int i = pclose(f)
   signal(SIGPIPE, SIG_DFL)
   return i


static char *fl_ps_command=0

static Fl_Printer * fl_printer_chooser(int lang_level = 3,  int pages = 0, char ** command=0,  const char * default_command = 0, const char * label = "Choose printing command")

  if(!command
    command = &fl_ps_command
  if(! *command)
    const char * cmd = fl_input(label, default_command)
    if(!cmd) return 0
    char * com = new char[strlen(cmd)+1]
    strcpy(com, cmd)
    *command = com
  
  FILE * pipe = popen(*command, "w")
  if(!pipe)
     fl_message("Can't open print manager")
     return 0
  
  signal(SIGPIPE, SIG_IGN);  //temporary hack to ignore broken pipe, ie. if user clicks "cancel" in the dialo
  Fl_PS_Printer * p = new Fl_PS_Printer(pipe, lang_level, pages)
  p->close_command(&pclose_command)
  return p
}




#endif //WIN3

#endi
