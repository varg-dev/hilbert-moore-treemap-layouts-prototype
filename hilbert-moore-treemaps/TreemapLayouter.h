
#pragma once


#include <glm/vec4.hpp>

#include "WeightPartitioning.h"
#include "RectangularLayoutAlgorithm.h"


class Tree;
class TreeNode;
class Rect;
template <typename T>
class Buffer;


class TreemapLayouter
{
public:
    TreemapLayouter();
    ~TreemapLayouter();

    void initialize();
    void deinitialize();

    void setAlgorithm(RectangularLayoutAlgorithm algorithm);
    void setRootAR(float ar);
    void setTargetAR(float ar);
    void useOrientation(bool orient);

    void process(const Tree * tree, const Buffer<float> & weights,
            Buffer<Rect> & layout);

protected:
    RectangularLayoutAlgorithm m_algorithm;
    float m_rootAR;
    float m_targetAR;
    bool m_useOrientation;

protected:
    void singleNodeProcess(const Tree * tree, const TreeNode * parent, const Buffer<float> & weights, Buffer<Rect> & layout);
};
