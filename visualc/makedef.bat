echo EXPORTS > fltk.def
nm ../lib/fltkd.lib | sed 's/?/_?/' > tmp
sed 's/_WinMain//' < tmp > tmp2
sed 's/_\$E//' < tmp2 > tmp
sed 's/_??_G//' < tmp > tmp2
grep '^........ [TRDB] _' | sed 's/[^_?]*[_?]//' >> fltk.def < tmp2
del tmp
del tmp2
