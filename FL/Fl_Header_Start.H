#ifdef FL_DLL
# ifdef FL_HEADER1
#  ifdef FL_HEADER2
#   ifdef FL_HEADER3
#    ifdef FL_HEADER4
#     ifdef FL_HEADER5
#      ifdef FL_HEADER6
#       ifdef FL_HEADER7
#        ifdef FL_HEADER8
#         ifdef FL_HEADER9
#          ifdef FL_HEADER10
#           ifdef FL_HEADER11
#            ifdef FL_HEADER12
#             error "Too much nested FL include files !"
#            endif
#            define FL_HEADER12
#           else
#            define FL_HEADER11
#           endif
#          else
#           define FL_HEADER10
#          endif
#         else
#          define FL_HEADER9
#         endif
#        else
#         define FL_HEADER8
#        endif
#       else
#        define FL_HEADER7
#       endif
#      else
#       define FL_HEADER6
#      endif
#     else
#      define FL_HEADER5
#     endif
#    else
#     define FL_HEADER4
#    endif
#    else
#    define FL_HEADER3
#   endif
#  else
#   define FL_HEADER2
#  endif
# else
#  define FL_HEADER1
#  define FL_IMPORT static __declspec(dllimport) static
#  define FL_IMPORT extern __declspec(dllimport) extern
# endif
#endif

