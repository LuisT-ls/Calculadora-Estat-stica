set title 'Box Plot'
set ylabel 'Valores'
set terminal png size 800,400
set output 'boxplot.png'
set style fill solid 0.5
set style boxplot outliers pointtype 7
set style line 1 lt 1 lc rgb '#0060ad' lw 2
set style line 2 lt 1 lc rgb '#dd181f' lw 2
set style line 3 lt 1 lc rgb '#00a94f' lw 2
plot 'dados_boxplot.txt' using (1):1 with boxes linecolor rgb '#00a94f' title 'Valores', '' using 2:2:(sprintf('%.2f', $2)) with labels offset 0,1 textcolor rgb '#0060ad' font ',8' title 'Mediana', '' using 3:3:(sprintf('%.2f', $3)) with labels offset 0,1 textcolor rgb '#dd181f' font ',8' title 'Quartil 1', '' using 4:4:(sprintf('%.2f', $4)) with labels offset 0,1 textcolor rgb '#dd181f' font ',8' title 'Quartil 3', '' using 5:5:(sprintf('%.2f', $5)) with labels offset 0,1 textcolor rgb '#00a94f' font ',8' title 'Limite Inferior', '' using 6:6:(sprintf('%.2f', $6)) with labels offset 0,1 textcolor rgb '#00a94f' font ',8' title 'Limite Superior', '' using (0):(0):(sprintf('%.2f', $1)) with labels offset 0,1 point pointtype 7 lc rgb '#dd181f' font ',8' title 'Outliers'