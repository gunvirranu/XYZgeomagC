#Nathan Zimmerberg
#24 OCT 2019
#wmmtestgen.py
#This is a simple script to generate c++ catch2 tests.
import pymap3d as pm
import numpy as np


def main(header_to_test, dates, heights, lats, lons, bns, bes, bds, margin):
    """create and write catch2 tests as a c++ source file to test header_to_test
        defining the WMM coefficents and model

     The file created has the name header_to_test[:-4]+'_test.cpp'

    Args:
        header_to_test(str ending in .hpp): the c++ header file with the model and coefficents
        dates(numpy array): decimal test years
        heights(numpy array): test heights (km) with respect to wgs 84 ellipsoid
        lats(numpy array): test lat (deg)
        lons(numpy array): test lon (deg)
        bns(numpy array): test X(local north) magnetic field (nT)
        bes(numpy array): test Y(local east) magnetic field (nT)
        bds(numpy array): test Z(local down) magnetic field (nT)
        margin(float or int): Acceptable error in each component (nT)"""
    xs, ys, zs = pm.geodetic2ecef(lats, lons, heights * 1000)
    bxs, bys, bzs = pm.enu2uvw(bes * 1e-9, bns * 1e-9, -bds * 1e-9, lats, lons)
    file_name = header_to_test[:-2] + '_test.cpp'
    with open(file_name, 'w') as f:
        outstr = """
// %s Generated by python script wmmtestgen.py
>>>>>>> upstream/master
/** \\file
 * \\author Nathan Zimmerberg (nhz2@cornell.edu)
 * \\date 24 OCT 2019
 * \\brief c++ catch2 tests for magnetic field header only library.
 * \\details Compile with g++ %s -std=c++1z
 */
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
extern "C" {
#include "../%s"
}

""" % (file_name, file_name, header_to_test)

        for i in range(len(xs)):
            testcase = """
TEST_CASE( "geomag test %s of WMM2020 model", "[GeoMag]" ) {
    double in[3] = {%s, %s, %s};
    double out[3];
    double truth[3] = {%s, %s, %s};
    geoMag(%s, &in, &out);
    CHECK( abs(out[0]*1E9 - truth[0]*1E9) <= (%s) );
    CHECK( abs(out[1]*1E9 - truth[1]*1E9) <= (%s) );
    CHECK( abs(out[2]*1E9 - truth[2]*1E9) <= (%s) );
}

""" % (
                i,
                repr(xs[i]), repr(ys[i]), repr(zs[i]),
                repr(bxs[i]), repr(bys[i]), repr(bzs[i]),
                dates[i],
                margin, margin, margin
            )
            outstr = outstr + testcase
        f.write(outstr)

if __name__ == '__main__':
    header_to_test = 'geomag.h'
    dates = [2020] * 6 + [2022.5] * 6
    heights = [0] * 3 + [100] * 3 + [0] * 3 + [100] * 3
    lats = [80, 0, -80] * 4
    lons = [0, 120, 240] * 4
    bns = [6570.4,39624.3,5940.6,6261.8,37636.7,5744.9,6529.9,39684.7,6016.5,6224.0,37694.0,5815.0]
    bes = [-146.3,109.9,15772.1,-185.5,104.9,14799.5,1.1,-42.2,15776.7,-44.5,-35.3,14803.0]
    bds = [54606.0,-10932.5,-52480.8,52429.1,-10474.8,-49969.4,54713.4,-10809.5,-52251.6,52527.0,-10362.0,-49755.3]

    margin = 0.1 # nT error acceptable
    main(
        header_to_test,
        np.array(dates),
        np.array(heights),
        np.array(lats),
        np.array(lons),
        np.array(bns),
        np.array(bes),
        np.array(bds),
        margin
    )
