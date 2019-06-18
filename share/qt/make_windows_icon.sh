#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/crca.png
ICON_DST=../../src/qt/res/icons/crca.ico
convert ${ICON_SRC} -resize 16x16 crca-16.png
convert ${ICON_SRC} -resize 32x32 crca-32.png
convert ${ICON_SRC} -resize 48x48 crca-48.png
convert crca-16.png crca-32.png crca-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/crca_testnet.png
ICON_DST=../../src/qt/res/icons/crca_testnet.ico
convert ${ICON_SRC} -resize 16x16 crca-16.png
convert ${ICON_SRC} -resize 32x32 crca-32.png
convert ${ICON_SRC} -resize 48x48 crca-48.png
convert crca-16.png crca-32.png crca-48.png ${ICON_DST}
rm crca-16.png crca-32.png crca-48.png
