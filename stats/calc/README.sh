#!/bin/bash
eval `../latex/tools/setenv`
touch .meta
rm -f ab_stats.pdf ; rm -f _* *.aux *.log *.dvi *.ps *.toc *.lot *.out
# Executes Python script without arguments or config file
# for producing figures for ab_stats.tex
# run script at command line or edit it to see needed arguments
# run script with arguments for examination
# run script with config_file produced from real data, no supplementary figures
touch __init__.py
python calc_ab_stats.py > _py.log
echo "See _py.log for details, or execute ABStats.py from cmd line"
make -f ../latex/tools/docdir.mk ab_stats.pdf > _tex.log
echo "See _tex.log for details, or run latex on ab_stats.tex from cmd line"
