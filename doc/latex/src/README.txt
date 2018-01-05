sudo apt-get install texlive-full # one time 
touch .meta
eval ` ../tools/setenv `
make -f ../tools/docdir.mk template.pdf
