//	=====================================================================
//	File:    testedit.cx
//	Program: testedi
//	Version: 0.1.
//	Started: 01/03/0
/
//	Copyright (C) 1999 Laurence Charlto
/
//	Description
//	Integrate editing.  DO NOT USE THIS PROGRAM!!!! IT IS ONLY FO
//	INTERNAL USE.  FINAL EDITING MOST LIKELY WILL BEAR NO SIMILARITIE
//	WITH THIS TESTING PROGRAM.  I AM USING IT TO SEE WHAT I CAN DO..
/
//	Notes
//	** Fix callback_when to wor
//	** Fix edit_when to wor
//	** Fix locked to wor
//	** Fix select_locked to wor
//	** Test out of cell editor
//	Test large editors(drop-down
//	Test other editor
//	Fix Flv_List to do editin
//	** Check if we need to set parent for edito
//	=====================================================================

#include <FL/Flv_Table.H
#include <FL/Flve_Input.H
#include <FL/Fl_Double_Window.H
#include <FL/fl_draw.H
#include <FL/Enumerations.H
#include <FL/Flve_Check_Button.H
#include <stdio.h
#include <string.h

char bufs[10][10][40]

char *get_value(int R, int C 

	static char buf[40]
	*buf = 0
	if (R==-1 && C>-1)				//	Row header, A, B, C..
	
		sprintf( buf, "  %c%c  ", C/26 + 'A'-1, (C%26)+'A' )
		if (*(buf+2)<'A'
			memmove( buf+2, buf+3, 4 )
	} else if (C==-1 && R>-1)	//	Column header 1, 2, 3..
		sprintf( buf, "%d", R )
	else if (R>-1 && C>-1)		//	Normal cell from buf
		strcpy(buf, bufs[R & 0x07][C & 0x03])
	return buf


class Flvt_Edit : public Flv_Tabl

public
	Flvt_Edit( int X, int Y, int W, int H, const char *l=0 ) 
		Flv_Table(X,Y,W,H,l) {}
protected
	int handle(int event)
	//	Required for editin
	virtual void save_editor( Fl_Widget *e, int R, int C )
	virtual void load_editor( Fl_Widget *e, int R, int C )
	virtual void position_editor( Fl_Widget *e, int x, int y, int w, int h, Flv_Style &s )

	//	Required for drawin
	virtual void draw_cell( int Offset, int &X, int &Y, int &W, int &H, int R, int C )
}

//	Required for manual editin
int Flvt_Edit::handle(int event

	if (event==FL_KEYBOARD
	
		if (Fl::event_key()==FL_F+2
		
			start_edit()
			return 1
		
	
	return Flv_Table::handle(event)


//	Required for editin
void Flvt_Edit::save_editor( Fl_Widget *e, int R, int C 

	switch( C 
	
		case 1
			switch( R 
			
				case 2
					strncpy( bufs[R][C], (((Flve_Check_Button *)e)->value()?"1":"0"), 40 )
					break
				default
					strncpy( bufs[R&0x7][C], ((Flve_Input *)e)->value() , 40)
					break
			
			break
		default
		   strncpy( bufs[R&0x7][C &0x3], ((Flve_Input *)e)->value() , 40)
		break
	


void Flvt_Edit::load_editor( Fl_Widget *e, int R, int C 

	switch(C
	
		case 1
			switch( R 
			
				case 2
					((Flve_Check_Button *)e)->value( *(bufs[R][C])=='1'?1:0 )
					break
				default
					((Flve_Input *)e)->value( bufs[R][C] )
					((Flve_Input *)e)->position(((Flve_Input *)e)->size(), 0 )
					break
			
			break
	


//	Required for drawin
void Flvt_Edit::draw_cell( int Offset, int &X, int &Y, int &W, int &H, int R, int C 

	Flv_Style s


	get_style(s, R, C)
	Flv_Table::draw_cell(Offset,X,Y,W,H,R,C)
	switch( C 
	
		case 1
			switch( R 
			
				case 2
					draw_flve_check_button( X-Offset-s.x_margin()+1, Y-s.y_margin()+1, W, H
																		(Flve_Check_Button *)s.editor()
																	get_value(R,C) )
					break

				default
					fl_draw( get_value(R,C), X-Offset, Y, W, H, s.align() )
					break
			
			break
		default
			fl_draw(get_value(R,C), X-Offset, Y, W, H, s.align() )
	


void Flvt_Edit::position_editor( Fl_Widget *e, int x, int y, int w, int h, Flv_Style &s 


//	Out of cel
//	e->resize( 10, 10, 200, 20 )

//	In cel
//	Flv_Table::position_editor(e,x+s.x_margin(),y,w-s.x_margin(),h,s)
	Flv_Table::position_editor(e,x,y,w,h,s)


int main(int argc, char **argv

	Fl_Double_Window *w = new Fl_Double_Window( 222, 216, "Test edit" )
		Flvt_Edit *l = new Flvt_Edit( 10, 35, 202, 171, "Sample Editing" )
		Flve_Check_Button *i = new Flve_Check_Button( 0, 0, 0, 0 )
		Flve_Input *j = new Flve_Input( 0, 0, 0, 0 )
	w->end()
	Flv_Style s

	i->hide()
	j->hide()
	i->down_box(FL_DOWN_BOX)
	i->owner = l
	j->owner = l
	l->get_default_style(s)
	i->color( s.background() )
	w->resizable(l)


//	l->edit_when(FLV_EDIT_ALWAYS)
	l->edit_when(FLV_EDIT_AUTOMATIC)
//	l->edit_when(FLV_EDIT_MANUAL)

	l->global_style.editor(j)
	l->row_style[2].editor(i)

	l->select_locked(false)
	l->global_style.font_size(14)
	l->global_style.x_margin(5)
	l->global_style.locked(false)

	l->rows(10000)
	l->cols(100)

    l->col_width(50,-1)
	l->feature(FLVF_DIVIDERS|FLVF_PERSIST_SELECT|FLVF_HEADERS|FLVF_MULTI_SELECT)

	l->global_style.width(100)
	l->global_style.height(15)
	l->global_style.font_size(12)
	j->textsize(12)

	l->col_style[0].align(FL_ALIGN_RIGHT)
	strcpy( bufs[0][0], "Title:  " )
	strcpy( bufs[1][0], "Row header:  " )
	strcpy( bufs[2][0], "Row footer:  " )
	strcpy( bufs[3][0], "Col header:  " )
	strcpy( bufs[4][0], "Col footer:  " )
	strcpy( bufs[5][0], "Cell text:  " )
	strcpy( bufs[6][0], "Row height:  " )
	strcpy( bufs[7][0], "Col width:  " )
	strcpy( bufs[8][0], "Name:  " )
	strcpy( bufs[9][0], "Tag:  " )

	l->row_style[-1].align(FL_ALIGN_CLIP)

	l->col_style[-1].align(FL_ALIGN_CLIP)

	l->col_style[0].locked(true)

	w->show(argc, argv)
	return Fl::run()


