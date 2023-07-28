#!/bin/bash

ROOTDIR=${1}
PARTTYPE=${2}

if [ ! -e ${PARTTYPE}_edep.root ]; then
  edep-sim -g ${ROOTDIR}/arbox.gdml -s -u -e 1000000 -o ${PARTTYPE}_edep.root ${ROOTDIR}/macros/${PARTTYPE}_gun.mac
fi

root -l -b -q "${ROOTDIR}/scripts/stoppingdist.C(\"${PARTTYPE}_edep.root\",\"${PARTTYPE}_summary.root\")"
root -l -b -q "${ROOTDIR}/scripts/PSTARLIKE.C(\"${PARTTYPE}_summary.root\",\"${ROOTDIR}/data/PSTAR_ar18.dat\",\"${PARTTYPE}_stop.dat\")"