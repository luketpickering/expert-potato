#!/bin/bash

for part in proton pip pim mu; do

  mkdir -p output/${part}
  cd output/${part}

  tsp ../../runone.sh $(pwd)/../.. ${part}

  cd -

done

