//
// Created by ryanm on 4/12/2016.
//

#include <string>
#include "Exceptions.h"

namespace Gaming {

    Gaming::DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height) {
        __exp_height = expHeight;
        __exp_width = expWidth;
        __width = width;
        __height = height;
        setName("DimensionsEx");
    }


    std::ostream &operator<<(std::ostream &os, const Gaming::GamingException &ex) {
        os << ex.__name << std::endl;
        ex.__print_args(os);
        return os;
    }

    void Gaming::InsufficientDimensionsEx::__print_args(std::ostream &os) const {
        os << "minWidth: " << getExpHeight() << " minHeight: " << getExpHeight() << std::endl;
        os << "width " << getWidth() << " height " << getHeight();
    }

    InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight, unsigned width,
                                                       unsigned height):DimensionEx(minWidth, minHeight, width, height){
        setName("InsufficientDimensionsEx");
    }

    void OutOfBoundsEx::__print_args(std::ostream &os) const {
        os << getWidth() << ' ' << getHeight() << " is out of bounds of " << getExpWidth() <<  ' '  << getExpHeight();
    }

    OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width, unsigned height)
                                :DimensionEx(maxWidth, maxHeight, width, height) {
        setName("OutOfBoundsEx");
    }

    void PositionEx::__print_args(std::ostream &os) const {
        os << __x << ' ' << __y << "is already occupied";
    }

    PositionEx::PositionEx(unsigned x, unsigned y) {
        __x = x;
        __y = y;
        setName("PositionEx");
    }

    PositionNonemptyEx::PositionNonemptyEx(unsigned x, unsigned y):PositionEx(x,y) {
        setName("PositionNonemptyEx");
    }

    PositionEmptyEx::PositionEmptyEx(unsigned x, unsigned y):PositionEx(x,y) {
        setName("PositionEmptyEx");
    }

    void Gaming::PosVectorEmptyEx::__print_args(std::ostream &os) const {

    }

    PosVectorEmptyEx::PosVectorEmptyEx() {
        setName("PosVectorEmptyEx");
    }


}




