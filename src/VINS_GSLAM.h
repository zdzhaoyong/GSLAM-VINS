#include <GSLAM/core/GSLAM.h>
#include "VINS.hpp"
#include "feature_tracker.hpp"

// This is the GSLAM interface for VINS-Mobile
class VINSMobile : public GSLAM::SLAM
{

    virtual bool    track(FramePtr &frame);
    FeatureTracker  _tracker;
    VINS            _vins;
};
