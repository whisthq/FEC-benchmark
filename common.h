//
// This file contains the parameters which get used for testing the performance of all the FEC libraries in this folder. 
// You can modify the parameters here to test the performance of the FEC libraries under different scenarios.

#pragma once

const int use_sleep=0;
const int use_shuffle=0;
const int segment_size=1280;
const int num_real=100;
const int num_fec=50;
const int iterations=200;

const int num_mem_test_segments=500;  // only for mem test
