###############################################################################
#
# SO - Seguridad en Sistemas Operativos
# Grado en Ingeniería Informática
#
# 2014 - Ernesto Serrano <erseco@correo.ugr.es>
# ---------------------------------------------
#
# Makefile para convertir de ficheros odt a ficheros markdown, requiere la
# instalación de los programas odt2html.py y pandoc
# 
#  sudo easy_install odt2html.py (requiere libxslt y libxml2)
#  sudo apt-get install pandoc / brew install pandoc
#
###############################################################################
HTMLS=$(wildcard *.html)
MDS=$(patsubst %.html,%.md, $(HTMLS))

.PHONY : all

all : odt2html $(MDS)

# conversion a HTML
odt2html: 
	odt2html -a *.odt	


# conversión a markdown
%.md : %.html
	pandoc -f html -t markdown -s $< -o $@