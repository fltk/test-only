/
/
// UTF-8 test program for the Fast Light Tool Kit (FLTK)
/
// Copyright 1998-2003 by Bill Spitzak and others
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

#include <FL/Fl.H
#include <FL/Fl_File_Chooser.H
#include <FL/fl_ask.H
#include <FL/Fl_Double_Window.H
#include <FL/Fl_Scroll.H
#include <FL/Fl_Toggle_Button.H
#include <FL/Fl_Choice.H
#include <FL/Fl_Input.H
#include <FL/Fl_Box.H
#include <string.h
#include <stdlib.h
#include <stdio.h
#include <FL/fl_draw.H
#include <FL/fl_math.h
#include <FL/fl_utf8.H


Fl_Scroll* thescroll

void set_foreign(void

  fl_no = "No"
  fl_yes= "Yes"
  fl_ok = "OK"
  fl_cancel= "Cancel"
  fl_close= "Close"
  fl_choose_file = "Choose File"
  fl_show = "Show"
  fl_favorites = "Favorites"
  fl_create_a_new_directory = "Create a new directory."
  fl_preview = "Preview"
  fl_file_name = "Filename:"
  fl_manage_favorites = "Manage Favorites"
  Fl_File_Chooser::add_favorites_label = "Add to Favorites"
  Fl_File_Chooser::all_files_label = "All Files ( *)"
  Fl_File_Chooser::custom_filter_label = "Custom Filter"
  Fl_File_Chooser::existing_file_label = "Please choose an existing file!"
  Fl_File_Chooser::favorites_label = "Favorites"
  Fl_File_Chooser::filename_label = "Filename:"
  Fl_File_Chooser::filesystems_label = "My Computer"
  Fl_File_Chooser::filesystems_label = "File Systems"
  Fl_File_Chooser::manage_favorites_label = "Manage Favorites"
  Fl_File_Chooser::new_directory_label = "New Directory?"
  Fl_File_Chooser::preview_label = "Preview"
  Fl_File_Chooser::show_label = "Show:"
  // fl_unable_to_load_icon = "Fl_File_Icon::load(): Unable to load icon file \"%s\"."
  // fl_unable_to_open_fti = "Fl_File_Icon::load_fti(): Unable to open \"%s\" - %s"
  // fl_expected_letter_fti = "Fl_File_Icon::load_fti(): Expected a letter at file position %ld (saw '%c')"
  // fl_expected_a_fti = "Fl_File_Icon::load_fti(): Expected a ( at file position %ld (saw '%c')"
  // fl_expected_b_fti = "Fl_File_Icon::load_fti(): Expected a ) at file position %ld (saw '%c')"
  // fl_expected_c_fti = "Fl_File_Icon::load_fti(): Expected a ; at file position %ld (saw '%c')"
  // fl_unknown_command_fti = "Fl_File_Icon::load_fti(): Unknown command \"%s\" at file position %ld."
  // fl_gif_unable_to_open = "Fl_GIF_Image: Unable to open %s!"
  // fl_gif_is_not_gif = "Fl_GIF_Image: %s is not a GIF file.\n"
  // fl_gif_is_version = "%s is version %c%c%c."
  // fl_gif_no_colormap = "%s does not have a colormap."
  // fl_gif_unexpected_eof = "Fl_GIF_Image: %s - unexpected EOF"
  // fl_gif_unknown_extension = "%s: unknown gif extension 0x%02x."
  // fl_gif_unknown_code = "%s: unknown gif code 0x%02x"
  // fl_gif_lzw_barf = "Fl_GIF_Image: %s - LZW Barf!"
  // fl_show_previous_help_page = "Show the previous help page."
  // fl_show_next_help_page = "Show the next help page."
  // fl_make_the_help_text_smaller = "Make the help text smaller."
  // fl_make_the_help_text_larger = "Make the help text larger."
  // fl_help_dialog = "Help Dialog"
  fl_untitled = "Untitled"
  fl_no_uri ="<HTML><HEAD><TITLE>Error</TITLE></HEAD>
             "<BODY><H1>Error</H1>
             "<P>Unable to follow the link \"%s\" - 
             "no handler exists for this URI scheme.</P></BODY>"
  fl_unable_to_follow =  "<HTML><HEAD><TITLE>Error</TITLE></HEAD>
               "<BODY><H1>Error</H1>
               "<P>Unable to follow the link \"%s\" - 
               "%s.</P></BODY>"

  // fl_pnm_early_eof = "Early end-of-file in PNM file \"%s\"!"
  fl_ins1_failed = "Fl_Text_Buffer::insert_column(): internal consistency check ins1 failed"
  fl_ovly1_failed = "Fl_Text_Buffer::overlay_rectangle(): internal consistency check ovly1 failed"
  fl_repl1_failed = "Fl_Text_Buffer::replace_rectangular(): internal consistency check repl1 failed"
  fl_cannot_modify_cb = "Fl_Text_Buffer::remove_modify_callback(): Can't find modify CB to remove"
  fl_cannot_find_pre = "Fl_Text_Buffer::remove_predelete_callback(): Can't find pre-delete CB to remove"
  fl_ptvl_failed = "Fl_Text_Display::position_to_line(): Consistency check ptvl failed"
  fl_bad_measurement = "Fl_Text_Display::draw_vline(): bad font measurement"
  fl_error = "Error"
  fl_unknown_color = "Unknown color: %s"
  // fl_xio_error = "X I/O error"
  // fl_cannot_open_display = "Can't open display: %s"
  // fl_requires_true_color = "Requires true color visual"
  fl_cannot_do_scanline = "Can't do scanline_pad of %d"
  fl_cannot_bits_per_pixel = "Can't do %d bits_per_pixel colormap"
  fl_cannot_24bit = "Can't do arbitrary 24bit color"
  fl_cannot_do_bits_per_pixel = "Can't do %d bits_per_pixel"
  fl_cannot_do_delta = "Can't do a delta value of %d"
  fl_cannot_create_pen = "fl_line_style(): Could not create GDI pen object."



void box_cb(Fl_Widget* o, void*) 
  thescroll->box(((Fl_Button*)o)->value() ? FL_DOWN_FRAME : FL_NO_BOX)
  thescroll->redraw()


class My_Input : public Fl_Inpu

public
	My_Input (int x, int y, int w, int h) : Fl_Input(x, y, w, h) {}
	void draw() 
		if (type() == FL_HIDDEN_INPUT) return
		 Fl_Boxtype b = box()
		if (damage() & FL_DAMAGE_ALL) draw_box(b, color())
		drawtext(x()+Fl::box_dx(b)+3, y()+Fl::box_dy(b)
			w()-Fl::box_dw(b)-6, h()-Fl::box_dh(b))
	
	void drawtext(int X, int Y, int W, int H) 
		fl_color(textcolor())
		fl_font(textfont(), textsize())
		fl_rtl_draw(value(), strlen(value()),
			X + (int)fl_width(value()), Y + fl_height() -fl_descent())
	
}


void i7_cb(Fl_Widget *w, void *d

  int i = 0
  char nb[] = "01234567"
  Fl_Input *i7 = (Fl_Input*)w
  Fl_Input *i8 = (Fl_Input*)d
  static char buf[1024]
  const char *ptr = i7->value()
  while (ptr && *ptr) 
     if (*ptr < ' ' || *ptr > 126) 
       buf[i++] = '\\'
       buf[i++] = nb[((*ptr >> 6) & 0x3)]
       buf[i++] = nb[((*ptr >> 3) & 0x7)]
       buf[i++] = nb[(*ptr & 0x7)]
     } else 
       if (*ptr == '\\') buf[i++] = '\\'
       buf[i++] = *ptr
     
     ptr++
  
  buf[i] = 0
  i8->value(buf)


int main(int argc, char** argv) 
  int l
  Fl_Window window(200 + 5*75,400)
  char *latin1 = "ABCabc‡ËÈÔ‚Óˆ¸„123"
  char *utf8 = (char*) malloc(strlen(latin1) * 5 + 1)
  l = 0
  l = fl_latin12utf((const unsigned char*)latin1, strlen(latin1), utf8)
  Fl_Input i1(5, 5, 190, 25)
  utf8[l] = '\0'
  i1.value(utf8)
  Fl_Scroll scroll(200,0,5 * 75,400)
 #ifdef _WIN3
        /* override default fonts *
        Fl::set_font(FL_HELVETICA, " MS Tahoma")
        Fl::set_font(FL_HELVETICA_BOLD,  "BMS Tahoma")
        Fl::set_font(FL_HELVETICA_ITALIC, "IMS Tahoma")
        Fl::set_font(FL_HELVETICA_BOLD_ITALIC, "PMS Tahoma")
        Fl::set_font(FL_COURIER, " Courier New")
        Fl::set_font(FL_COURIER_BOLD, "BCourier New")
        Fl::set_font(FL_COURIER_ITALIC, "ICourier New")
        Fl::set_font(FL_COURIER_BOLD_ITALIC, "PCourier New")
        Fl::set_font(FL_TIMES, " Times New Roman")
        Fl::set_font(FL_TIMES_BOLD, "BTimes New Roman")
        Fl::set_font(FL_TIMES_ITALIC, "ITimes New Roman")
        Fl::set_font(FL_TIMES_BOLD_ITALIC, "PTimes New Roman")
#elif __APPLE_

#els
        /* override default fonts *
        Fl::set_font(FL_HELVETICA
                "-*-helvetica-medium-r-normal--*-iso8859-1")
        Fl::set_font(FL_HELVETICA_BOLD
                "-*-helvetica-bold-r-normal--*-iso8859-2")
        Fl::set_font(FL_HELVETICA_ITALIC
                "-*-helvetica-medium-o-normal--*-iso8859-2")
        Fl::set_font(FL_HELVETICA_BOLD_ITALIC
                "-*-helvetica-bold-o-normal--*-iso8859-2")
        Fl::set_font(FL_COURIER
                "-*-courier-medium-r-normal--*-iso8859-1")
        Fl::set_font(FL_COURIER_BOLD
                "-*-courier-bold-r-normal--*-iso8859-2")
        Fl::set_font(FL_COURIER_ITALIC
                "-*-courier-medium-o-normal--*-iso8859-2")
        Fl::set_font(FL_COURIER_BOLD_ITALIC
                "-*-courier-bold-o-normal--*-iso8859-2")
        Fl::set_font(FL_TIMES
                "-*-times-medium-r-normal--*-iso8859-2")
        Fl::set_font(FL_TIMES_BOLD
                "-*-times-bold-r-normal--*-iso8859-2")
        Fl::set_font(FL_TIMES_ITALIC
                "-*-times-medium-i-normal--*-iso8859-2")
        Fl::set_font(FL_TIMES_BOLD_ITALIC
                "-*-times-bold-i-normal--*-iso8859-2")
#endi

  /* setup an extra font *
  Fl::set_font((Fl_Font)(FL_FREE_FONT)
#ifdef _WIN3
	" MS Gothic
#elif __APPLE_
	" Arial
#els

	"-*-courier-medium-r-normal--*,
	"-*-fixed-medium-*-*--*-*-*-*-*-*-iso8859-15,
	"-*-symbol-*-*-*--*-*-*-*-*-*-adobe-fontspecific,
	"-*-*dingbats-*-*-*--*-*-*-*-*-*-adobe-fontspecific,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-2,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-3,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-4,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-5,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-6,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-7,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-8,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-9,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-13,
	"-*-fixed-*-*-*--*-*-*-*-*-*-iso8859-14,
	"-*-fixed-*-*-*--*-*-*-*-*-*-koi8-*,
	"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0208.1983-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-jisc6226.1978-*,
	"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0201.1976-*,
	"-*-*-*-*-*--*-*-*-*-*-*-ksc5601.1987-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-jisx0208.1990-*,
	"-*-fixed-medium-r-normal--*-*-*-*-*-*-big5*-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-iso646.1991-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-mulearabic-1,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-muleindian-1,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-muleindian-2,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-mulelao-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-tis620.2529-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-is13194-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-mulearabic-2,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-sisheng_cwnn-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-omron_udc_zh-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-muleipa-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-viscii1.1-*,
	//"-*-fixed-*-*-*--*-*-*-*-*-*-gost19768.74-*,
	"-*-unifont-*-*-*--*-*-*-*-*-*-iso10646-1
#endif
  )

  //for (int y=1; y<4095; y++) 
  int off = 2
 
  if (argc > 1) 
	off = (int)strtoul(argv[1], NULL, 0)
	off /= 16
  
  argc = 1
  for (long y=off; y< 0x10000 / 16; y++) 
    int o = 0
    char bu[25]
    char buf[16*6]
    int i = 16 * y;
    for (int x=0; x<16; x++) 
	int l
	l = fl_ucs2utf(i, buf + o)
	if (l < 1) l = 1
	o += l
	i++
    
    buf[o] = '\0'
    sprintf(bu, "0x%04lX", y * 16)
    Fl_Input* b = new Fl_Input(200,(y-off)*25,60,25)
    b->value(strdup(bu))
    b = new Fl_Input(260,(y-off)*25,400,25)
    b->textfont((Fl_Font)(FL_FREE_FONT))
    b->value(strdup(buf))
  
  window.resizable(scroll)
  scroll.end()

  thescroll = &scroll
  char *utf8l = (char*) malloc(strlen(utf8) * 3 + 1)
  Fl_Input i2(5, 35, 190, 25)
  l = fl_utf_tolower((const unsigned char*)utf8, l, utf8l)
  utf8l[l] = '\0'
  i2.value(utf8l)

  char *utf8u = (char*) malloc(strlen(utf8l) * 3 + 1)
  Fl_Input i3(5, 65, 190, 25)
  l = fl_utf_toupper((const unsigned char*)utf8l, l, utf8u)
  utf8u[l] = '\0'
  i3.value(utf8u)

  char *ltr_txt = "\\->e\xCC\x82=\xC3\xAA"
  Fl_Input i4(5, 90, 190, 25)
  i4.value(ltr_txt);
  i4.textfont((Fl_Font)(FL_FREE_FONT))

  xchar arabic[] ={/*8238,*/ 1610, 1608, 1606, 1616, 1603, 1608, 1583, 0}
  char abuf[40]
  l = fl_unicode2utf(arabic, 8, abuf); 
  abuf[l] = 0
  My_Input i5(5, 115, 190, 50)
  i5.textfont((Fl_Font)(FL_FREE_FONT))
  i5.textsize(36)
  i5.value(abuf)
 xchar arabic1[] ={/*8238,*/ 0x5c, 1610, 0x20, 1608, 0x20, 1606, 0x20,  1616, 0x20, 1603, 0x20, 1608, 0x20, 1583, 0}

  l = fl_unicode2utf(arabic1, 15, abuf); 
  abuf[l] = 0
  My_Input i6(5, 175, 190, 50)
  i6.textfont((Fl_Font)(FL_FREE_FONT))
  i6.textsize(30)
  i6.value(abuf)

  Fl_Input i7(5, 230, 190, 25)
  Fl_Input i8(5, 260, 190, 25)
  i7.callback(i7_cb, &i8)
  i7.value(abuf)
  i7.when(FL_WHEN_CHANGED)


  window.end()
  fl_set_status(0, 370, 100, 30);
  window.show(argc,argv)
  return Fl::run()


/
/
