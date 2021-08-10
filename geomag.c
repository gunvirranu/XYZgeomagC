/*
MIT License

Copyright (c) 2019 Nathan Zimmerberg
Copyright (c) 2021 Gunvir Ranu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "geomag.h"
#include "math.h"

// Model order
#define WMM_NMAX 12

// Number of coefficients
#define WMM_TOT_COEFFS ((WMM_NMAX + 1) * (WMM_NMAX + 2) / 2)

// Epoch of model in decimal year
static const real WMM_EPOCH = 2020;

// Mean radius of ellipsoid
static const real EARTH_R = 6371200.0;

// Declared here, initialized below because the table is hefty
struct WMM_COEFF_SET {
    real main_field_c, main_field_s;
    real sec_var_c, sec_var_s;
};

// Table of coefficients
static const struct WMM_COEFF_SET WMM_COEFFS[WMM_TOT_COEFFS];

static int calc_index(int n, int m) {
    return m * (2 * WMM_NMAX - m + 1) / 2 + n;
}

static real C(int idx, real t_since) {
    return WMM_COEFFS[idx].main_field_c + t_since * WMM_COEFFS[idx].sec_var_c;
}

static real S(int idx, real t_since) {
    return WMM_COEFFS[idx].main_field_s + t_since * WMM_COEFFS[idx].sec_var_s;
}

void geoMag(const real dyear, const real (*pos_itrf)[3], real (*mag_itrf)[3]) {
    const real t = dyear - WMM_EPOCH;
    const real x = (*pos_itrf)[0];
    const real y = (*pos_itrf)[1];
    const real z = (*pos_itrf)[2];
    const real pos_norm_sq = x * x + y * y + z * z;
    const real abf_mul = EARTH_R / pos_norm_sq;
    const real a = abf_mul * x;
    const real b = abf_mul * y;
    const real f = abf_mul * z;
    const real g = abf_mul * EARTH_R;

    real V_top = EARTH_R / REAL_SQRT(pos_norm_sq);
    real W_top = 0;
    real V_prev = 0;
    real W_prev = 0;
    real V_nm = V_top;
    real W_nm = W_top;
    real px = 0, py = 0, pz = 0;

    for (int m = 0; m <= WMM_NMAX + 1; ++m) {
        for (int n = m; n <= WMM_NMAX + 1; ++n) {
            if (m == n) {
                if (m != 0) {
                    real prev_V_top = V_top;
                    V_top = (2 * m - 1) * (a * V_top - b * W_top);
                    W_top = (2 * m - 1) * (a * W_top + b * prev_V_top);
                    V_prev = 0;
                    W_prev = 0;
                    V_nm = V_top;
                    W_nm = W_top;
                }
            } else {
                real prev_V_nm = V_nm;
                real inv_nm = 1.0 / (n - m);
                V_nm = ((2 * n - 1) * f * V_nm - (n + m - 1) * g * V_prev) * inv_nm;
                V_prev = prev_V_nm;
                real prev_W_nm = W_nm;
                W_nm = ((2 * n - 1) * f * W_nm - (n + m - 1) * g * W_prev) * inv_nm;
                W_prev = prev_W_nm;
            }
            if (m < WMM_NMAX && n >= m + 2) {
                int idx = calc_index(n - 1, m + 1);
                real nm_coeff = 0.5 * (n - m) * (n - m - 1);
                px += nm_coeff * (C(idx, t) * V_nm + S(idx, t) * W_nm);
                py += nm_coeff * (-C(idx, t) * W_nm + S(idx, t) * V_nm);
            }
            if (m >= 2 && n >= 2) {
                int idx = calc_index(n - 1, m - 1);
                px += 0.5 * (-C(idx, t) * V_nm - S(idx, t) * W_nm);
                py += 0.5 * (-C(idx, t) * W_nm + S(idx, t) * V_nm);
            }
            if (m == 1 && n >= 2) {
                int idx = calc_index(n - 1, 0);
                px += -C(idx, t) * V_nm;
                py += -C(idx, t) * W_nm;
            }
            if (m < n && n >= 2) {
                int idx = calc_index(n - 1, m);
                pz += (n - m) * (-C(idx, t) * V_nm - S(idx, t) * W_nm);
            }
        }
    }
    // Convert to tesla
    (*mag_itrf)[0] = px * -1e-9;
    (*mag_itrf)[1] = py * -1e-9;
    (*mag_itrf)[2] = pz * -1e-9;
}

static const struct WMM_COEFF_SET WMM_COEFFS[] = {
    // Generated via WMM 2020 COF file
    {                     0.0,                     0.0,                     0.0,                     0.0 },
    {                -29404.5,                     0.0,                     6.7,                     0.0 },
    {                 -2500.0,                     0.0,                   -11.5,                     0.0 },
    {                  1363.9,                     0.0,                     2.8,                     0.0 },
    {                   903.1,                     0.0,                    -1.1,                     0.0 },
    {                  -234.4,                     0.0,                    -0.3,                     0.0 },
    {                    65.9,                     0.0,                    -0.6,                     0.0 },
    {                    80.6,                     0.0,                    -0.1,                     0.0 },
    {                    23.6,                     0.0,                    -0.1,                     0.0 },
    {                     5.0,                     0.0,                    -0.1,                     0.0 },
    {                    -1.9,                     0.0,                     0.0,                     0.0 },
    {                     3.0,                     0.0,                    -0.0,                     0.0 },
    {                    -2.0,                     0.0,                     0.0,                     0.0 },
    {                 -1450.7,                  4652.9,                     7.7,                   -25.1 },
    {       1721.658502723464,     -1727.2010653076843,      -4.099186911246343,     -17.435978129526696 },
    {      -972.0391795944579,      -33.55800947612954,     -2.5311394008759507,       2.327015255644019 },
    {      255.95475381402863,        89.1762300167483,     -0.5059644256269408,      0.0632455532033676 },
    {        93.7520168671942,      12.316087040939586,     0.15491933384829665,    0.025819888974716113 },
    {      14.315093599481099,      -4.167961703507454,    -0.08728715609439695,     0.02182178902359924 },
    {     -14.513835763554324,      -9.713686956337138,    -0.05669467095138408,      0.0944911182523068 },
    {      1.6333333333333333,                     1.4,    0.016666666666666666,   -0.049999999999999996 },
    {       1.222383827699885,     -3.4733589250496735,     -0.0298142396999972,   -0.044721359549995794 },
    {     -0.8360078294544202,      0.4584559064750046,                    -0.0,                    -0.0 },
    {    -0.17232808737106584,                    -0.0,   -0.012309149097933274,                    -0.0 },
    {   -0.011322770341445958,     -0.1358732440973515,                    -0.0,                    -0.0 },
    {       484.0504656885822,      -212.1184889002685,     -0.6350852961085883,      -6.899335716816027 },
    {      159.59273375272028,       31.21624577043178,     0.43893811257017384,    -0.12909944487358055 },
    {       6.424968655349396,      -11.80643892119889,      -0.447213595499958,      0.5142956348249517 },
    {       9.163701684986728,      10.168878760123716,   -0.034156502553198666,     0.12198750911856666 },
    {       2.518739291599593,      0.8625819491779427,    0.017251638983558856,   -0.062105900340811884 },
    {    -0.21345296744756048,    -0.43204937989385733,  -0.0025717224993681985,     0.01543033499620919 },
    {    -0.34860834438919813,     -0.3047832953802704,  -0.0019920476822239894,    0.013944333775567924 },
    {     0.04608402514687029,     0.17639057901043456,                    -0.0,    0.003178208630818641 },
    {   -0.001297498240269205,    -0.00259499648053841,                    -0.0,    0.001297498240269205 },
    {   -0.026989594817970655,    0.028069178610689485,                    -0.0,   0.0010795837927188264 },
    {   0.0045620741787613245,   0.0045620741787613245,                    -0.0,                     0.0 },
    {      27.706822765841952,     -28.613342361756885,     -0.6429964575675704,     0.05797509043642029 },
    {      -6.163395528801023,       3.980111269083531,     0.10757057484009544,     0.07370576424228761 },
    {     -1.4014055444445763,     -1.2081769192688496,   0.0009960238411119947,   -0.008964214570007952 },
    {     -0.6986913788341337,     0.30305379147785055,    0.008050764858994133,   -0.008050764858994133 },
    {      0.2054885133055595,     0.00836501912571304,   0.0025458753860865776,  -0.0025458753860865776 },
    {  -0.0009808164772274995,    0.031386127271279984,   0.0012260205965343744,  -0.0004904082386137498 },
    {    -0.00242739693751473,    0.016991778562603112,   0.0006935419821470658,  -0.0006935419821470658 },
    {    0.002162912891956627,    0.004453055954028349,   0.0002544603402301914,  -0.0003816905103452871 },
    {   0.0023082472415244704,  -0.0004808848419842647,                     0.0,                     0.0 },
    {   0.0009684786719132941,   0.0009684786719132941,                     0.0,   -7.44983593779457e-05 },
    {      0.3373574066791329,     -2.4657375381704476,    -0.03873623667505701,    -0.03944053188733077 },
    {    -0.35496478698597694,     0.07559435278405066,   0.0028171808490950554,   0.0070429521227376385 },
    {   -0.038006427563705626,    -0.06761364461609509,   -0.001469861839480328,    0.000944911182523068 },
    {    0.008663030650303626,     0.01288488735962881,  0.00010965861582662818, -0.00010965861582662818 },
    {   -0.006679356009162114,  -0.0037353744506214664, -3.1655715683232764e-05,  0.00015827857841616382 },
    { -0.00021595245611506707,   -0.001001234114715311, -5.8896124395018286e-05,    7.85281658600244e-05 },
    {  -0.0001156696920770171,   0.0006169050244107579, -1.2852188008557456e-05,  1.2852188008557456e-05 },
    {  -7.901744265512675e-05,  -3.511886340227856e-05,                    -0.0,   1.755943170113928e-05 },
    {   -7.44983593779457e-05, -0.00011174753906691856,                    -0.0,   6.208196614828809e-06 },
    {     0.01017077503944504,     0.07357108075978126,   0.0007423923386456233,  0.00037119616932281165 },
    {   0.0030218361150951764,    0.002014557410063451,                    -0.0,  2.2383971222927232e-05 },
    {   0.0005848459510753503, -0.00020104079568215166,  -4.569108992776174e-05, -0.00010965861582662816 },
    {   0.0006716482625685774,   0.0006540888308674382,   1.755943170113928e-05, -1.3169573775854458e-05 },
    {  -0.0003120815423275714,  -0.0001454816212354092,                    -0.0,   2.346477761861439e-06 },
    {   8.128437404749033e-06, -0.00011650760280140282, -2.7094791349163448e-06, -2.7094791349163448e-06 },
    {   2.488815505973484e-06,   4.977631011946968e-06,  -8.296051686578281e-07,                    -0.0 },
    {  3.7264392750537873e-06,   5.323484678648268e-07,                    -0.0,                    -0.0 },
    {   -0.004180717250887574,    0.004400414911676101,   5.169356724435949e-05,   6.461695905544936e-05 },
    {  -0.0001290349435231273, -0.00048746534219848084, -1.4337215947014143e-05,  3.5843039867535357e-06 },
    {    9.27996603708848e-05,  2.4385312214247102e-05,  3.3868489186454308e-06, -3.3868489186454308e-06 },
    {   3.332218741109649e-06,  2.3628460164232055e-05,   9.087869293935405e-07,                    -0.0 },
    { -1.3631803940903108e-06, -1.5146448823225676e-07,                    -0.0,  1.5146448823225676e-07 },
    {  -5.750020635711086e-07, -1.6428630387745962e-07,                     0.0,                     0.0 },
    {  1.4227611265172995e-07,  3.3197759618736983e-07,                     0.0,                     0.0 },
    {  4.6939331209678796e-05,  -9.100482581468336e-06,  4.7897276744570195e-06,  1.4369183023371058e-06 },
    { -2.0405589067644898e-05,  -8.533246337378777e-06,                     0.0,   4.946809470944218e-07 },
    {   3.891438805883525e-06,  1.7489612610712472e-07,                    -0.0,  -8.744806305356236e-08 },
    {   3.489875760800776e-07,  -7.714462208085927e-07,  -1.836776716210935e-08,                    -0.0 },
    {  -8.658648477055405e-09, -1.4719702410994188e-07,                    -0.0,   8.658648477055405e-09 },
    {  2.2208964739434834e-08,  -4.441792947886967e-09,                    -0.0,                    -0.0 },
    {  -9.275267758020409e-08,    8.65691657415238e-07,  1.2367023677360544e-07,   3.091755919340136e-08 },
    {  -6.973706875519563e-07, -1.1247914315354133e-07,                    -0.0,   3.749304771784711e-08 },
    {   3.499351120332397e-08,  -8.498424149378678e-08,  -4.999073029046282e-09,  -2.499536514523141e-09 },
    {  1.3905011362836212e-08, -1.5891441557527103e-08,   -9.93215097345444e-10,                    -0.0 },
    {  -8.883585895773934e-10,    2.66507576873218e-09,                     0.0,   4.441792947886967e-10 },
    {  -2.103252670898813e-07,  1.7144160426654187e-07,    -7.0697568769708e-09,     3.5348784384854e-09 },
    {   -9.73148077330024e-09,  -4.054783655541767e-10,  -4.054783655541767e-10,   8.109567311083534e-10 },
    {  -7.693411062441365e-10,  -3.846705531220682e-09,  -1.282235177073561e-10,  -1.282235177073561e-10 },
    { -2.4231967148825077e-10,   9.692786859530031e-11,                    -0.0,                    -0.0 },
    {  -3.536041036260129e-09,  -7.978759261304907e-09,                    -0.0,                    -0.0 },
    {   3.957063665233731e-11,  -3.957063665233731e-10, -1.9785318326168656e-11,                     0.0 },
    {    5.96549793142218e-12, -5.3689481382799615e-11,                    -0.0,                    -0.0 },
    {   1.307655652543513e-10, -1.0967434505203656e-10,  -4.218244040462945e-12,                    -0.0 },
    {  -9.675211528915663e-12,                    -0.0,                    -0.0,                     0.0 },
    {  -5.386211681667379e-13,   8.977019469445631e-13, -1.7954038938891263e-13, -1.7954038938891263e-13 }
};
