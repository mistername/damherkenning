#include "Constants.h"

Constants<float> ConstantsValues::DominationMultiplier = Constants<float>("DominationMultiplier", 2.0f);
Constants<int> ConstantsValues::DominationMinimum = Constants<int>("DominationMinimum", 200);
Constants<size_t> ConstantsValues::ImageSize = Constants<size_t>("ImageSize", 500);
Constants<size_t> ConstantsValues::CameraInput = Constants<size_t>("CameraInput", 0);
Constants<size_t> ConstantsValues::port = Constants<size_t>("Port", 8080);
Constants<size_t> ConstantsValues::Verifies = Constants<size_t>("Verifies", 10);
Constants<size_t> ConstantsValues::TileDif = Constants<size_t>("TileDif", 20);