#!/usr/bin/env bash
devdir=/nfs/space3/tkohno/work/Vlq/dev
srcdir=${devdir}/sources

cd ${devdir}/sources

function download() {
    fn=''
    if [[ $# -eq 1 ]]; then
	url=$1
	fn=$(basename ${url})
    elif [[ $# -eq 2 ]]; then
	url=$1
	fn=$2
    else
	echo "$0 <url> [<filename>]"
	exit 1
    fi
    if [[ -e $fn ]]; then
	echo "File $fn exists at ${PWD}"
    else
	curl -g ${url} -o ${fn}
    fi
    # Extract tar.gz
    if [[ -e ${fn} ]]; then
	tar xfz ${fn}
    fi
}

function getSources() {
    # lhapdf
    download https://lhapdf.hepforge.org/downloads/?f=LHAPDF-6.5.4.tar.gz LHAPDF-6.5.4.tar.gz
    
    # HepMC3
    # Installed by dnf
    
    # Pythia8
    download https://pythia.org/download/pythia83/pythia8310.tgz
    
    # FastJet
    download http://fastjet.fr/repo/fastjet-3.4.2.tar.gz
    download http://fastjet.hepforge.org/contrib/downloads/fjcontrib-1.052.tar.gz fjcontrib-1.052

    # labpp
    cd ${devdir}
    if [[ ! -e labpp ]]; then
	git clone https://github.com/tkono17/labpp.git
    fi
}

function compile() {
    cd ${devdir}/sources/LHAPDF-6.5.4
    ./configure --prefix=${SWDIR}
    make install
    
    cd ${devdir}/sources/pythia8310
    ./configure --prefix=${SWDIR}
    make install

    cd ${devdir}/sources/fastjet-3.4.2
   ./configure --prefix=${SWDIR}
    make install

    cd ${devdir}/sources/fjcontrib-1.052
    ./configure --prefix=${SWDIR} --fastjet-contrib=$(which fastjet-contrib)
    make fragile-shared
    make fragile-shared-install
}

#getSources
compile

cd ${devdir}
