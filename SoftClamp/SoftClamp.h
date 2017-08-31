#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MVector.h>
#include <maya/MTypeId.h>


class SoftClamp : public MPxNode {
public:
    SoftClamp();
    virtual	~SoftClamp();
    virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; };
    static void* creator();
    static MStatus initialize();
    virtual MStatus compute( const MPlug &plug, MDataBlock &data );

public:
    static MTypeId id;

    static MObject input;
    static MObject min;
    static MObject minSoft;
    static MObject max;
    static MObject maxSoft;
    static MObject output;
};

