# dual-readout_TB

FWcore for TB data analysis

## Dependencies
* ROOT
* python3
* boost::python

## Compile
```sh
source buildNinstall.sh
cd test
source compile.sh <analysis code in cpp>

e.g.) source compile.sh TBanalysis
```

## Run
```sh
./TBanalysis <run number>

e.g.) ./TBanalysis 682  #if you want to analyze Run682 (20GeV positron in the center of M2T5 (SiPM tower))
```

## If don't want to use script
### Compile

```sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_102/x86_64-centos7-gcc11-opt/setup.sh # on lxplus for dependencies
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
make -j4 install
```

### Environment setup

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<path_to_install>/lib64 # or lib based on your architecture
export PYTHONPATH=$PYTHONPATH:<path_to_install>/lib64 # or lib based on your architecture
```
