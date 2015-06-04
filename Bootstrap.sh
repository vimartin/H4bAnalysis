
# Download the package
#git clone https://github.com/vimartin/H4bAnalysis.git
#cd H4bAnalysis

# compile fastjet
# note $(WORKDIR) is the current working area
curl -O http://fastjet.fr/repo/fastjet-3.1.2.tar.gz
tar -xvzf fastjet-3.1.2.tar.gz
rm -rf fastjet/*
cd fastjet-3.1.2
export WORKDIR=$PWD
./configure --prefix=${WORKDIR}/fastjet/
make install

# Prepare and install the code
cd $WORKDIR
mkdir results/
mkdir data/
scp -r caminal@lxplus.cern.ch:/afs/cern.ch/work/c/caminal/public/ToVerena/ data/.

make clean
make all
