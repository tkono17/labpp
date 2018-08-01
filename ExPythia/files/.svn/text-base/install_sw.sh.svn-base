#---------------------------------------------------------------------
# Software install procedure for phenomenology study
#---------------------------------------------------------------------
#---------------------------------------------------------------------
# o Specify the directories
#   sw_dir: The directory where the libraries and header files are installed.
#   swsrc_dir: The directory where the package source files are copied to.
#---------------------------------------------------------------------
sw_dir=/nfs/space1/tkohno/work/Phenomenology/sw
swsrc_dir=/nfs/space1/tkohno/work/Phenomenology/src
#---------------------------------------------------------------------
# o Specify which software to install
#   Set 'yes' to install, otherwise leave it empty or set string other 
#   than 'yes'.
#---------------------------------------------------------------------
install_lhapdf=
install_fastjet=
install_hepmc=
install_madgraph=
install_pythia8=yes
#---------------------------------------------------------------------

dir0=$(pwd)
mkdir -p ${sw_dir}
mkdir -p ${swsrc_dir}

cd ${swsrc_dir}

# LHAPDF
url=http://www.hepforge.org/archive/lhapdf/LHAPDF-6.1.6.tar.gz
if [[ ${install_lhapdf} == "yes" ]]; then
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

# Fastjet
url=http://fastjet.fr/repo/fastjet-3.2.0.tar.gz
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

# HepMC
url=http://lcgapp.cern.ch/project/simu/HepMC/download/HepMC-2.06.08.tar.gz
if [[ ${install_hepmc} == "yes" ]]; then
    fname=$(basename ${url})
    pname=$(echo ${fname} | sed "s/\.tar\.gz//g")
    bname=hepmc_build
    sname=${swsrc_dir}/${pname}
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

# MadGraph MC@NLO
url=https://launchpad.net/mg5amcnlo/2.0/2.4.0/+download/MG5_aMC_v2.4.0.tar.gz
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

# Pythia8
url=http://home.thep.lu.se/~torbjorn/pythia8/pythia8219.tgz
if [[ ${install_pythia8} == "yes" ]]; then
    fname=$(basename ${url})
    pname=$(echo ${fname} | sed "s/\.tgz//g")
    if [[ ! -e $fname ]]; then
	wget ${url}
    fi
    #tar xvfz ${fname}
    cd ${pname}
    ./configure --prefix=${sw_dir} \
	--enable-shared \
	--with-lhapdf6=${sw_dir} \
	--with-fastjet3=${sw_dir} \
	--with-hepmc2=${sw_dir} \
	--with-root=${ROOTSYS}
    make
    make install
    cd -
fi

cd ${dir0}

unset dir0
unset sw_dir
unset swsrc_dir
unset fname
unset pname

