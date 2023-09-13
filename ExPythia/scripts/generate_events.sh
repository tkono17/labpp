#!/usr/bin/env zsh

mg_process='p p > j j'
output_dir=output
rootfile=a.root

if [[ $# -eq 3 ]]; then
    mg_process=$1
    output_dir=$2
    root_output=$3
else
    echo "Usage: $0 <mg_process> <output_dir> <root_output>"
    echo "Example: $0 'p p > j j' /tmp/$USER/gen abc.root"
    exit 1
fi

pythia_cmnd=${SWDIR}/labpp/ExPythia/files/main82_mod.cmnd

# 1. Generate events with mg5_aMC
output_mg=${output_dir}/mg
cat <<EOF > mg5_config.txt
generate ${mg_process}
output ${output_mg}
launch -f
  set seed 12345600
  set ptj 100
EOF
mg5_aMC mg5_config.txt

# 2. Uncompress LHE file
lhe_gz=${output_mg}/Events/run_01/unweighted_events.lhe.gz
lhe=${output_mg}/Events/run_01/unweighted_events.lhe
gzip -d ${lhe_gz}

# 3. Run pythia
#pythia_mod82b.exe ${pythia_cmnd} ${lhe} ${root_output}
runPythiaJet.exe ${pythia_cmnd} ${lhe} ${root_output}

# 4. Print summary
echo "MG5_aMC process          : ${mg_process}"
echo "Working directory        : ${output_dir}"
echo "Pythia configuration     : ${pythia_cmnd}"
echo "ROOT output (MG5+Pythia8): ${root_output}"
