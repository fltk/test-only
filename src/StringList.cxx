#include <fltk/StringList.h>
#include <fltk/Item.h>
using namespace fltk;

/*! \class fltk::StringList

  A very simple type of List that allows a menu to display an
  array of C strings, or a nul-seperated single string:

  \code
  const char* names[] = {"alpha", "beta", "gamma", 0}
  StringList* list = new StringList(names);
  // Identical result can be made with this:
  // StringList* list = new StringList("alpha\0beta\0gamma\0");
  menu->list(list);
  \endcode
*/

int StringList::children(const Menu* group, const int* indexes, int level) {
  // Get number of widget children:
  int n = List::children(group, 0,0);	
  if (!level) {
    // top level has widgets + all strings
    return children_ + n;
  } else if (*indexes < n) {
    // return children counts for widgets:
    return List::children(group, indexes, level);
  } else {
    // the strings have no children:
    return -1;
  }
}

Widget* StringList::child(const Menu* group, const int* indexes, int level) {
  // Return children for any widgets:
  int n = *indexes - List::children(group, 0,0);
  if (n < 0) return List::child(group, indexes, level);
  // Construct reusable widget for string and return it:
  static Widget* widget;
  if (!widget) {
    Group::current(0);
    widget = new Item();
  }
  widget->type(Widget::NORMAL);
  widget->label(array[n]);
  widget->w(0);
  return widget;
}

/*! \fn StringList::StringList(const char*const* a,int n)
  Sets the string list to return \a n items, where the labels are
  from the array \a a. Warning: the array is not copied, it should
  be in static memory!
*/

/*! Sets the string list to return items who's labels are from the
  array \a a. The number of items is determined by the first null
  pointer in the array. Warning: the array is not copied, it should
  be in static memory! */
StringList::StringList(const char*const* a) {
  array = a;
  for (children_ = 0; a && a[children_]; children_++);
}

/*! Sets the string list to return the nul-seperated list of strings.
  The list is terminated by two nul characters. Warning: the string
  is not copied! Also the current implementation leaks the created
  array when the StringList is deleted. */
StringList::StringList(const char* s) {
  if (!s || !*s) {children_ = 0; return;}
  const char* temp[256];
  int n = 0;
  while (*s && n < 256) {
    temp[n] = s;
    while (*s++);
    n++;
  }
  children_ = n;
  const char** t = new const char* [n]; // this array is never freed...
  array = t;
  for (; n--;) t[n] = temp[n];
}
