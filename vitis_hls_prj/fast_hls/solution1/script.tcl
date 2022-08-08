############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
############################################################
open_project fast_hls
set_top fast_accel
add_files fast_hls/sources/fast.h
add_files fast_hls/sources/fast.cpp
add_files -tb fast_hls/sources/fast_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1" -flow_target vivado
set_part {xc7z020-clg400-1}
create_clock -period 10 -name default
config_export -format ip_catalog -output D:/VitisHLSProjects/fast_hls/fast_accel_new.zip -rtl verilog
source "./fast_hls/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog -output D:/VitisHLSProjects/fast_hls/fast_accel_bi.zip
