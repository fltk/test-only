#include <fltk/WidgetAssociation.h>
#include <fltk/Widget.h>

#include <string.h>

using namespace fltk;

/* implementation:
 *   hashtable over the primary key (the widget)
 *   each table entry is for exactly one widget and contains a list
 *     of all associations for that widget
 *   this allows for fast lookup of many widgets, but assumes that not all
 *   that much data is associated with one widget, in case applications come up
 *   with too much data for one widget we need to rectify this
 */

/* instead of a linked list we could use an array here to save some memory
 */
struct Association {
  const AssociationType* at;
  void* data;
  Association* next;
};

struct PrimaryAssociation {
  const Widget* wg;
  Association* associations;
  PrimaryAssociation* next;
};

static PrimaryAssociation** associationTable = 0;
static unsigned int associationTableSize = 0;
static unsigned int associationTableEntries = 0;

static void growTable(void) {

  if (associationTableEntries >= associationTableSize) {

    unsigned int newassociationTableSize;

    // increase table size and rehash
    if (associationTableSize == 0)
      newassociationTableSize = 23;
    else {
      newassociationTableSize = 2 * associationTableSize + 1;
    }

    PrimaryAssociation** newTab = new PrimaryAssociation*[newassociationTableSize];
    memset(newTab, 0, newassociationTableSize*sizeof(PrimaryAssociation*));

    for (unsigned int i = 0; i < associationTableSize; i++)
      while (associationTable[i]) {
        unsigned int hash = (int)associationTable[i]->wg % newassociationTableSize;
        PrimaryAssociation* node = associationTable[i];
        associationTable[i] = node->next;

        node->next = newTab[hash];
        newTab[hash] = node;
      }

    if (associationTableSize)
      delete [] associationTable;

    associationTable = newTab;
    associationTableSize = newassociationTableSize;
  }
}

/*!
 * Add an association to a this widget. The associated data is of the given
 * association type. The associated data must not be NULL. NULL is simply
 * not added as association.
 */
void Widget::add(const AssociationType& at, void* data) {

  if (!data) return;

  growTable();

  unsigned int hash = (int)this % associationTableSize;

  PrimaryAssociation* node = associationTable[hash];

  while (node && node->wg != this) node = node->next;

  if (!node) {

    if (!data) return;

    node = new PrimaryAssociation;
    node->next = associationTable[hash];
    node->wg = this;
    node->associations = 0;
    associationTable[hash] = node;

    associationTableEntries++;
  }

  /* add new association for this type */
  Association* ass = new Association;
  ass->next = node->associations;
  ass->at = &at;
  ass->data = data;
  node->associations = ass;
}


/*!
 * Removes all associations of the given assiciation type from this widget and,
 * if data is not NULL, add the given data pointer as a new association.
 *
 * The removed data is freed by calling the destroy method of the association type.
 */
void Widget::set(const AssociationType& at, void* data) {

  if (associationTableEntries > 0) {

    unsigned int hash = (int)this % associationTableSize;

    PrimaryAssociation* node = associationTable[hash];

    while (node && node->wg != this) node = node->next;

    if (node) {

      /* remove all associations for this associationtype */
      Association* ass = node->associations;
      Association* prevass = 0;

      while (ass) {

        if (ass->at == &at) {

          if (ass == node->associations)
            node->associations = ass->next;
          else
            prevass->next = ass->next;

          ass->at->destroy(ass->data);
          Association* an = ass->next;
          delete ass;
          ass = an;
        } else {
          prevass = ass;
          ass = ass->next;
        }
      }
    }
  }

  add(at, data);
}

/*!
 * Returns the first association of the given type that is connected with this widget.
 * Returns NULL if none.
 */
void* Widget::get(const AssociationType& at) const {

  if (associationTableSize == 0) return 0;

  unsigned int hash = (int)this % associationTableSize;

  PrimaryAssociation* node = associationTable[hash];

  while (node && node->wg != this) node = node->next;

  if (!node) return 0;

  Association* ass = node->associations;

  while (ass && ass->at != &at) ass = ass->next;

  if (!ass) return 0;

  return ass->data;
}

/*!
  Call the functor for each piece of data of the give AssociationType.
  This is a wrapper for ::foreach(&at, this, fkt).
 */
void* Widget::foreach(const AssociationType& at, AssociationFunctor& fkt) const { return fltk::foreach(&at, this, fkt); }

// This is an internal function that is called by ~Widget().
// It destroys all the associations with the widget.
void delete_associations_for(Widget* widget) {

  if (associationTableSize == 0) return;

  unsigned int hash = (int)widget % associationTableSize;

  PrimaryAssociation* node = associationTable[hash];
  PrimaryAssociation* prev = 0;

  while (node && node->wg != widget) {
    prev = node;
    node = node->next;
  }

  if (!node) return;

  while (node->associations) {

    Association* an = node->associations->next;
    node->associations->at->destroy(node->associations->data);
    delete node->associations;
    node->associations = an;
  }

  if (prev)
    prev->next = node->next;
  else
    associationTable[hash] = node->next;

  associationTableEntries--;

  delete node;
}

static void* foreach(const PrimaryAssociation* node, const AssociationType* at, AssociationFunctor& fkt) {

  Association* ass = node->associations;

  while (ass) {

    if (!at || ass->at == at)
      if (fkt.handle(*(ass->at), node->wg, ass->data)) return ass->data;

    ass = ass->next;
  }

  return 0;
}

void* fltk::foreach(const AssociationType* at, const Widget* wg, AssociationFunctor& fkt) {

  if (wg) {

    if (associationTableSize == 0) return 0;

    unsigned int hash = (int)wg % associationTableSize;
    PrimaryAssociation* node = associationTable[hash];

    while (node && node->wg != wg) node = node->next;

    if (!node) return 0;

    return foreach(node, at, fkt);

  } else {

    for (unsigned int i = 0; i < associationTableSize; i++) {

      PrimaryAssociation* node = associationTable[i];

      while (node) {

        void * res = foreach(node, at, fkt);

        if (res) return res;

        node = node->next;
      }
    }
  }

  return 0;
}
