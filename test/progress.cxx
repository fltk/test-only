#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_ProgressBar.H>

  Fl_Window* w;

static void ptimer(void *o)
{
	Fl_ProgressBar *pbar = (Fl_ProgressBar *)o;
	if(pbar->position() < 100)
	{
		pbar->step(1);
		Fl::add_timeout(0.1, ptimer, (void *)pbar);
	}
	else
		w->hide();
}

int main(int argc, char **argv) {
  Fl_ProgressBar* pbar;
  { Fl_Window* o = new Fl_Window(400, 100);
    w = o;
    { Fl_ProgressBar* o = new Fl_ProgressBar(25, 25, 330, 25, "Simple Progress Bar");
	  pbar = o;
      o->box(FL_ENGRAVED_BOX);
#if (FL_MAJOR_VERSION > 1)
	  o->clear_flag(FL_ALIGN_MASK);
	  o->set_flag(FL_ALIGN_BOTTOM);
#else
	  o->align(FL_ALIGN_BOTTOM);
#endif
	  o->selection_color(FL_BLUE);
	  o->color(FL_WHITE);
	  o->textcolor(FL_RED);
    }
    o->end();
  }
  Fl::add_timeout(0.1, ptimer, (void *)pbar);
  w->show(argc, argv);

  return Fl::run();
}
