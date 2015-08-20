
# Download the package
#git clone https://github.com/vimartin/H4bAnalysis.git
#cd H4bAnalysis

# compile fastjet
# note $(WORKDIR) is the current working area
export WORKDIR=$PWD

cp Makefile_lxplus Makefile
asetup 20.1.0.2,here,opt,slc6,64,gcc48
mv HepMC HepMC_local
ln -s /cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc6-gcc48-opt/20.1.0/sw/lcg/releases/LCG_71/HepMC/2.06.09/x86_64-slc6-gcc48-opt HepMC

curl -O http://fastjet.fr/repo/fastjet-3.1.2.tar.gz
tar -xvzf fastjet-3.1.2.tar.gz
rm -rf fastjet/*
cd fastjet-3.1.2
./configure --prefix=${WORKDIR}/fastjet/
make install

# Prepare and install the code
cd $WORKDIR
mkdir -p results/
mkdir -p data/
mkdir -p plots/
cp -r /afs/cern.ch/work/c/caminal/public/ToVerena/data/* data/.

make clean
make all
