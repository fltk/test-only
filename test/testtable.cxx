//	======================================================================
//	File:    testtable.cxx
//	Program: testtable
//	Version: 0.1.0
//	Started: 11/21/99
//
//	Copyright (C) 1999 Laurence Charlton
//
//	Description:
//	Example of using an Flv_Table
//	======================================================================

#include <FL/Flv_Table.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <string.h>
#include <FL/fl_message.H>

const char *get_value(int R, int C )
{
	static char buf[10];
	*buf = 0;
	if (R==-1 && C>-1)				//	Row header, A, B, C...
	{
		sprintf( buf, "%c%c", C/26 + 'A'-1, (C%26)+'A' );
		if (*buf<'A')
			memmove( buf, buf+1, 2 );
	} else if (C==-1 && R>-1)	//	Column header 1, 2, 3...
		sprintf( buf, "%d", R );
	else if (R>-1 && C>-1)		//	Normal cell blank
		*buf = 0;
	return buf;
}

class Flv_Table_Child : public Flv_Table
{
public:
	Flv_Table_Child( int X, int Y, int W, int H, const char *l=0 ) :
		Flv_Table(X,Y,W,H,l) {};
	virtual void draw_cell( int Offset, int &X, int &Y, int &W, int &H, int R, int C );
};

void Flv_Table_Child::draw_cell( int Offset, int &X, int &Y, int &W, int &H, int R, int C )
{
	Flv_Style s;

	get_style(s, R, C);
	Flv_Table::draw_cell(Offset,X,Y,W,H,R,C);
	fl_draw(get_value(R,C), X-Offset, Y, W, H, s.align() );
}

void tbl_cb(Flv_Table_Child *l, void * )
{
	if (l->clicks()>2)
		fl_message( "Why: %d, Clicks: %d", l->why_event(), l->clicks() );
}

int main(int argc, char **argv)
{
	Fl_Double_Window *w = new Fl_Double_Window( 200, 200, "Test table" );
	Flv_Table_Child *l = new Flv_Table_Child( 10, 10, 180, 180, "Sample Spreadsheet View" );
	w->end();
	w->resizable(l);

	l->max_clicks(3);
	l->callback_when( FLVEcb_ROW_HEADER_CLICKED );
	l->callback((Fl_Callback*)tbl_cb);

	l->rows(10);
	l->cols(10);
	l->col_width(50,-1);
	l->feature(FLVF_HEADERS|FLVF_DIVIDERS|FLVF_MULTI_SELECT);

	l->global_style.resizable(true);
  l->global_style.width(20);

	l->row_style[-1].align(FL_ALIGN_CLIP);

	l->col_style[-1].align(FL_ALIGN_CLIP);

	w->show(argc, argv);
	return Fl::run();
}

