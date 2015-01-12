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
# Tambien convierte de odt a txt usando odt2txt
#
#  sudo easy_install odt2html.py (requiere libxslt y libxml2)
#  sudo apt-get install pandoc / brew install pandoc
#  sudo apt-get install odt2txt / brew install odt2txt
#
###############################################################################
ODTS=$(shell find . -type f -name '*.odt')
HTMLS=$(wildcard *.html)
MDS=$(patsubst %.html,%.md, $(HTMLS))
TXTS=$(patsubst %.odt,%.txt, $(ODTS))

.PHONY : all

all : $(TXTS) #odt2html $(MDS)

# conversion a HTML
odt2html:
	odt2html -a *.odt


# conversión a markdown
%.md : %.html
	pandoc -f html -t markdown -s $< -o $@

# conversión a txt
%.txt : %.odt
	#odt2txt $< > $@
	textutil -convert txt $< -output $@

clean:
	find . -type f -name '*.txt' -exec rm -f {} \;