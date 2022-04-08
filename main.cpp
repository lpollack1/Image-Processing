#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Image.h"
using namespace std;
Image GetData(string fileName){
    // get count
    ifstream file(fileName, ios_base::binary);
    vector <Pixel> Pix;
    Pixel P;
    Header H;

    file.read((char*)&H.idLength, 1);
    file.read((char*)&H.colorMapType, 1);
    file.read((char*)&H.dataTypeCode, 1);
    file.read((char*)&H.colorMapOrigin, 2);
    file.read((char*)&H.colorMapLength, 2);
    file.read((char*)&H.colorMapDepth, 1);
    file.read((char*)&H.xOrigin, 2);
    file.read((char*)&H.yOrigin, 2);
    file.read((char*)&H.width, 2);
    file.read((char*)&H.height, 2);
    file.read((char*)&H.bitsPerPixel, 1);
    file.read((char*)&H.imageDescriptor, 1);

    int size = (int)H.width*(int)H.height;

    for (unsigned int i = 0; i < size; ++i) {
        // GET BLUE, GREEN, RED
        unsigned char blue, green, red;
        file.read((char*)&blue, 1);
        P.blue = blue;
        file.read((char*)&green, 1);
        P.green = green;
        file.read((char*)&red, 1);
        P.red = red;

        Pix.push_back(P);
    }
    return Image(Pix, H);

}

Image GetBlankImage(Header H){
    int size = (int)H.width*(int)H.height;
    vector<Pixel> P(size);
    return Image(P, H);
}
Image GetBlankImage4(Header H){
    int size = (int)H.width*(int)H.height;
    vector<Pixel> P(4*size);
    Header newH;
    newH.idLength = 4*(int)H.idLength;
    newH.colorMapType = 4*(int)H.colorMapType;
    newH.dataTypeCode = 4*(int)H.dataTypeCode;
    newH.colorMapOrigin = 4*(int)H.colorMapOrigin;
    newH.colorMapLength = 4*(int)H.colorMapLength;
    newH.colorMapDepth = 4*(int)H.colorMapDepth;
    newH.xOrigin = 4*(int)H.xOrigin;
    newH.yOrigin = 4*(int)H.yOrigin;
    newH.width = 4*(int)H.width;
    newH.height = 4*(int)H.height;
    newH.bitsPerPixel = 4*(int)H.bitsPerPixel;
    newH.imageDescriptor = 4*(int)H.imageDescriptor;
    return Image(P, H);
}

void Output(string filename, vector<Pixel> &Pixels, Header H){
    ofstream file(filename);
    //outfile << "Typing on readme.txt file...";
    if(file.is_open()){
        file.write((char*)&H.idLength, 1);
        file.write((char*)&H.colorMapType, 1);
        file.write((char*)&H.dataTypeCode, 1);
        file.write((char*)&H.colorMapOrigin, 2);
        file.write((char*)&H.colorMapLength, 2);
        file.write((char*)&H.colorMapDepth, 1);
        file.write((char*)&H.xOrigin, 2);
        file.write((char*)&H.yOrigin, 2);
        file.write((char*)&H.width, 2);
        file.write((char*)&H.height, 2);
        file.write((char*)&H.bitsPerPixel, 1);
        file.write((char*)&H.imageDescriptor, 1);

        for (int i = 0; i < Pixels.size(); ++i) {
            file.write((char*)&Pixels[i].blue, 1);
            file.write((char*)&Pixels[i].green, 1);
            file.write((char*)&Pixels[i].red, 1);

        }

    }
}
Image Multiply(Image &top, Image &bottom){
    unsigned char red = 0, green = 0, blue = 0;
    Image temp = GetBlankImage(bottom.H);

    for (int i = 0; i < top.P.size(); ++i) {
        red = (char)(((top.P[i].red/255.0f)*(bottom.P[i].red/255.0f)*255.0f)+0.5f);
        temp.P[i].red = red;

        green = (char)(((top.P[i].green/255.0f)*(bottom.P[i].green/255.0f)*255.0f)+0.5f);
        temp.P[i].green = green;

        blue = (char)(((top.P[i].blue/255.0f)*(bottom.P[i].blue/255.0f)*255.0f)+0.5f);
        temp.P[i].blue = blue;
    }
    return temp;
}
unsigned char Clamp(float x){
    if(x > 255){
        return 255;
    }
    else if(x < 0){
        return 0;
    }
    else{
        return (unsigned char)x;
    }
}
Image Subtract(Image &top, Image &bottom){
    float red = 0, green = 0, blue = 0;
    Image temp = GetBlankImage(bottom.H);
    for (int i = 0; i < top.P.size(); ++i) {
        red = (float)bottom.P[i].red - (float)top.P[i].red;
        green = (float)bottom.P[i].green - (float)top.P[i].green;
        blue = (float)bottom.P[i].blue - (float)top.P[i].blue;

        temp.P[i].red = Clamp(red);
        temp.P[i].green = Clamp(green);
        temp.P[i].blue = Clamp(blue);

    }
    return temp;
}
Image Screen(Image &top, Image &bottom){
    float red = 0, green = 0, blue = 0;
    Image temp = GetBlankImage(bottom.H);

    for (int i = 0; i < top.P.size(); ++i) {
        red = (1-(1-(top.P[i].red/255.0f))*(1-(bottom.P[i].red/255.0f)))*255.0f+0.5f;
        temp.P[i].red = red;
        green = (1-(1-(top.P[i].green/255.0f))*(1-(bottom.P[i].green/255.0f)))*255.0f+0.5f;
        temp.P[i].green = green;
        blue = (1-(1-(top.P[i].blue/255.0f))*(1-(bottom.P[i].blue/255.0f)))*255.0f+0.5f;
        temp.P[i].blue = blue;
    }
    return temp;
}
Image Overlay(Image &top, Image &bottom){
    unsigned char red = 0, green = 0, blue = 0;
    Image temp = GetBlankImage(bottom.H);
    float r1,g1,b1,r2,b2,g2;
    for (int i = 0; i < top.P.size(); ++i) {
        r1 = (float)top.P[i].red/255.0f;
        g1 = (float)top.P[i].green/255.0f;
        b1 = (float)top.P[i].blue/255.0f;

        r2 = (float)bottom.P[i].red/255.0f;
        g2 = (float)bottom.P[i].green/255.0f;
        b2 = (float)bottom.P[i].blue/255.0f;

        if(r2 <= 0.5){
            red = (2.0f*r1*r2)*255.0f + 0.5f;
        }
        else{
            red = ((1.0f-(2.0f*(1.0f-r1)*(1.0f-r2))))*255.0f + 0.5f;
        }
        temp.P[i].red = red;
        if(g2 <= 0.5){
            green = (2.0f*g1*g2)*255.0f + 0.5f;
        }
        else{
            green = ((1.0f-(2.0f*(1.0f-g1)*(1.0f-g2))))*255.0f + 0.5f;
        }
        temp.P[i].green = green;
        if(b2 <= 0.5){
            blue = (2.0f*b1*b2)*255.0f + 0.5f;
        }
        else{
            blue = ((1.0f-(2.0f*(1.0f-b1)*(1.0f-b2))))*255.0f + 0.5f;
        }
        temp.P[i].blue = blue;
    }
    return temp;
}
Image AddGreen(Image &img){
    Image temp = GetBlankImage(img.H);
    for (int i = 0; i < img.P.size(); ++i) {
        float green = img.P[i].green +200;
        temp.P[i].green = Clamp(green);

        temp.P[i].red = img.P[i].red;
        temp.P[i].blue = img.P[i].blue;
    }
    return temp;
}
Image RBscale(Image &img){
    Image temp = GetBlankImage(img.H);
    for (int i = 0; i < img.P.size(); ++i) {
        float r = (float)img.P[i].red*4;
        unsigned char red = Clamp(r);
        unsigned char blue = 0;
        temp.P[i].red = red;
        temp.P[i].green = img.P[i].green;
        temp.P[i].blue = blue;
    }
    return temp;
}
Image Combine(Image &r, Image &g, Image &b){
    Image temp = GetBlankImage(b.H);
    for (int i = 0; i < g.P.size(); ++i) {
        temp.P[i].red = r.P[i].red;
        temp.P[i].green = g.P[i].green;
        temp.P[i].blue = b.P[i].blue;
    }
    return temp;
}
Image Rotate(Image &img){
    Image temp = GetBlankImage(img.H);
    int place = img.P.size()-1;
    for (int i = 0; i < img.P.size(); ++i) {
        temp.P[place].red = img.P[i].red;
        temp.P[place].green = img.P[i].green;
        temp.P[place].blue = img.P[i].blue;
        place--;
    }
    return temp;
}
Image EC(Image &a, Image &b, Image &c, Image &d){
    Image temp = GetBlankImage4(a.H);
    //cout << temp.P.size() << endl;
    int quarter = temp.P.size()/4;
    for (int i = 0; i < quarter; ++i) {
        temp.P[i].red = a.P[i].red;
        temp.P[i].green = a.P[i].green;
        temp.P[i].blue = a.P[i].blue;
    }
    for (int i = quarter; i < 2*quarter; ++i) {
        temp.P[i].red = b.P[i].red;
        temp.P[i].green = b.P[i].green;
        temp.P[i].blue = b.P[i].blue;
    }
    for (int i = 2*quarter; i < 3*quarter; ++i) {
        temp.P[i].red = c.P[i].red;
        temp.P[i].green = c.P[i].green;
        temp.P[i].blue = c.P[i].blue;
    }for (int i = 3*quarter; i < 4*quarter; ++i) {
        temp.P[i].red = d.P[i].red;
        temp.P[i].green = d.P[i].green;
        temp.P[i].blue = d.P[i].blue;
    }
    return temp;
}
bool Test(Image &Img, Image &ex){
    for (int i = 0; i < Img.P.size(); ++i) {
        if(Img.P[i].blue != ex.P[i].blue){
            return false;
        }
        if(Img.P[i].green != ex.P[i].green){
            return false;
        }
        if(Img.P[i].red != ex.P[i].red){
            return false;
        }
    }
    return true;
}

int main() {
    string car_name = "input/car.tga";
    Image car = GetData(car_name);

    string circles_name = "input/circles.tga";
    Image circles = GetData(circles_name);

    string layer1_name = "input/layer1.tga";
    Image layer1 = GetData(layer1_name);

    string layer2_name = "input/layer2.tga";
    Image layer2 = GetData(layer2_name);

    string layer_blue_name = "input/layer_blue.tga";
    Image layer_blue = GetData(layer_blue_name);

    string layer_green_name = "input/layer_green.tga";
    Image layer_green = GetData(layer_green_name);

    string layer_red_name = "input/layer_red.tga";
    Image layer_red = GetData(layer_red_name);

    string pattern1_name = "input/pattern1.tga";
    Image pattern1 = GetData(pattern1_name);

    string pattern2_name = "input/pattern2.tga";
    Image pattern2 = GetData(pattern2_name);

    string text_name = "input/text.tga";
    Image text = GetData(text_name);

    string text2_name = "input/text2.tga";
    Image text2 = GetData(text2_name);

    Image ex1 = GetData("examples/EXAMPLE_part1.tga");
    Image ex2 = GetData("examples/EXAMPLE_part2.tga");
    Image ex3 = GetData("examples/EXAMPLE_part3.tga");
    Image ex4 = GetData("examples/EXAMPLE_part4.tga");
    Image ex5 = GetData("examples/EXAMPLE_part5.tga");
    Image ex6 = GetData("examples/EXAMPLE_part6.tga");
    Image ex7 = GetData("examples/EXAMPLE_part7.tga");
    Image ex8r = GetData("examples/EXAMPLE_part8_r.tga");
    Image ex8g = GetData("examples/EXAMPLE_part8_g.tga");
    Image ex8b = GetData("examples/EXAMPLE_part8_b.tga");
    Image ex9 = GetData("examples/EXAMPLE_part9.tga");
    Image ex10 = GetData("examples/EXAMPLE_part10.tga");
    Image exEC = GetData("examples/EXAMPLE_extracredit.tga");

    int x=0;
    for (int i = 1; i <= 11; ++i) {
        // test 1- multiply layer1 and pattern1
        if (i == 1) {
            Image img = Multiply(layer1, pattern1);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex1) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part1.tga", img.P, img.H);
        }
        else if (i == 2) {
            Image img = Subtract(layer2, car);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex2) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part2.tga", img.P, img.H);

        }
        else if (i == 3) {
            Image temp = Multiply(layer1, pattern2);
            Image img = Screen(text, temp);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex3) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part3.tga", img.P, img.H);

        }
        else if (i == 4) {
            Image temp = Multiply(layer2, circles);
            Image img = Subtract(pattern2, temp);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex4) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part4.tga", img.P, img.H);
        }
        else if (i == 5) {
            Image img = Overlay(layer1, pattern1);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex5) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }

            Output("output/part5.tga", img.P, img.H);
        }
        else if (i == 6) {
            Image img = AddGreen(car);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex6) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part6.tga", img.P, img.H);
        }
        else if (i == 7) {
            Image img = RBscale(car);
            cout << "Test #" << i << "...... ";
            if(Test(img,ex7) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part7.tga", img.P, img.H);
        }
        else if (i == 8) {
            Image imgR = GetBlankImage(car.H);
            for (int i = 0; i < car.P.size(); ++i) {
                imgR.P[i].red = car.P[i].red;
                imgR.P[i].green = car.P[i].red;
                imgR.P[i].blue = car.P[i].red;
            }
            cout << "Test #8...... ";
            if(Test(imgR,ex8r) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part8_r.tga", imgR.P, imgR.H);

            Image imgG = GetBlankImage(car.H);
            for (int i = 0; i < car.P.size(); ++i) {
                imgG.P[i].red = car.P[i].green;
                imgG.P[i].green = car.P[i].green;
                imgG.P[i].blue = car.P[i].green;
            }
            cout << "Test #9...... ";
            if(Test(imgG,ex8g) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part8_g.tga", imgG.P, imgG.H);
            Image imgB = GetBlankImage(car.H);
            for (int i = 0; i < car.P.size(); ++i) {
                imgB.P[i].red = car.P[i].blue;
                imgB.P[i].green = car.P[i].blue;
                imgB.P[i].blue = car.P[i].blue;
            }
            cout << "Test #10...... ";
            if(Test(imgB,ex8b) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part8_b.tga", imgB.P, imgB.H);

        }
        else if (i == 9) {
            Image img = Combine(layer_red, layer_green, layer_blue);
            cout << "Test #11...... ";
            if(Test(img,ex9) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part9.tga", img.P, img.H);
        }
        else if(i == 10){
            Image img = Rotate(text2);
            cout << "Test #12...... ";
            if(Test(img,ex10) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/part10.tga", img.P, img.H);
        }
        else if(i ==11){
            Image img = layer1;
            cout << "Test #13...... ";
            if(Test(img,exEC) == 1){
                cout << "Passed!" << endl;
                x++;
            } else{
                cout << "Failed!" << endl;
            }
            Output("output/partEC.tga", img.P, img.H);

        }
    }
    cout << "Test results: " << x << " / 13";

}
