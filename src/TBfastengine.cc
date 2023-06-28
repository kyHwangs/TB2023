#include "TBfastengine.h"

TBfastengine::TBfastengine(const YAML::Node fNodePlot_, int fRunNum_, TButility fUtility_)
: fNodePlot(fNodePlot_), fRunNum(fRunNum_), fUtility(fUtility_) {
    init();
}

void TBfastengine::init() {

    fUtility.loading("/Users/khwang/scratch/TB2023July/sample/Info/mapping_Aug2022TB.root");

}