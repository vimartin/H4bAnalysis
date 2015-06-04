# H4bAnalysis

	--- checkout analysis code ---
	git clone https://github.com/vimartin/H4bAnalysis.git
	cd H4bAnalysis

	--- compile fastjet ---
	--- note $(WORKDIR) is the current working area ---
	rm -rf fastjet/*
	cd fastjet-3.1.2
	./configure --prefix=$(WORKDIR)/fastjet/
	make install

	--- compile and run analysis code ---
	make clean
	make all
	./analysis.exe analysis.cfg
