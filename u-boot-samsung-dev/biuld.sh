make distclean
make smdkv210single_config
make
cd sd_fusing/
./sd_fusing.sh /dev/sdb
cd ..
