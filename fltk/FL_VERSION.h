/* The FLTK version number; this is changed slightly from the beta versions
 * because the old "const double" definition would not allow for conditional
 * compilation...

 * FL_VERSION is a float that describes the major, minor, and patch version
 * numbers.  Version 1.1 is actually stored as 1.01 to allow for more than 9
 * minor releases.  Similarly, version 1.0.5 is stored as 1.0005.

 * The FL_MAJOR_VERSION, FL_MINOR_VERSION, and FL_PATCH_VERSION constants
 * give the integral values for the major, minor, and patch releases
 * respectively.
 */

#ifndef FL_VERSION

# define FL_MAJOR_VERSION	2
# define FL_MINOR_VERSION	0
# define FL_PATCH_VERSION	2
# define FL_VERSION		2.0002f

#endif
