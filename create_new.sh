#!/bin/bash

mkdir ../../new_30R_00b 
mkdir ../../new_20R_30b 
mkdir ../../new_18R_30b 
mkdir ../../new_20R_45b 
mkdir ../../new_18R_45b 

./new_catalog.exe ../../old ../../new_30R_00b 40 0
./new_catalog.exe ../../old ../../new_20R_30b 20 30
./new_catalog.exe ../../old ../../new_18R_30b 18 30
./new_catalog.exe ../../old ../../new_20R_45b 20 45
./new_catalog.exe ../../old ../../new_18R_45b 18 45
