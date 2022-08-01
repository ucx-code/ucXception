#!/bin/bash

# Given a root Xen source folder and another folder containting the modified or new files (e.g., L0)
# creates a soft link between the new files and the original Xen folder

XEN_ROOT=$1
EXTRA_ROOT=$2

for file in `find $EXTRA_ROOT -type f -maxdepth 10`; do
	s=${file#"$EXTRA_ROOT"}
	rm ${XEN_ROOT}$s
	ln -v -s ${EXTRA_ROOT}$s ${XEN_ROOT}$s
done
