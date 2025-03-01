build:final

final:src/pdfSniffer.c
	gcc -o final src/pdfSniffer.c `pkg-config --cflags --libs poppler-glib`

