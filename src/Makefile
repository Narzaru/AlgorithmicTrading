all: build

build:
	cmake -S . -B build -D CMAKE_CXX_COMPILER=g++ -D CMAKE_CC_COMPILER=gcc
	make -C build A8_Algorithmic_trading -j4

install: build
	make -C build install -j4

uninstall:
	rm -rf install/A8_Algorithmic_trading

clean:
	rm -rf build

dvi:
	cd latex && texi2dvi doc.tex

pdf:
	cd latex && texi2pdf doc.tex

dist:
	git archive --format=tar.gz -o A8_Algorithmic_trading.tar.gz develop


.PHONY: all build install uninstall clean dvi dist
