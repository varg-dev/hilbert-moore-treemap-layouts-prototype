
#pragma once


#include <functional>

#include "Buffer.h"




class Tree;


class AttributeSumAggregation
{
public:
    AttributeSumAggregation();

    void initialize();
    void deinitialize();

    void process(const Tree * tree, Buffer<float> & values);
};
