//
// "$Id$"
//
// X Color Browser demo program for the Fast Light Tool Kit (FLTK).
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


#include <fltk3/run.h>
#include <fltk3/DoubleWindow.h>
#include <fltk3/Button.h>
#include <fltk3/ValueSlider.h>
#include <fltk3/HoldBrowser.h>
#include <fltk3/Box.h>

#include <fltk3/ask.h>
#include <fltk3/filename.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// some constants

#define MAX_RGB 3000

namespace fltk3 {
  const Color FREE_COL4 = ((fltk3::Color)(fltk3::FREE_COLOR+3));
  const Color INDIANRED	= ((fltk3::Color)(164));
}


static fltk3::DoubleWindow *cl;
static fltk3::Box *rescol;
static fltk3::Button *dbobj;
static fltk3::HoldBrowser *colbr;
static fltk3::ValueSlider *rs, *gs, *bs;

static char dbname[FLTK3_PATH_MAX];

static void create_form_cl(void);
static int load_browser(char *);

typedef struct { int r, g, b; } RGBdb;

static RGBdb rgbdb[MAX_RGB];


int main(int argc, char *argv[])
{
  fltk3::args(argc, argv);
  
  create_form_cl();
  
#ifdef USING_XCODE
  // Xcode apps do not set the current directory
  strcpy(dbname, argv[0]);
  char *slash = strrchr(dbname, '/');
  if (slash)
    strcpy(slash, "/../Resources/rgb.txt");
#else
  strcpy(dbname, "rgb.txt");
#endif
  
  if (load_browser(dbname))
    dbobj->label(dbname);
  else
    dbobj->label("None");
  dbobj->redraw();
  
  cl->size_range(cl->w(),cl->h(),2*cl->w(),2*cl->h());
  
  cl->label("RGB Browser");
  cl->free_position();
  cl->show(argc,argv);
  
  return fltk3::run();
}


static void set_entry(int i)
{
  RGBdb *db = rgbdb + i;
  fltk3::set_color(fltk3::FREE_COL4, db->r, db->g, db->b);
  rs->value(db->r);
  gs->value(db->g);
  bs->value(db->b);
  rescol->redraw();
}


static void br_cb(fltk3::Widget *ob, long)
{
  int r = ((fltk3::Browser *)ob)->value();
  
  if (r <= 0)
    return;
  set_entry(r - 1);
}


static int read_entry(FILE * fp, int *r, int *g, int *b, char *name)
{
  int  n;
  char buf[512], *p;
  
  if (!fgets(buf, sizeof(buf) - 1, fp))
    return 0;
  
  if(buf[0] == '!') {
    if (fgets(buf,sizeof(buf)-1,fp)==0) {
      /* ignore */
    }
  }
  
  if(sscanf(buf, " %d %d %d %n", r, g, b, &n) < 3)
    return 0;
  
  p = buf + n;
  
  /* squeeze out all spaces */
  while (*p)
  {
    if (*p != ' ' && *p != '\n')
      *name++ = *p;
    p++;
  }
  *name = 0;
  
  return (feof(fp) || ferror(fp)) ? 0 : 1;
}


static int load_browser(char *fname)
{
  FILE *fp;
  RGBdb *db = rgbdb, *dbs = db + MAX_RGB;
  int r, g, b,  lr  = -1 , lg = -1, lb = -1;
  char name[256], buf[256];
#ifdef __EMX__
  if (!(fp = fltk3::fopen(__XOS2RedirRoot(fname), "r")))
#else
    if (!(fp = fltk3::fopen(fname, "r")))
#endif
    {
      fltk3::alert("%s\n%s\n%s","Load", fname, "Can't open");
      return 0;
    }
  
  /* read the items */
  
  for (; db < dbs && read_entry(fp, &r, &g, &b, name);)
  {
    db->r = r;
    db->g = g;
    db->b = b;
    
    /* unique the entries on the fly */
    if (lr != r || lg != g || lb != b)
    {
      db++;
      lr = r;
      lg = g;
      lb = b;
      sprintf(buf, "(%3d %3d %3d) %s", r, g, b, name);
      colbr->add(buf);
    }
  }
  fclose(fp);
  
  if (db < dbs)
    db->r = 1000;		/* sentinel */
  else
  {
    db--;
    db->r = 1000;
  }
  
  colbr->topline(1);
  colbr->select(1,1);
  set_entry(0);
  
  return 1;
}


static int search_entry(int r, int g, int b)
{
  register RGBdb *db = rgbdb;
  int i, j, diffr, diffg, diffb;
  unsigned int diff, mindiff;
  
  mindiff = (unsigned int)~0;
  for (i = j = 0; db->r < 256; db++, i++)
  {
    diffr = r - db->r;
    diffg = g - db->g;
    diffb = b - db->b;
    
#ifdef FL_LINEAR
    diff = unsigned(3.0 * (fltk3::abs(r - db->r)) +
                    (5.9 * fltk3::abs(g - db->g)) +
                    (1.1 * (fltk3::abs(b - db->b))));
#else
    diff = unsigned(3.0 * (diffr *diffr) +
                    5.9 * (diffg *diffg) +
                    1.1 * (diffb *diffb));
#endif
    
    if (mindiff > diff)
    {
      mindiff = diff;
      j = i;
    }
  }
  
  return j;
}


static void search_rgb(fltk3::Widget *, long)
{
  int r, g, b, i;
  int top  = colbr->topline();
  
  r = int(rs->value());
  g = int(gs->value());
  b = int(bs->value());
  
  // fl_freeze_form(cl);
  fltk3::set_color(fltk3::FREE_COL4, r, g, b);
  rescol->redraw();
  i = search_entry(r, g, b);
  /* change topline only if necessary */
  if(i < top || i > (top+15))
    colbr->topline(i-8);
  colbr->select(i+1, 1);
  // fl_unfreeze_form(cl);
}


/* change database */
static void db_cb(fltk3::Widget * ob, long)
{
  const char *p = fltk3::input("Enter New Database Name", dbname);
  char buf[512];
  
  if (!p || strcmp(p, dbname) == 0)
    return;
  
  strcpy(buf, p);
  if (load_browser(buf))
    strcpy(dbname, buf);
  else
    ob->label(dbname);
}


static void done_cb(fltk3::Widget *, long)
{
  exit(0);
}


static void create_form_cl(void)
{
  if (cl)
    return;
  
  cl = new fltk3::DoubleWindow(400,385);
  cl->box(fltk3::UP_BOX);
  cl->color(fltk3::INDIANRED, fltk3::GRAY);
  
  fltk3::Box *title = new fltk3::Box(40, 10, 300, 30, "Color Browser");
  title->box(fltk3::NO_BOX);
  title->labelcolor(fltk3::RED);
  title->labelsize(32);
  title->labelfont(fltk3::HELVETICA_BOLD);
  title->labeltype(fltk3::SHADOW_LABEL);
  
  dbobj = new fltk3::Button(40, 50, 300, 25, "");
  dbobj->type(fltk3::NORMAL_BUTTON);
  dbobj->box(fltk3::BORDER_BOX);
  dbobj->color(fltk3::INDIANRED,fltk3::INDIANRED);
  dbobj->callback(db_cb, 0);
  
  colbr = new fltk3::HoldBrowser(10, 90, 280, 240, "");
  colbr->textfont(fltk3::COURIER); 
  colbr->callback(br_cb, 0);
  colbr->box(fltk3::DOWN_BOX);
  
  rescol = new fltk3::Box(300, 90, 90, 35, "");
  rescol->color(fltk3::FREE_COL4, fltk3::FREE_COL4);
  rescol->box(fltk3::BORDER_BOX);
  
  rs = new fltk3::ValueSlider(300, 130, 30, 200, "");
  rs->type(fltk3::VERT_FILL_SLIDER);
  rs->color(fltk3::INDIANRED, fltk3::RED);
  rs->bounds(0, 255);
  rs->precision(0);
  rs->callback(search_rgb, 0);
  rs->when(fltk3::WHEN_RELEASE);
  
  gs = new fltk3::ValueSlider(330, 130, 30, 200, "");
  gs->type(fltk3::VERT_FILL_SLIDER);
  gs->color(fltk3::INDIANRED, fltk3::GREEN);
  gs->bounds(0, 255);
  gs->precision(0);
  gs->callback(search_rgb, 1);
  gs->when(fltk3::WHEN_RELEASE);
  
  bs = new fltk3::ValueSlider(360, 130, 30, 200, "");
  bs->type(fltk3::VERT_FILL_SLIDER);
  bs->color(fltk3::INDIANRED, fltk3::BLUE);
  bs->bounds(0, 255);
  bs->precision(0);
  bs->callback(search_rgb, 2);
  bs->when(fltk3::WHEN_RELEASE);
  
  fltk3::Button *done = new fltk3::Button(160, 345, 80, 30, "Done");
  done->type(fltk3::NORMAL_BUTTON);
  done->callback(done_cb, 0);
  
  cl->end();
  cl->resizable(cl);
}

//
// End of "$Id$".
//
