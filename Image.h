//
// Created by Lance Pollack on 3/23/22.
//
#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Pixel{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};
struct Header
{
    char  idLength;
    char  colorMapType;
    char  dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char  colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char  bitsPerPixel;
    char  imageDescriptor;
};
class Image{
public:
    Image();
    Image(vector<Pixel> Pix, Header H);
    vector<Pixel> P;
    Header H;
};





