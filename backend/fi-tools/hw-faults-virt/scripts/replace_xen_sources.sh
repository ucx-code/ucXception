#!/bin/bash

# Given a root Xen source folder and another folder containting the modified or new files (e.g., L0)
# copies the new files to the Xen source folder

XEN_ROOT=$1
EXTRA_ROOT=$2

for file in `find $EXTRA_ROOT -type f -maxdepth 10`; do
	s=${file#"$EXTRA_ROOT"}
	cp -v ${EXTRA_ROOT}$s ${XEN_ROOT}$s

done