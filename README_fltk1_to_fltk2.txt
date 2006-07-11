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
	
3. fltk2 namespaces and file naming conventions
	a. all fltk widgets and most public functions
		are defined in the fltk namespace.
		For instance, creating a fltk2 Window could be written as follow:
			fltk::Window myWin(300,300);
		Or
		you can use the C++ statement :
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
	
	Also concerning fltk files naming :
	all lib names + fltk tools named .*fltk.* are named .*fltk2.*
	As an example fluid is called fluid2, fltk_config is called fltk2_config	
	and libfltk.a is named libfltk2.a

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

==========================================================
Popular fltk Widgets differences in instanciation and use 
==========================================================
6. Button symbols handling
	In fltk2 you can also assign Symbols to your buttons but in different ways from fltk1:
	A Symbol can be an Image or a predefined glyph. All Image classes derives from Symbol.
	a. Assigning a Symbol to a button
			fltk::Button myButton(100, 130, 80, 30, "&I love fltk2");
			myButton.image(new xpmImage(folder_small)); // an xpm image to this button

		A simple example of this is use of image() other default parameters:
			myButton.image(default,disabled, belowmouse, pushedopen);
		This will affect four different image : 
		a default one, an image when widget is inactive,
		an image raising when you put mouse over the widget (hoover-like effect)
		and a pushed(open for Browser trees) image to differentiate 
		when pushing a button or opening a Browser tree node.

		Another (even more powerful) way to achieve event dependent images 
		is to use the MultiImage class. 
		This class permits you to handle more than 4 combinations 
		but requires a bit more work to instanciate.
		see fltk doxygen doc for more details;

		Hints & summary: 
			i.  In fltk2 you can use BOTH text labels and images in the same widget
			ii. In fltk2, you can use multiple symbols/images according to the event type.
			iii. Well known Fl_Pixmap fltk1 class correspond to fltk2 xpmImage.

7. Boxes
	In FLTK2, there is basically no need for an equivalent fltk1 Fl_Box class.
	Instead, all Widget derived classes can use and benefit from a box drawing capability
	but simply using the box() method.
	For more straightforward/simple use of boxes (especially for fltk1 users)
	you may like the     InvisibleBox class.
	Here's an example of use :
	fltk::InvisibleBox b (UP_BOX,0,0,50,50, "My descriptive label in my box");
	This will create a flatbox with a label centered by default inside the box.

8. Browser
	The Browser class in fltk2 is very different from the fltk1 Fl_Browser class.
	
	In fltk2, it also permits to address most tree view needs, and multi-columning.
	(fltk::Browser was first implemented from Marteen de Boer ToggleTree contrib.).
	
	As Browser inherits from Menu, 
	in order to walk the fltk2 browser tree, 
	you 'll use the menu inherited methods like child(), children().
	
	Be careful when indexing browser items, items start at index 0 in fltk2,
	but 0 mean no item in fltk1. 
	
	Though the new fltk2 Browser is quite complex, it can be implemented quite easily 
	(see Browser demo for more details and doxygen documentation containing example of use )
	
	Another hint to port complex browser use 	may be to have a look to 
	the fltk2 FileBrowser port from fltk1 and used in FileChooser in the fltk2 lib.


* TODO next :
	font management differences
	image management differences (what a pain)
	drawing api differences/ similitudes
