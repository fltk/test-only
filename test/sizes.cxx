#include <FL/Fl_Widget.H>
#include <FL/Fl_Browser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Menu_.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Clock.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Valuator.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input_.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Positioner.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Tabs.H>

#include <stdio.h>

main() {
  printf("sizeof(Fl_Menu_Item) = %d\n", sizeof(struct Fl_Menu_Item));
  printf("sizeof(Fl_Widget) = %d\n", sizeof(class Fl_Widget));
  printf("  sizeof(Fl_Box) = %d\n", sizeof(class Fl_Box));
  printf("  sizeof(Fl_Button) = %d\n", sizeof(class Fl_Button));
  printf("    sizeof(Fl_Light_Button) = %d\n", sizeof(class Fl_Light_Button));
  printf("      sizeof(Fl_Check_Button) = %d\n", sizeof(class Fl_Check_Button));
  printf("    sizeof(Fl_Return_Button) = %d\n", sizeof(class Fl_Return_Button));
  printf("  sizeof(Fl_Chart) = %d\n", sizeof(class Fl_Chart));
  printf("  sizeof(Fl_Clock) = %d\n", sizeof(class Fl_Clock));
  printf("  sizeof(Fl_Group) = %d\n", sizeof(class Fl_Group));
  printf("    sizeof(Fl_Browser_) = %d\n", sizeof(class Fl_Browser_));
  printf("      sizeof(Fl_Browser) = %d\n", sizeof(class Fl_Browser));
  printf("    sizeof(Fl_Color_Chooser) = %d\n", sizeof(class Fl_Color_Chooser));
  printf("      sizeof(Fl_Scroll) = %d\n", sizeof(class Fl_Scroll));
  printf("      sizeof(Fl_Tabs) = %d\n", sizeof(class Fl_Tabs));
  printf("    sizeof(Fl_Window) = %d\n", sizeof(class Fl_Window));
  printf("      sizeof(Fl_Double_Window) = %d\n", sizeof(class Fl_Double_Window));
  printf("  sizeof(Fl_Input_) = %d\n", sizeof(class Fl_Input_));
  printf("    sizeof(Fl_Input) = %d\n", sizeof(class Fl_Input));
  printf("    sizeof(Fl_Output) = %d\n", sizeof(class Fl_Output));
  printf("  sizeof(Fl_Menu_) = %d\n", sizeof(class Fl_Menu_));
  printf("    sizeof(Fl_Choice) = %d\n", sizeof(class Fl_Choice));
  printf("    sizeof(Fl_Menu_Bar) = %d\n", sizeof(class Fl_Menu_Bar));
  printf("    sizeof(Fl_Menu_Button) = %d\n", sizeof(class Fl_Menu_Button));
  printf("  sizeof(Fl_Positioner) = %d\n", sizeof(class Fl_Positioner));
  printf("  sizeof(Fl_Roller) = %d\n", sizeof(class Fl_Roller));
  printf("  sizeof(Fl_Valuator) = %d\n", sizeof(class Fl_Valuator));
  printf("    sizeof(Fl_Adjuster) = %d\n", sizeof(class Fl_Adjuster));
  printf("    sizeof(Fl_Counter) = %d\n", sizeof(class Fl_Counter));
  printf("    sizeof(Fl_Dial) = %d\n", sizeof(class Fl_Dial));
  printf("    sizeof(Fl_Slider) = %d\n", sizeof(class Fl_Slider));
  printf("      sizeof(Fl_Scrollbar) = %d\n", sizeof(class Fl_Scrollbar));
  printf("    sizeof(Fl_Value_Input) = %d\n", sizeof(class Fl_Value_Input));
  printf("    sizeof(Fl_Value_Output) = %d\n", sizeof(class Fl_Value_Output));
}
