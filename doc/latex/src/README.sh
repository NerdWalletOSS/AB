# sudo apt-get install texlive-full # one time 
touch .meta
eval ` ../tools/setenv `
# make -f ../tools/docdir.mk template.pdf
make -f ../tools/docdir.mk ab_ui.pdf
lua get_endpoints.lua | \
  grep '\/\/' | \
  sed s'/\/\// \& /'g | \
  sed s'/,//'g | \
  sed s'/$/\\\\ \\hline/'g > _endpoints.tex
make -f ../tools/docdir.mk ab_rts.pdf
