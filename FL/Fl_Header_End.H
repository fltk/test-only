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
#             undef FL_HEADER12
#            else
#             undef FL_HEADER11
#            endif
#           else
#            undef FL_HEADER10
#           endif
#          else
#           undef FL_HEADER9
#          endif
#         else
#          undef FL_HEADER8
#         endif
#        else
#         undef FL_HEADER7
#        endif
#       else
#        undef FL_HEADER6
#       endif
#      else
#       undef FL_HEADER5
#      endif
#     else
#      undef FL_HEADER4
#     endif
#    else
#     undef FL_HEADER3
#    endif
#   else
#    undef FL_HEADER2
#   endif
#  else
#   undef FL_HEADER1
#   undef static
#   undef extern
#  endif
# endif
#endif
