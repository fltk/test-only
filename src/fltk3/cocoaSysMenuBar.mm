//
// "$Id$"
//
// MacOS system menu bar widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

/**
 * This code is a quick hack! It was written as a proof of concept.
 * It has been tested on the "menubar" sample program and provides
 * basic functionality. 
 * 
 * To use the System Menu Bar, simply replace the main fltk3::MenuBar
 * in an application with fltk3::SysMenuBar.
 *
 * FLTK features not supported by the Mac System menu
 *
 * - no invisible menu items
 * - no symbolic labels
 * - embossed labels will be underlined instead
 * - no font sizes
 * - Shortcut Characters should be English alphanumeric only, no modifiers yet
 * - no disable main menus
 * - changes to menubar in run-time don't update! 
 *     (disable, etc. - toggle and radio button do!)
 *
 * No care was taken to clean up the menu bar after destruction!
 * ::menu(bar) should only be called once!
 * Many other calls of the parent class don't work.
 * Changing the menu items has no effect on the menu bar.
 * Starting with OS X 10.5, FLTK applications must be created as
 * a bundle for the System Menu Bar (and maybe other features) to work!
 */

#if defined(__APPLE__) || defined(FLTK3_DOXYGEN)

#import <Cocoa/Cocoa.h>

#include <fltk3/x.h>
#include <fltk3/run.h>
#include <fltk3/SysMenuBar.h>

#include "flstring.h"
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

typedef const fltk3::MenuItem *pFl_Menu_Item;

fltk3::SysMenuBar *fltk3::sys_menu_bar = 0;

extern void (*fl_lock_function)();
extern void (*fl_unlock_function)();


@interface FLMenuItem : NSMenuItem 
- (const fltk3::MenuItem*) getFlItem;
- (void) doCallback:(id)unused;
- (void) directCallback:(id)unused;
- (void)setFLKeyEquivalentModifierMask:(unsigned)value;
@end
@implementation FLMenuItem
- (const fltk3::MenuItem*) getFlItem
{
  return *(const fltk3::MenuItem **)[(NSData*)[self representedObject] bytes];
}
- (void) doCallback:(id)unused
{
  fl_lock_function();
  const fltk3::MenuItem *item = [self getFlItem];
  fltk3::sys_menu_bar->picked(item);
  if ( item->flags & fltk3::MENU_TOGGLE ) {	// update the menu toggle symbol
    [self setState:(item->value() ? NSOnState : NSOffState)];
  }
  else if ( item->flags & fltk3::MENU_RADIO ) {	// update the menu radio symbols
    NSMenu* menu = [self menu];
    int flRank = [menu indexOfItem:self];
    int last = [menu numberOfItems] - 1;
    int from = flRank;
    while (from > 0) {
      if ([[menu itemAtIndex:from-1] isSeparatorItem]) break;
      item = [(FLMenuItem*)[menu itemAtIndex:from-1] getFlItem];
      if ( !(item->flags & fltk3::MENU_RADIO) ) break;
      from--;
    }
    int to = flRank;
    while (to < last) {
      if ([[menu itemAtIndex:to+1] isSeparatorItem]) break;
      item = [(FLMenuItem*)[menu itemAtIndex:to+1] getFlItem];
      if (!(item->flags & fltk3::MENU_RADIO)) break;
      to++;
    }
    for (int i =  from; i <= to; i++) {
      NSMenuItem *nsitem = [menu itemAtIndex:i];
      [nsitem setState:(nsitem != self ? NSOffState : NSOnState)];
    }
  }
  fl_unlock_function();
}
- (void) directCallback:(id)unused
{
  fl_lock_function();
  fltk3::MenuItem *item = (fltk3::MenuItem *)[(NSData*)[self representedObject] bytes];
  if ( item && item->callback() ) item->do_callback(NULL);
  fl_unlock_function();
}
- (void)setFLKeyEquivalentModifierMask:(unsigned)value
{
  NSUInteger macMod = 0;
  if ( value & fltk3::META ) macMod = NSCommandKeyMask;
  if ( value & fltk3::SHIFT || isupper(value) ) macMod |= NSShiftKeyMask;
  if ( value & fltk3::ALT ) macMod |= NSAlternateKeyMask;
  if ( value & fltk3::CTRL ) macMod |= NSControlKeyMask;
  [super setKeyEquivalentModifierMask:macMod];
}
@end

// creates a new menu item at the end of 'menu'
// attaches the item of fltk3::sys_menu_bar to it
//  returns the rank (counted in NSMenu) of the new item
static int addNewItem(NSMenu *menu, const fltk3::MenuItem *mitem, NSString* cfname)
{
  FLMenuItem *item = [[FLMenuItem alloc] initWithTitle:cfname 
						action:@selector(doCallback:) 
					 keyEquivalent:@""];
  NSData *pointer = [NSData dataWithBytes:&mitem length:sizeof(fltk3::MenuItem*)];
  [item setRepresentedObject:pointer];
  [menu addItem:item];
  [item setTarget:item];
  [item release];
  return [menu indexOfItem:item];
}

/** 
 * \brief Attaches a callback to the "About myprog" item of the system application menu.
 *
 * \param cb   a callback that will be called by "About myprog" menu item
 *		   with NULL 1st argument.
 * \param user_data   a pointer transmitted as 2nd argument to the callback.
 * \param shortcut    optional shortcut to attach to the "About myprog" menu item (e.g., fltk3::META+'a')
 */
void fl_mac_set_about( fltk3::Callback *cb, void *user_data, unsigned int shortcut) 
{
  fl_open_display();
  fltk3::MenuItem aboutItem;
  memset(&aboutItem, 0, sizeof(fltk3::MenuItem));
  aboutItem.callback(cb);
  aboutItem.user_data(user_data);
  aboutItem.shortcut(shortcut);
  NSMenu *appleMenu = [[[NSApp mainMenu] itemAtIndex:0] submenu];
  CFStringRef cfname = CFStringCreateCopy(NULL, (CFStringRef)[[appleMenu itemAtIndex:0] title]);
  [appleMenu removeItemAtIndex:0];
  FLMenuItem *item = [[[FLMenuItem alloc] initWithTitle:(NSString*)cfname 
						 action:@selector(directCallback:) 
					  keyEquivalent:@""] autorelease];
  if (aboutItem.shortcut()) {
    char sc = aboutItem.shortcut() & 0xff;
    NSString *equiv = [[NSString alloc] initWithBytes:&sc length:1 encoding:NSASCIIStringEncoding];
    [item setKeyEquivalent:equiv];
    [equiv release];
    [item setFLKeyEquivalentModifierMask:aboutItem.shortcut()];
  }
  NSData *pointer = [NSData dataWithBytes:&aboutItem length:sizeof(fltk3::MenuItem)];
  [item setRepresentedObject:pointer];
  [appleMenu insertItem:item atIndex:0];
  CFRelease(cfname);
  [item setTarget:item];
}


static NSString* remove_ampersand(const char *s)
{
  char *ret = strdup(s);
  const char *p = s;
  char *q = ret;
  while(*p != 0) {
    if (p[0]=='&') {
      if (p[1]=='&') {
        *q++ = '&'; p+=2;
      } else {
        p++;
      }
    } else {
      *q++ = *p++;
    }
  }
  *q = 0;
  NSString* rets = [NSString stringWithUTF8String:ret];
  free(ret);
  return rets;
}
 

/*
 * Set a shortcut for an Apple menu item using the FLTK shortcut descriptor.
 */
static void setMenuShortcut( NSMenu* mh, int miCnt, const fltk3::MenuItem *m )
{
  if ( !m->shortcut() ) 
    return;
  if ( m->flags & fltk3::SUBMENU )
    return;
  if ( m->flags & fltk3::SUBMENU_POINTER )
    return;
  char key = m->shortcut() & 0xff;
  if ( !isalnum( key ) )
    return;
  
  FLMenuItem *menuItem = (FLMenuItem*)[mh itemAtIndex:miCnt];
  char sc = m->shortcut() & 0xff;
  NSString *equiv = [[NSString alloc] initWithBytes:&sc length:1 encoding:NSASCIIStringEncoding];
  [menuItem setKeyEquivalent:equiv];
  [equiv release];
  [menuItem setFLKeyEquivalentModifierMask:m->shortcut()];
}


/*
 * Set the Toggle and Radio flag based on FLTK flags
 */
static void setMenuFlags( NSMenu* mh, int miCnt, const fltk3::MenuItem *m )
{
  if ( m->flags & fltk3::MENU_TOGGLE || m->flags & fltk3::MENU_RADIO ) {
    [[mh itemAtIndex:miCnt] setState:(m->flags & fltk3::MENU_VALUE ? NSOnState : NSOffState)];
    }
}


/*
 * create a sub menu for a specific menu handle
 */
static void createSubMenu( NSMenu * mh, pFl_Menu_Item &mm,  const fltk3::MenuItem *mitem )
{
  NSMenu *submenu;
  int miCnt, flags;
  
  NSMenuItem *menuItem;
  submenu = [[NSMenu alloc] initWithTitle:remove_ampersand(mitem->text)];
  [submenu setAutoenablesItems:NO];
  int cnt;
  cnt = [mh numberOfItems] - 1;
  menuItem = [mh itemAtIndex:cnt];
  [menuItem setSubmenu:submenu];
  [submenu release];
  
  while ( mm->text )
  {
    char visible = mm->visible() ? 1 : 0;
    miCnt = addNewItem(submenu, mm, remove_ampersand(mm->label()));
    setMenuFlags( submenu, miCnt, mm );
    setMenuShortcut( submenu, miCnt, mm );
    if ( mm->flags & fltk3::MENU_INACTIVE || mitem->flags & fltk3::MENU_INACTIVE) {
      [[submenu itemAtIndex:miCnt] setEnabled:NO];
    }
    flags = mm->flags;
    if ( mm->flags & fltk3::SUBMENU )
    {
      mm++;
      createSubMenu( submenu, mm, mm - 1 );
    }
    else if ( mm->flags & fltk3::SUBMENU_POINTER )
    {
      const fltk3::MenuItem *smm = (fltk3::MenuItem*)mm->user_data_;
      createSubMenu( submenu, smm, mm );
    }
    if ( flags & fltk3::MENU_DIVIDER ) {
      [submenu addItem:[NSMenuItem separatorItem]];
      }
    if ( !visible ) {
      [submenu removeItem:[submenu itemAtIndex:miCnt]];
    }
    mm++;
  }
}
 

/*
 * convert a complete fltk3::MenuItem array into a series of menus in the top menu bar
 * ALL PREVIOUS SYSTEM MENUS, EXCEPT THE APPLICATION MENU, ARE REPLACED BY THE NEW DATA
 */
static void convertToMenuBar(const fltk3::MenuItem *mm)
{
  int rank;
  int count;//first, delete all existing system menus
  NSMenu* fl_system_menu = [NSApp mainMenu];
  count = [fl_system_menu numberOfItems];
  for(int i = count - 1; i > 0; i--) {
    [fl_system_menu removeItem:[fl_system_menu itemAtIndex:i]];
  }
  //now convert FLTK stuff into MacOS menus
  for (;;)
  {
    if ( !mm || !mm->text )
      break;
    char visible = mm->visible() ? 1 : 0;
    rank = addNewItem(fl_system_menu, mm, remove_ampersand(mm->label()));
   
    if ( mm->flags & fltk3::SUBMENU ) {
      mm++;
      createSubMenu( fl_system_menu, mm, mm - 1);
    }
    else if ( mm->flags & fltk3::SUBMENU_POINTER ) {
      const fltk3::MenuItem *smm = (fltk3::MenuItem*)mm->user_data_;
      createSubMenu( fl_system_menu, smm, mm);
    }
    if ( !visible ) {
      [fl_system_menu removeItem:[fl_system_menu itemAtIndex:rank]];
    }
    mm++;
  }
}


/**
 * @brief create a system menu bar using the given list of menu structs
 *
 * \author Matthias Melcher
 *
 * @param m list of fltk3::MenuItem
 */
void fltk3::SysMenuBar::menu(const fltk3::MenuItem *m) 
{
  fl_open_display();
  fltk3::MenuBar::menu( m );
  convertToMenuBar(m);
}


/**
 * @brief add to the system menu bar a new menu item
 *
 * add to the system menu bar a new menu item, with a title string, shortcut int,
 * callback, argument to the callback, and flags.
 *
 * @see fltk3::Menu_::add(const char* label, int shortcut, fltk3::Callback *cb, void *user_data, int flags) 
 */
int fltk3::SysMenuBar::add(const char* label, unsigned int shortcut, fltk3::Callback *cb, void *user_data, int flags)
{
  fl_open_display();
  int rank = fltk3::Menu_::add(label, shortcut, cb, user_data, flags);
  convertToMenuBar(fltk3::Menu_::menu());
  return rank;
}

/**
 * @brief insert in the system menu bar a new menu item
 *
 * insert in the system menu bar a new menu item, with a title string, shortcut int,
 * callback, argument to the callback, and flags.
 *
 * @see fltk3::Menu_::insert(int index, const char* label, int shortcut, fltk3::Callback *cb, void *user_data, int flags) 
 */
int fltk3::SysMenuBar::insert(int index, const char* label, unsigned int shortcut, fltk3::Callback *cb, void *user_data, int flags)
{
  fl_open_display();
  int rank = fltk3::Menu_::insert(index, label, shortcut, cb, user_data, flags);
  convertToMenuBar(fltk3::Menu_::menu());
  return rank;
}

void fltk3::SysMenuBar::clear()
{
  fltk3::Menu_::clear();
  convertToMenuBar(NULL);
}

int fltk3::SysMenuBar::clear_submenu(int index)
{
  int retval = fltk3::Menu_::clear_submenu(index);
  if (retval != -1) convertToMenuBar(fltk3::Menu_::menu());
  return retval;
}

/**
 * @brief remove an item from the system menu bar
 *
 * @param rank		the rank of the item to remove
 */
void fltk3::SysMenuBar::remove(int rank)
{
  fltk3::Menu_::remove(rank);
  convertToMenuBar(fltk3::Menu_::menu());
}


/**
 * @brief rename an item from the system menu bar
 *
 * @param rank		the rank of the item to rename
 * @param name		the new item name as a UTF8 string
 */
void fltk3::SysMenuBar::replace(int rank, const char *name)
{
  fltk3::Menu_::replace(rank, name);
  convertToMenuBar(fltk3::Menu_::menu());
}


/*
 * Draw the menu bar. 
 * Nothing here because the OS does this for us.
 */
void fltk3::SysMenuBar::draw() {
}


#endif /* __APPLE__ */

//
// End of "$Id$".
//
