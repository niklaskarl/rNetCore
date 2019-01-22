#!/bin/bash

if [ -d ".build" ]; then
	rm -r ".build"
fi

cp -r "package" ".build"
R CMD INSTALL .build
