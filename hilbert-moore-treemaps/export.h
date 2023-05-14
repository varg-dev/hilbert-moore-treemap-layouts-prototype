
#pragma once


#include <iosfwd>

#include "Buffer.h"
#include "Rect.h"


enum class SVGColorType : unsigned char
{
    ColorScale,
    GrayScaleDiscriminatibility,
    GrayScaleContinuous
};

enum class PathType
{
    None,
    PerDepth,
    LeavesOnly,
    DepthFirst
};

bool writeToFile(const Buffer<Rect> & layout, std::ostream & out);
bool writeToFile(const Buffer<Rect> & layout, const Tree * tree, std::ostream & out);

template <PathType pathType = PathType::LeavesOnly>
bool writeToSVG(const Buffer<Rect> & layout, const Tree * tree, float layoutScale, bool outlined, SVGColorType colored, std::ostream & out);

#include "export.inl"
