=======================================================
Minimum Survival Guide for porting FLTK1 apps to FLTK2
=======================================================
F. Costantini                               Version 0.1

===============================
About FLTK2 Source Organisation
===============================
1. fltk includes directory
	FL/*.H	-> fltk/*.h

2. fltk include filenames
	a. fltk2 header all have a lowercase .h extension
	b. No fltk2 header should have a Fl_ prefix,
	    if you find the case please report it
	
3. fltk2 namespaces
	a. all fltk widgets and most public functions
		are defined in the fltk namespace.
		For instance, creating a fltk2 Window could be written as follow:
			fltk::Window myWin(300,300);
		Or
		you can use the c++ statement :
			using namespace fltk; 
		before referencing a fltk2 widget,then previous sample could be written:
			Window myWin(300,300);
		Note1: 
			don't use the using statement in include files !
			Usually it's a good idea to reference the namespace 
			after the include files section.
		Note2: 
			In some rare situations, you will have to remove ambiguity
			as with XWindow based unix systems, 
			be careful i.e. when using Button and Window classes
			they will be for sure ambigous after the use of <fltk/x.h> include. 

============================================
About FLTK2 Window creation and event loops
============================================
4. Group/Window  creation / parenting differences
	When creating any fltk2 widget2 that inherits from the Group class,
	be careful not to omit the method:
		myGroupKind.begin();
	This is automatically called in fltk1 in the constructor,
	so in fltk1 you can ommit it but for some reasons, 
	it have NOT been the case on FLTK2.
	
	So to continue with our example let's add one button to our fltk2 window:
		fltk::Window myWin(300,300);
		myWin.begin(); // mandatory on fltk2 don't forget it !
			fltk::Button myButton(100, 130, 80, 30, "&I love fltk2");
		myWin.end(); 

5. Window visibility
	In fltk2 the shown() method is different from fltk1,
	to get similar effect on fltk2 you must use visible() instead of shown().
	(Typically when you are polling window events and you wait in a loop that the user 
	hide/close the window.
	An example of following our example is:
	  myWin.show();
	  while(myWin.visible()) fltk::wait();
	
	For handling the main Window event loop, 
	you won't use this and would preferrably simply use :
	  fltk::run();


TO BE CONTINUED ...
