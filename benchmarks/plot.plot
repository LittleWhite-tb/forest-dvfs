set terminal svg font "Ubuntu Mono, 12" linewidth 1 size 1280,768
set output "|cat >./output2.svg"

set style data lines
set xlabel "Time"
set ylabel "Frequency Id chosen"

plot "RESTlog0" using 1:2	


