#!/usr/bin/env bash

set -euxo pipefail

BASE_URL=https://github.com/nicolov/simple_slam_loop_closure/releases/download/0.0.1

rm -rf data
mkdir data

wget -O data/brief_k10L6.voc.gz "${BASE_URL}/brief_k10L6.voc.gz"

wget -O data/Images.zip "${BASE_URL}/Images.zip"
(cd data && unzip Images.zip)

wget -O data/ImageCollectionCoordinates.txt "${BASE_URL}/ImageCollectionCoordinates.txt"
