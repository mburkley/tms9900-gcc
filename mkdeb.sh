#!/bin/bash
rsync -a tms9900-gcc deb/opt
dpkg -b deb tms9900-gcc.deb
