#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/ProgressBar.h>

using namespace fltk;

Window* w;

static void ptimer(void *o)
{
	ProgressBar *pbar = (ProgressBar *)o;
	if(pbar->position() < 100)
	{
		pbar->step(1);
		add_timeout(0.1, ptimer, (void *)pbar);
	}
	else
		w->hide();
}

int main(int argc, char **argv) {
  ProgressBar* pbar;
  { Window* o = new Window(400, 100);
    o->begin();
    w = o;
    { ProgressBar* o = new ProgressBar(25, 25, 330, 25, "Simple Progress Bar");
	  pbar = o;
	  //pbar->set_vertical();
      o->box(ENGRAVED_BOX);
#if (MAJOR_VERSION > 1)
	  o->clear_flag(ALIGN_MASK);
	  o->set_flag(ALIGN_BOTTOM);
#else
	  o->align(ALIGN_BOTTOM);
#endif
	  o->selection_color(BLUE);
	  o->color(WHITE);
	  o->textcolor(RED);
    }
    o->end();
  }
  add_timeout(0.1, ptimer, (void *)pbar);
  w->show(argc, argv);

  return run();
}
