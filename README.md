# H4bAnalysis

--- checkout analysis code ---
git clone https://github.com/vimartin/H4bAnalysis.git
cd H4bAnalysis

--- checkout and compile fastjet ---
--- note $(WORKDIR) is the current working area ---
curl -O http://fastjet.fr/repo/fastjet-3.1.2.tar.gz
tar -xvzf fastjet-3.1.2.tar.gz
rm -rf fastjet/*
cd fastjet-3.1.2
./configure --prefix=$(WORKDIR)/fastjet/
make install

--- compile and run analysis code ---
make clean
make all
./analysis.exe analysis.cfg
