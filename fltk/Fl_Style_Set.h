#ifndef Fl_Style_Set_h
#define Fl_Style_Set_h

#include <fltk/Fl_Style.h>

/**

   Fltk can manage "sets" of styles. This is useful for making a program that 
   displays more than one "theme" at a time, such as a program that edits or 
   creates themes. The "test" widgets can display the theme under development,
   while the main widgets stay unchanged. Fluid uses this to allow you to try 
   your program under different themes. 

   Each widget "belongs" to a set, which is assigned to the current set when 
   the widget is constructed. 

   The actual implementation is to change the pointers such as 
   Fl_Widget::default_style for every widget class to point to a new copy,
   so when the constructors for each widget are run they assign the new copy.
   The previous copies are saved and restored if the older set name is used. 

   There are some problems. Many widgets are dynamically created (for instance
   pop-up menus) and may not inherit the expected theme unless you are 
   careful. Also some global things such as the background color cannot 
   be seperated into sets.

*/
class FL_API Fl_Style_Set {
  Fl_Named_Style* first_style;
  Fl_Theme theme;
  const char* scheme;
  Fl_Color background;
  void* reserved; // pointer to extra saved stuff
public:
  /**
   The first Fl_Style_Set you create is made into the current one 
   and is made equal to the current settings of the styles. All other
   style sets you create are set to a revert form of all the styles 
   and you must call make_current on them to make them useful. 
  */
  Fl_Style_Set();
  /**
   Make this set be current. All widgets created after this use this set,
   and calling Fl_Style::reload_theme() will change the styles belonging 
   to tis set. 
  */
  void make_current();
  /**
   Currently this does nothing.
  */
  ~Fl_Style_Set();
};

#endif
