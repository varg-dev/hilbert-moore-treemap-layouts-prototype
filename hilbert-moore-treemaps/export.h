
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

bool writeToFile(const Buffer<Rect> & layout, std::ostream & out);
bool writeToSVG(const Buffer<Rect> & layout, const Tree * tree, float layoutScale, bool outlined, SVGColorType colored, std::ostream & out);
