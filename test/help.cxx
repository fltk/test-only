/
// "$Id$
/
// Fl_Help_Dialog test program
/
// Copyright 1999-2003 by Easy Software Products
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
// Please report all bugs and problems to "fltk-bugs@fltk.org"
/
// Contents
/
//   main() - Display the help GUI..
/

/
// Include necessary headers..
/

#include <FL/Fl_Help_Dialog.H


/
// 'main()' - Display the help GUI..
/

int				// O - Exit statu
main(int  argc,			// I - Number of command-line argument
     char *argv[])		// I - Command-line argument

  Fl_Help_Dialog	*help;		// Help dialo


  help = new Fl_Help_Dialog

  if (argc < 2
    help->load("../documentation/index.html")
  els
    help->load(argv[1])

  help->show(1, argv)

  Fl::run()

  delete help

  return (0)



/
// End of "$Id$"
/
