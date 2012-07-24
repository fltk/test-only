//
// "$Id$"
//
// An input/chooser widget.
//            ______________  ____
//           |              || __ |
//           | input area   || \/ |
//           |______________||____|
//
// Copyright 1998-2010 by Bill Spitzak and others.
// Copyright 2004 by Greg Ercolano.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

/* \file
 fltk3::InputChoice widget . */



#ifndef Fltk3_Input_Choice_H
#define Fltk3_Input_Choice_H

#include <fltk3/run.h>
#include <fltk3/Group.h>
#include <fltk3/Input.h>
#include <fltk3/MenuButton.h>
#include <fltk3/draw.h>
#include <string.h>


namespace fltk3 {
  
  /**
   A combination of the input widget and a menu button.
   The user can either type into the input area, or use the
   menu button chooser on the right, which loads the input area
   with predefined text.  Normally it is drawn with an inset box 
   and a white background. 
   <P>
   The application can directly access both the input and menu
   widgets directly, using the menubutton()
   and input() accessor methods.
   */
  class FLTK3_EXPORT InputChoice : public fltk3::Group {
    // Private class to handle slightly 'special' behavior of menu button
    class InputMenuButton : public fltk3::MenuButton {
      void draw() {
        draw_box(fltk3::UP_BOX, color());
        fltk3::color(active_r() ? labelcolor() : fltk3::inactive(labelcolor()));
        int xc = w()/2, yc=h()/2;
        fltk3::polygon(xc-5,yc-3,xc+5,yc-3,xc,yc+3);
        if (fltk3::focus() == this) draw_focus();
      }
    public:
      InputMenuButton(int x,int y,int w,int h,const char*l=0) : 
      fltk3::MenuButton(x,y,w,h,l) { box(fltk3::UP_BOX); }
    };
    
    fltk3::Input *inp_;
    InputMenuButton *menu_;
    
    static void menu_cb(fltk3::Widget*, void *data) { 
      fltk3::InputChoice *o=(fltk3::InputChoice *)data;
      fltk3::WidgetTracker wp(o);
      const fltk3::MenuItem *item = o->menubutton()->mvalue();
      if (item && item->flags & (fltk3::SUBMENU|fltk3::SUBMENU_POINTER)) return;	// ignore submenus
      if (!strcmp(o->inp_->value(), o->menu_->text()))
      {
        o->Widget::clear_changed();
        if (o->when() & fltk3::WHEN_NOT_CHANGED)
          o->do_callback();
      }
      else
      {
        o->inp_->value(o->menu_->text());
        o->inp_->set_changed();
        o->Widget::set_changed();
        if (o->when() & (fltk3::WHEN_CHANGED|fltk3::WHEN_RELEASE))
          o->do_callback();
      }
      
      if (wp.deleted()) return;
      
      if (o->callback() != default_callback)
      {
        o->Widget::clear_changed();
        o->inp_->clear_changed();
      }
    }
    
    static void inp_cb(fltk3::Widget*, void *data) { 
      fltk3::InputChoice *o=(fltk3::InputChoice *)data;
      fltk3::WidgetTracker wp(o);
      if (o->inp_->changed()) {
        o->Widget::set_changed();
        if (o->when() & (fltk3::WHEN_CHANGED|fltk3::WHEN_RELEASE))
          o->do_callback();
      } else {
        o->Widget::clear_changed();
        if (o->when() & fltk3::WHEN_NOT_CHANGED)
          o->do_callback();
      }
      
      if (wp.deleted()) return;
      
      if (o->callback() != default_callback)
        o->Widget::clear_changed();
    }
    
    // Custom resize behavior -- input stretches, menu button doesn't
    inline int inp_x() { return(fltk3::box_dx(box())); }
    inline int inp_y() { return(fltk3::box_dy(box())); }
    inline int inp_w() { return(w() - fltk3::box_dw(box()) - 20); }
    inline int inp_h() { return(h() - fltk3::box_dh(box())); }
    
    inline int menu_x() { return(w() - 20 - fltk3::box_dx(box())); }
    inline int menu_y() { return(fltk3::box_dy(box())); }
    inline int menu_w() { return(20); }
    inline int menu_h() { return(h() - fltk3::box_dh(box())); }
    
  public:
    /**
     Creates a new fltk3::InputChoice widget using the given position, size,
     and label string.
     <P> Inherited destructor Destroys the widget and any value associated with it.
     */
    InputChoice (int x,int y,int w,int h,const char*l=0);
    
    /** Adds an item to the menu.*/
    void add(const char *s) { menu_->add(s);  }
    int changed() const { return inp_->changed() | Widget::changed();}
    void clear_changed() { 
      inp_->clear_changed();
      Widget::clear_changed();
    }
    void set_changed() { 
      inp_->set_changed();
      // no need to call fltk3::Widget::set_changed()
    }
    /** Removes all items from the menu. */
    void clear() { menu_->clear(); }
    /** Gets the box type of the menu button */
    fltk3::Box* down_box() const { return (menu_->down_box()); }
    /** Sets the box type of the menu button */
    void down_box(fltk3::Box* b) { menu_->down_box(b); }
    /** Gets the fltk3::MenuItem array used for the menu. */
    const fltk3::MenuItem *menu() { return (menu_->menu()); }
    /** Sets the fltk3::MenuItem array used for the menu. */
    void menu(const fltk3::MenuItem *m) { menu_->menu(m); }
    void resize(int X, int Y, int W, int H) {
      Group::resize(X,Y,W,H);
      inp_->resize(inp_x(), inp_y(), inp_w(), inp_h());
      menu_->resize(menu_x(), menu_y(), menu_w(), menu_h());
    }
    /** Gets the encapsulated input text color attributes */
    fltk3::Color textcolor() const { return (inp_->textcolor());}
    /** Sets the encapsulated input text color attributes */
    void textcolor(fltk3::Color c) { inp_->textcolor(c);}
    /** Gets the encapsulated input text font  attributes */
    fltk3::Font textfont() const { return (inp_->textfont());}
    /** Sets the encapsulated input text font attributes */
    void textfont(fltk3::Font f) { inp_->textfont(f);}
    /** Gets the encapsulated input size attributes */
    fltk3::Fontsize textsize() const { return (inp_->textsize()); }
    /** Sets the encapsulated input size attributes */
    void textsize(fltk3::Fontsize s) { inp_->textsize(s); }
    /** See void fltk3::InputChoice::value(const char *s)  */
    const char* value() const { return (inp_->value()); }
    /**
     Sets or returns the input widget's current contents. The
     second form sets the contents using the index into the menu
     which you can set as an integer.  Setting the value effectively
     'chooses' this menu item, and sets it as the new input text,
     deleting the previous text.
     */
    void value(const char *val) { inp_->value(val); }
    /**    See void fltk3::InputChoice::value(const char *s)  */
    void value(int val) {
      menu_->value(val);
      inp_->value(menu_->text(val));
    }
    /**    Returns a reference to the internal fltk3::MenuButton widget.  */
    fltk3::MenuButton *menubutton() { return menu_; }
    /**
     Returns a reference to the internal fltk3::Input widget.</p>
     */
    fltk3::Input *input() { return inp_; }
  };
  
}

#endif // !Fltk3_Input_Choice_H

//
// End of "$Id$".
//
