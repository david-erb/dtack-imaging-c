#! /bin/bash

uclient \
-connect rocket,206.251.105.85:5770 \
-input "start?sessid=mysession&streamid=/home/tableau/jpeg/cam001/cam0.%08ld.rgb888.enc&client=rocket,0.0.0.0:0&limit=1111&start=rocket,localhost:80&done=rocket,localhost:80" \
-D reply 
