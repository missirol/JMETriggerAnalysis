#!/bin/bash

set -e

if [ $# -lt 1 ]; then
  printf "\n%s\n\n" ">> argument missing - specify path to output directory"
  exit 1
fi

NEVT=50000

ODIR=$1

if [ -d ${ODIR} ]; then
  echo "output directory already exists: ${ODIR}"
  exit 1
fi

declare -A samplesMap
samplesMap["Phase2HLTTDR_WJetsToLNu_14TeV_PU200"]="/WJetsToLNu_TuneCP5_14TeV-amcatnloFXFX-pythia8/Phase2HLTTDRSummer20ReRECOMiniAOD-PU200_111X_mcRun4_realistic_T15_v1-v1/GEN-SIM-DIGI-RAW-MINIAOD"

recoKeys=(
#  HLT_TRKv00
#  HLT_TRKv00_TICL
#  HLT_TRKv02
#  HLT_TRKv02_TICL
  HLT_TRKv06
  HLT_TRKv06_TICL
#  HLT_TRKv06_skimmedTracks
#  HLT_TRKv06_TICL_skimmedTracks
)

JDIR=${1}_json

if [ ! -d ${JDIR} ]; then
  mkdir -p ${JDIR}
fi

for sample_key in ${!samplesMap[@]}; do
  sample_name=${samplesMap[${sample_key}]}

  if [ ! -f ${JDIR}/${sample_key}.json ]; then
    das_jsondump -v -m ${NEVT} -d ${sample_name} -o ${JDIR}/${sample_key}.json -p 0
  fi

  for reco_key in "${recoKeys[@]}"; do
    htc_driver -c jmeTriggerNTuple_cfg.py -n 100 numThreads=1 --cpus 1 --memory 2000 --runtime 10800 \
      -d ${JDIR}/${sample_key}.json -p 0 \
      -o ${ODIR}/${reco_key}/${sample_key} \
      -m ${NEVT} reco=${reco_key} globalTag=111X_mcRun4_realistic_T15_v1 trkdqm=1 pfdqm=2
  done
  unset -v reco_key
  unset -v sample_name
done
unset -v sample_key

unset NEVT ODIR JDIR
unset recoKeys samplesMap