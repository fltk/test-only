#ifndef Fl_Tree_H
#define Fl_Tree_H

class Fl_Node {

  friend class Fl_Tree;

public:

  Fl_Node() {
    prev_ = 0;
    next_ = 0;
    sub_ = 0;
    vsub_ = 0;
    up_ = 0;
    opened_ = 0;
  }

protected:

  Fl_Node* prev_;
  Fl_Node* next_;
  Fl_Node* sub_;
  Fl_Node* vsub_;
  Fl_Node* up_;

  int opened_;

};

#include <fltk/Fl_Widget.h>

class Fl_Pixmap;

enum sort_order {
  NORMAL_SORT = 0,
  REVERSE_SORT = 1
};

class Fl_Tree : public Fl_Widget {

public:

  Fl_Tree(int x, int y, int w, int h, const char* l);

  Fl_Node* first(void) {
    return first_;
  }

  Fl_Node* find(int fy, int& depth, int& ry);

  void traverse_start(Fl_Node * a);
  Fl_Node * traverse_start();
  void traverse_up (void);
  Fl_Node * traverse_forward(int visible, int &depth);
  Fl_Node * traverse_forward();
  Fl_Node * traverse_backward();

  void add_next (Fl_Node* node);
  void add_sub (Fl_Node* node);
  int remove (Fl_Node * a);
  int clear();

  virtual void draw(void);
  void update_height(void);

  int open(Fl_Node* node);
  int close(Fl_Node* node);

  Fl_Node* top() {
    return top_;
  }
protected:

  Fl_Node* first_;
  Fl_Node* top_;
  Fl_Node* t_current_;
  Fl_Node* current_;

  int top_depth_;
  int top_yoffset_;

  Fl_Node* damaged_;

  virtual int height(Fl_Node* node);
  int height_(Fl_Node* node) {
    return height(node) | 1;
    // height must be |1 , so Fl_ToggleTree can do color-swapping
    // with &1 on y coordinate
  }
  int total_height(Fl_Node* node);
  void update_top(void);
  virtual void draw_node(int depth, int cy, Fl_Node* node);
  virtual int handle(int) { return 1; }

  Fl_Node* sort_( Fl_Node* start,
                  int (*compar)(Fl_Node *, Fl_Node *),
                  int down, sort_order order = NORMAL_SORT);

public:
  static int s_compare_(void* a, void *b);
  static int s_compare_reverse_(void* a, void *b);

  Fl_Node* sort(Fl_Node* start, int (*compar)(Fl_Node *, Fl_Node *),
                sort_order order = NORMAL_SORT);
  Fl_Node* sort_tree(Fl_Node* start, int (*compar)(Fl_Node *, Fl_Node *),
                     sort_order order = NORMAL_SORT);

  void sort(int (*compar)(Fl_Node *, Fl_Node *),
            sort_order order = NORMAL_SORT);
  void sort_tree(int (*compar)(Fl_Node *, Fl_Node *),
                 sort_order order = NORMAL_SORT);
};

#endif
