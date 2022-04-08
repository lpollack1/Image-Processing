//
// Created by Lance Pollack on 3/23/22.
//
#include <iostream>
#include <vector>
#include <string>
#include "Image.h"
using namespace std;

Image::Image(){}

Image::Image(vector<Pixel> P, Header H){
    this->H = H;

    this->P = P;
}
