#include "SoftClamp.h"

#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>

#include <maya/MGlobal.h>
#include <cmath>
#include <vector>


namespace illmill {
    const double s = sin(45 * (M_PI / 180.0));
    const double t = tan(22.5 * (M_PI / 180.0));

    double softMax(double v, double mx, double sft) {
        const double r = sft / (s * t);
        if (v < (mx - ((s - t) * r)))
            return v;
        if (v > (mx + (r * t)))
            return mx;
        return mx + (r * (cos(asin(((mx - v) / r) + t)))) - r;
    }

    double softMin(double v, double mn, double sft) {
        const double r = sft / (s * t);
        if (v > (mn + ((s - t) * r)))
            return v;
        if (v < (mn - (r * t)))
            return mn;
        return mn - (r * (cos(asin(((mn - v) / r) - t)))) + r;
    }

    double softClamp(double v, double mn, double mnS, double mx, double mxS) {
        return (mx < mn) ? softMax(softMin(v, mx, mxS), mn, mnS)
                         : softMax(softMin(v, mn, mnS), mx, mxS);
    }
}

// Static Members
MTypeId SoftClamp::id(0x00121BDD);
MObject SoftClamp::input;
MObject SoftClamp::min;
MObject SoftClamp::minSoft;
MObject SoftClamp::max;
MObject SoftClamp::maxSoft;
MObject SoftClamp::output;

SoftClamp::SoftClamp() {}

SoftClamp::~SoftClamp() {}

void* SoftClamp::creator() {
    return new SoftClamp();
}


MStatus SoftClamp::initialize() {
    MStatus stat;
    MFnNumericAttribute fnNum;

    input = fnNum.create("input", "in", MFnNumericData::kDouble, 0.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setKeyable(true);
    addAttribute(input);

    min = fnNum.create("min", "min", MFnNumericData::kDouble, 0.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setKeyable(true);
    addAttribute(min);

    minSoft = fnNum.create("minSoft", "mnsf", MFnNumericData::kDouble, 0.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setKeyable(true);
    addAttribute(minSoft);

    max = fnNum.create("max", "max", MFnNumericData::kDouble, 0.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setKeyable(true);
    addAttribute(max);

    maxSoft = fnNum.create("maxSoft", "mxsf", MFnNumericData::kDouble, 0.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setKeyable(true);
    addAttribute(maxSoft);

    output = fnNum.create("output", "out", MFnNumericData::kDouble, 0.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setWritable(false);
    fnNum.setStorable(false);
    addAttribute(output);

    std::vector<MObject> inAttributes {input, min, minSoft, max, maxSoft};
    for (auto &attr : inAttributes)
        attributeAffects(attr, SoftClamp::output);

    return MS::kSuccess;
}

MStatus SoftClamp::compute(const MPlug &plug, MDataBlock &data) {
    if (plug != output)
        return MS::kUnknownParameter;

    const double in = data.inputValue(input).asDouble();
    const double mn = data.inputValue(min).asDouble();
    const double mnS = data.inputValue(minSoft).asDouble();
    const double mx = data.inputValue(max).asDouble();
    const double mxS = data.inputValue(maxSoft).asDouble();

    const double result = illmill::softClamp(in, mn, mnS, mx, mxS);

    data.outputValue(output).set(result);
    data.setClean(plug);

    return MS::kSuccess;
}



