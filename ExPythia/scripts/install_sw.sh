#!/usr/bin/env zsh
#---------------------------------------------------------------------
# Software install procedure for phenomenology study
#---------------------------------------------------------------------
#---------------------------------------------------------------------
# o Specify the directories
#   proj_dir: The project directory. All work is done under this directory
#             Create two directories (src and sw) under this directory, e.g. 
#             cd ${proj_dir}; mkdir src sw
#---------------------------------------------------------------------
proj_dir=/nfs/space3/tkohno/work/Phenomenology/mg2020
#---------------------------------------------------------------------
sw_dir=${proj_dir}/sw
dev_dir=${proj_dir}/dev
devsrc_dir=${dev_dir}/sources
#---------------------------------------------------------------------
# o Specify which software to install
#   Set 'yes' to install, otherwise leave it empty or set string other 
#   than 'yes'.
#---------------------------------------------------------------------
install_lhapdf=
install_fastjet=
install_hepmc=
install_pythia8=yes
install_madgraph=
#---------------------------------------------------------------------

if [[ $ROOTSYS == "" ]]; then
    echo "Make sure ROOT is not available to install event generator software."
    exit 1
fi

dir0=$(pwd)
mkdir -p ${sw_dir}
mkdir -p ${dev_dir}
mkdir -p ${devsrc_dir}

cd ${devsrc_dir}

# LHAPDF
#url=http://www.hepforge.org/archive/lhapdf/LHAPDF-6.2.1.tar.gz
url=https://lhapdf.hepforge.org/downloads/?f=LHAPDF-6.3.0.tar.gz
if [[ ${install_lhapdf} == "yes" ]]; then
    fname=$(basename ${url})
    fname=$fname[4,-1]
    pname=$(echo ${fname} | sed "s/\.tar\.gz//g")
    if [[ ! -e $fname ]]; then
	curl -o ${fname} ${url}
    fi
    tar xvfz ${fname}
    cd ${pname}
    ./configure --prefix=${sw_dir}
    make
    make install
    cd -
fi

cd ${devsrc_dir}
# Fastjet
#url=http://www.fastjet.fr/repo/fastjet-3.3.1.tar.gz
url=http://fastjet.fr/repo/fastjet-3.3.4.tar.gz
if [[ ${install_fastjet} == "yes" ]]; then
    fname=$(basename ${url})
    pname=$(echo ${fname} | sed "s/\.tar\.gz//g")
    if [[ ! -e $fname ]]; then
	wget ${url}
    fi
    tar xvfz ${fname}
    cd ${pname}
    ./configure --prefix=${sw_dir}
    make
    make install
    cd -
fi

cd ${devsrc_dir}
# HepMC
#url=http://lcgapp.cern.ch/project/simu/HepMC/download/HepMC-2.06.08.tar.gz
#url=http://hepmc.web.cern.ch/hepmc/releases/hepmc3.0.0.tgz
url=https://hepmc.web.cern.ch/hepmc/releases/HepMC3-3.2.2.tar.gz
if [[ ${install_hepmc} == "yes" ]]; then
    fname=$(basename ${url})
    pname=$(echo ${fname} | sed "s/\.tar\.gz//g")
    bname=hepmc_build
    sname=${devsrc_dir}/${pname}
    if [[ ! -e $fname ]]; then
	wget ${url}
    fi
    tar xvfz ${fname}
    mkdir -p ${bname}
    cd ${bname}
    cmake -DCMAKE_INSTALL_PREFIX=${sw_dir} ${sname} \
	-Dmomentum:STRING=GEV -Dlength:STRING=MM
    ./configure --prefix=${sw_dir} 
    make
    make install
    cd -
    unset bname
    unset sname
fi

cd ${devsrc_dir}
# MadGraph MC@NLO
url=https://launchpad.net/mg5amcnlo/2.0/2.6.x/+download/MG5_aMC_v2.6.3.2.tar.gz
if [[ ${install_madgraph} == "yes" ]]; then
    fname=$(basename ${url})
    pname=$(echo ${fname} | sed "s/\.tar\.gz//g" | sed "s/\./_/g")
    if [[ ! -e $fname ]]; then
	wget ${url}
    fi
    tar xvfz ${fname}
    cd ${pname}
    ./bin/mg5_aMC <<EOF
install ExRootAnalysis
quit
EOF
    cd -
    mv ${pname} ${sw_dir}
fi

cd ${devsrc_dir}
# Pythia8
url=http://home.thep.lu.se/~torbjorn/pythia8/pythia8235.tgz
if [[ ${install_pythia8} == "yes" ]]; then
    fname=$(basename ${url})
    pname=$(echo ${fname} | sed "s/\.tgz//g")
    if [[ ! -e $fname ]]; then
	wget ${url}
    fi
    tar xvfz ${fname}
    cd ${pname}
    ./configure --prefix=${sw_dir} \
	--enable-shared \
	--with-lhapdf6=${sw_dir} \
	--with-fastjet3=${sw_dir} \
	--with-hepmc3=${sw_dir} \
	--with-root=${ROOTSYS}
    make
    make install
    cd -
fi

cd ${dir0}

unset dir0
unset sw_dir
unset dev_dir
unset devsrc_dir
unset fname
unset pname

