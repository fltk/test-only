#include <exception>
#include <fltk/run.h>
#include <fltk/Button.h>
#include <fltk/Window.h>
#include <fltk/ask.h>
#include <stdio.h>
using namespace fltk;

class my_exception : public std::exception {
public:
  const char* what() const throw() {return "my exception";}
};

void throwit(Widget*, void*) {
  throw my_exception();
}

int main(int argc, char** argv) {
  Window window(200,100);
  window.begin();
  Button button(25,50,150,24,"Throw Exception");
  button.callback(throwit);
  window.end();
  window.show(argc, argv);
  for (;;) {
    try {
      return run();
    } catch (std::exception& e) {
      message("Exception was thrown!\nIt was \"%s\"", e.what());
    }
  }
}

