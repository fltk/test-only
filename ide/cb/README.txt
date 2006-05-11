Code::Blocks
============


This directory contains Code::Blocks (http://www.codeblocks.org) workspace
and project files for each specific system-subsystem.
In order to build FLTK inside Code::Blocks you will have to propely setup chosen
subsystems. For example, in order to build "windows-dmc" project you will have
to first setup DigitalMars C/C++ compiler, than activate this specific project, 
and at the end run "Build".

All files in this directory are NOT officialy supported, and serve only the purpose
of helping FLTK team in easier/faster development.


Note for FLTK developers
------------------------

Developers should naturally commit only fltk.workspace, and .cbp files. 
All other files are not needed in our subversion repository, because they 
are automatically generated.

