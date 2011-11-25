



.PHONY: compile
compile:
	mkdir build -p
	qmake prj/qadoc.pro -o build/Makefile
	make -C build