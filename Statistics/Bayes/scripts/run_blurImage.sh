#!/usr/bin/env bash

#name=test1

#imageFile=CamBrowser_20220113.jpg
#imageFile_blurred=work_test1/CamBrowser_20220113_blurred.jpg

name=mug3
imageFile=coffee-mug-1387830_640.jpg
imageFile_blurred=work_${name}/coffee-mug-1387830_640_blurred.jpg

sigmaBlur=2
niterations=200

function run_blur() {
    blurImage.py -i ${imageFile}\
		 -n ${name} \
		 --sigmaBlur ${sigmaBlur} \
		 --width 100 --height 100 --offset 380,100
}

function run_recovery() {
    blurImage.py -i ${imageFile_blurred} \
		 -n ${name} \
		 --recoveryMode \
		 --sigmaBlur ${sigmaBlur} \
		 --niterations ${niterations} \
		 --recordInterval 1000 \
		 --initialCondition original \
		 --scanOrder rowColumn \
		 --proposalSigma 50 --adaptProposalSigma
}

run_blur
run_recovery
