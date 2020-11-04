#!/usr/bin/env bash

runDir=$SWDIR/../run
#---------------------------------------
# Parameters
runName0=pp_jj
process0="p p > j j"
nevents0=0
ptj0=100
seed0=20201104
#---------------------------------------
if [[ $# -ge 1 ]]; then runName0=$1; fi
if [[ $# -ge 2 ]]; then process0=$2; fi
if [[ $# -ge 3 ]]; then nevents0=$3; fi
if [[ $# -ge 4 ]]; then ptj0=$4; fi
if [[ $# -ge 5 ]]; then seed0=$5; fi
#---------------------------------------
if [[ ${nevents0} -le 0 ]]; then
    echo "Usage: $0 <runName> <mg5_process> <nevents> <ptj> <seed>"
    exit 1
fi

dir0=$(pwd)

process=${process0}
function runMG5() {
    runName=$1
    nevents=$2
    ptj=$3
    seed=$4

    cd ${runDir}

    if [[ -e $runName ]]; then
	rm -fr $runName
    fi
    mgConfigFile=${runName}_config.txt
    
    if [[ -e ${mgConfigFile} ]]; then
	rm ${mgConfigFile}
    fi
    cat <<EOF >> ${mgConfigFile}
import model sm
generate p p > j j
output ${runName}

launch ${runName}
nevents ${nevents}
iseed ${seed}
set ptj ${ptj}
EOF

    mg5_aMC ${mgConfigFile} >& mg5_${runName}.log
    cd ${dir0}
}

function runPJ() {
    runName=$1
    rootFile=$2
    cd ${runDir}

    lheFile=unweighted_events.lhe

    echo hello
    pwd
    cd ${runName}/Events/run_01
    pwd
    if [[ -e ${lheFile} ]]; then
	echo "LHE file found: "
    elif [[ -e ${lheFile}.gz ]]; then
	echo "Ungzip LHE file"
	gzip -d ${lheFile}.gz
    else
	echo "No LHE file found"
    fi

    cd ${runDir}
    lhePath=${runDir}/${runName}/Events/run_01/${lheFile}
    if [[ -e ${lhePath} ]]; then
	echo "Run pj"
	echo runPythiaJet.exe 'no_cmnd' ${lhePath} ${rootFile}
	runPythiaJet.exe 'no_cmnd' ${lhePath} ${rootFile} >& pj_${runName}.log
    fi
    cd ${dir0}
}

process=${process0}
runMG5 ${runName0} ${nevents0} ${ptj0} ${seed0}
runPJ ${runName0} ${runName0}.root


