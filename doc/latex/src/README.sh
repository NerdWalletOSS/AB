# sudo apt-get install texlive-full # one time 
touch .meta
eval ` ../tools/setenv `
rm -f *.pdf
# make -f ../tools/docdir.mk template.pdf
make -f ../tools/docdir.mk ab_ui.pdf
lua get_logger_fields.lua > _logger_fields.tex
lua get_endpoints.lua | \
  grep '\/\/' | \
  sed s'/\/\// \& /'g | \
  sed s'/,//'g | \
  sed s'/$/\\\\ \\hline/'g > _endpoints.tex
lua get_logs.lua > _counters.tex
make -f ../tools/docdir.mk ab_rts.pdf
#-- 
lua get_tbl_type.lua > _table_type.tex
make -f ../tools/docdir.mk ab_sql.pdf
#----
make -f ../tools/docdir.mk ab_ops.pdf
