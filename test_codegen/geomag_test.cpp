// geomag_test.cpp Generated by python script `wmmtestgen.py`

// This tells Catch to provide a main() - only do this in one cpp file
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
    #include "../geomag.h"
}

TEST_CASE( "geomag test 0 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {1111164.8708100126, 0.0, 6259542.961028692};
    double out[3];
    double truth[3] = {-1.5952813250071817e-05, -1.4630000000000003e-07, -5.263547417444184e-05};
    geomag(2020.0, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 1 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-3189068.4999999986, 5523628.670817468, 0.0};
    double out[3];
    double truth[3] = {-5.561426191875908e-06, 9.412872726873376e-06, 3.9624300000000006e-05};
    geomag(2020.0, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 2 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-555582.4354050067, -962297.0059143245, -6259542.961028692};
    double out[3];
    double truth[3] = {6.177267161005053e-06, -2.084485942521248e-05, -5.0651924360034926e-05};
    geomag(2020.0, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 3 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {1128529.6885767058, 0.0, 6358023.736329913};
    double out[3];
    double truth[3] = {-1.5270886859529107e-05, -1.8550000000000001e-07, -5.054523400453757e-05};
    geomag(2020.0, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 4 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-3239068.4999999986, 5610231.211195912, 0.0};
    double out[3];
    double truth[3] = {-5.328246064856985e-06, 9.018992899561239e-06, 3.76367e-05};
    geomag(2020.0, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 5 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-564264.8442883533, -977335.3792323682, -6358023.736329913};
    double out[3];
    double truth[3] = {5.649384308612917e-06, -1.9813979346000043e-05, -4.821266111748948e-05};
    geomag(2020.0, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 6 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {1111164.8708100126, 0.0, 6259542.961028692};
    double out[3];
    double truth[3] = {-1.5931578350356247e-05, 1.1000000000000001e-09, -5.274827527831086e-05};
    geomag(2022.5, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 7 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-3189068.4999999986, 5523628.670817468, 0.0};
    double out[3];
    double truth[3] = {-5.368203727960294e-06, 9.38240160220789e-06, 3.9684699999999996e-05};
    geomag(2022.5, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 8 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-555582.4354050067, -962297.0059143245, -6259542.961028692};
    double out[3];
    double truth[3] = {6.163777504796279e-06, -2.0877424195142713e-05, -5.04130265263596e-05};
    geomag(2022.5, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 9 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {1128529.6885767058, 0.0, 6358023.736329913};
    double out[3];
    double truth[3] = {-1.5250661283058837e-05, -4.45e-08, -5.0648210584673275e-05};
    geomag(2022.5, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 10 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-3239068.4999999986, 5610231.211195912, 0.0};
    double out[3];
    double truth[3] = {-5.150429303246407e-06, 8.991405234014353e-06, 3.7694e-05};
    geomag(2022.5, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}

TEST_CASE( "geomag test 11 of WMM2020 model", "[geomag]" ) {
    const double in[3] = {-564264.8442883533, -977335.3792323682, -6358023.736329913};
    double out[3];
    double truth[3] = {5.6364869232023285e-06, -1.984331827281598e-05, -4.798964104031512e-05};
    geomag(2022.5, &in, &out);
    CHECK( out[0]*1E9 == Approx(truth[0]*1E9).margin(0.1) );
    CHECK( out[1]*1E9 == Approx(truth[1]*1E9).margin(0.1) );
    CHECK( out[2]*1E9 == Approx(truth[2]*1E9).margin(0.1) );
}
