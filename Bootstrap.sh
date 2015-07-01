
# Download the package
#git clone https://github.com/vimartin/H4bAnalysis.git
#cd H4bAnalysis

# compile fastjet
# note $(WORKDIR) is the current working area
export WORKDIR=$PWD

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
scp -r caminal@lxplus.cern.ch:/afs/cern.ch/work/c/caminal/public/ToVerena/data/* data/.

make clean
make all
