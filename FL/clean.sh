for i in $(ls *.H) $(ls *.h); do
	sed -f clean.sed < $i > tmp
	mv tmp $i
done
