

#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <cmath>
#include<string>
#include<vector>

using namespace std;

//Function to find MBVQ

string MBVQ(float R, float G, float B){
    if(( R + G) > 255){
        if((G + B) > 255) {
            if ((R + G + B) > 510) {
                return "CMYW";
            } else {
                return "MYGC";
            }
        } else {
            return "RGMY";
        }
    }
    else {
        if(!(G+B) > 255) {
            if (!(R + G + B) > 255) {
                return "KRGB";
            } else {
                return "RGBM";
            }
        } else{
            return "CMGB";
        }
    }
}

string get_vertex(string mbvq, float r, float g, float b){
    string vertex;
    if (mbvq == "CMYW"){
        vertex = 'W';
        if (b < 128){
            if (b <= r){
                if (b <= g){
                    vertex = 'Y';
                }
            }
        }
        if (g < 128){
            if (g <= b){
                if (g <= r){
                    vertex = 'M';
                }
            }
        }
        if (r < 128){
            if (r <= b){
                if ( r<= g){
                    vertex = 'C';
                }
            }
        }
    }
    else if (mbvq == "MYGC") {
        vertex = 'M';
        if (g >= b) {
            if (r >= b) {
                if (r >= 128) {
                    vertex = 'Y';
                } else {
                    vertex = 'G';
                }
            }
        }
        if (g >= r) {
            if (b >= r) {
                if (b >= 128) {
                    vertex = 'C';
                } else {
                    vertex = 'G';
                }
            }
        }
    }
    else if (mbvq == "RGMY") {
        if (b > 128) {
            if (r > 128) {
                if (b >= g) {
                    vertex = 'M';
                } else {
                    vertex = 'Y';
                }
            } else {
                if (g > b + r) {
                    vertex = 'G';
                } else {
                    vertex = 'M';
                }
            }
        } else {
            if (r >= 128) {
                if (g >= 128) {
                    vertex = 'Y';
                } else {
                    vertex = 'R';
                }
            } else {
                if (r >= g) {
                    vertex = 'R';
                } else {
                    vertex = 'G';
                }
            }
        }
    }
    else if (mbvq == "KRGB") {
        vertex = 'K';
        if (b > 128) {
            if (b >= r) {
                if (b >= g) {
                    vertex = 'B';
                }
            }
        }
        if (g > 128) {
            if (g >= b) {
                if (g >= r) {
                    vertex = 'G';
                }
            }
        }
        if (r > 128) {
            if (r >= b) {
                if (r >= g) {
                    vertex = 'R';
                }
            }
        }
    }
    else if (mbvq == "RGBM") {
        vertex = 'G';
        if (r > g) {
            if (r >= b) {
                if (b < 128) {
                    vertex = 'R';
                } else {
                    vertex = 'M';
                }
            }
        }
        if (b > g) {
            if (b >= r) {
                if (r < 128) {
                    vertex = 'B';
                } else {
                    vertex = 'M';
                }
            }
        }
    }
    else  if (mbvq == "CMGB") {
        if (b > 128) {
            if (r > 128) {
                if (g >= r) {
                    vertex = 'C';
                } else {
                    vertex = 'M';
                }
            } else {
                if (g > 128) {
                    vertex = 'C';
                } else {
                    vertex = 'B';
                }
            }
        } else {
            if (r > 128) {
                if (r - g + b >= 128) {
                    vertex = 'M';
                } else {
                    vertex = 'G';
                }
            } else {
                if (g >= b) {
                    vertex = 'G';
                } else {
                    vertex = 'B';
                }
            }
        }
    }
    return vertex;
}


vector<int> get_vector(string vertex){
    vector<int> color_vector;
    // cout << "here" << endl;
    if (vertex == "K"){
        color_vector.push_back(0);
        color_vector.push_back(0);
        color_vector.push_back(0);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "K here" << endl;
        return color_vector;
    }
    else if (vertex == "B"){
        color_vector.push_back(0);
        color_vector.push_back(0);
        color_vector.push_back(255);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "B here" << endl;
        return color_vector;
    }
    else if (vertex == "G"){
        color_vector.push_back(0);
        color_vector.push_back(255);
        color_vector.push_back(0);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "G here" << endl;
        return color_vector;
    }
    else if (vertex == "R"){
        color_vector.push_back(255);
        color_vector.push_back(0);
        color_vector.push_back(0);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "R here" << endl;
        return color_vector;
    }
    else if (vertex == "C"){
        color_vector.push_back(0);
        color_vector.push_back(255);
        color_vector.push_back(255);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "C here" << endl;
        return color_vector;
    }
    else if (vertex == "M"){
        color_vector.push_back(255);
        color_vector.push_back(0);
        color_vector.push_back(255);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "M here" << endl;
        return color_vector;
    }
    else if (vertex == "Y"){
        color_vector.push_back(255);
        color_vector.push_back(255);
        color_vector.push_back(0);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "Y here" << endl;
        return color_vector;
    }
    else if(vertex == "W"){
        color_vector.push_back(255);
        color_vector.push_back(255);
        color_vector.push_back(255);
        // cout << color_vector[0] <<  " " << color_vector[1] << " " << color_vector[2] << endl;
        // cout << "W here" << endl;
        return color_vector;
    }
}


int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int Size = 256;
    int length = 375;
    int width = 500;

    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is greyscale or color
    if (argc < 4) {
        BytesPerPixel = 1; // default is grey image
    } else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5) {
            Size = atoi(argv[4]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[length][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length * width * 3, file);
    fclose(file);


    // COLOR HALF-TONING USING MBVQ
    // Boundary Extension For 3*3 Filter
    float NewImagedataThree[length + 2][width + 2][3];

    for (int k = 0; k < 3; k++) {
        // Creating New Image Data Array by mirroring the second last rows and columns to create length+2*width+2 Bayer Array
        for (int i = 1; i < 1 + width; i++) { // Rows added to top and bottom
            NewImagedataThree[0][i][k] = Imagedata[1][i - 1][k];
            NewImagedataThree[length + 1][i][k] = Imagedata[length - 1][i - 1][k];
        }

        for (int i = 1; i < 1 + length; i++) {
            NewImagedataThree[i][0][k] = Imagedata[i - 1][1][k];
            NewImagedataThree[i][width + 1][k] = Imagedata[i - 1][width - 1][k];
        }

        for (int i = 1; i < length + 1; i++) {
            for (int j = 1; j < width + 1; j++) {
                NewImagedataThree[i][j][k] = Imagedata[i - 1][j - 1][k];
            }
        }

        // Corner elements
        NewImagedataThree[0][0][k] = NewImagedataThree[0][2][k];
        NewImagedataThree[0][width + 1][k] = NewImagedataThree[0][width - 1][k];
        NewImagedataThree[length + 1][0][k] = NewImagedataThree[length + 1][2][k];
        NewImagedataThree[length + 1][width + 1][k] = NewImagedataThree[length + 1][width - 1][k];
    }


//     // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage[length][width][3];
    int e;


    for(int k = 0; k < 3; k++) {
        for (int i = 0; i < length; i++) {
            if (i % 2 == 0) { // Odd Rows
                for (int j = 0; j < width; j++) {
                    string mbvq_output = MBVQ(Imagedata[i][j][0], Imagedata[i][j][1], Imagedata[i][j][2]);
                    string corner = get_vertex(mbvq_output, NewImagedataThree[i][j][0], NewImagedataThree[i][j][1], NewImagedataThree[i][j][2]);
                    vector<int> output = get_vector(corner);
                    e = NewImagedataThree[i + 1][j + 1][k] - output[k];
                    NewImagedataThree[i + 1][j + 2][k] += e * 7 / 16.0;
                    NewImagedataThree[i + 2][j + 2][k] += e * 1 / 16.0;
                    NewImagedataThree[i + 2][j + 1][k] += e * 5 / 16.0;
                    NewImagedataThree[i + 2][j][k] += e * 3 / 16.0;
                    OutputImage[i][j][k] = output[k];
                    // OutputImage[i][j][1] = output[1];
                    // OutputImage[i][j][2] = output[2];

                }
            } else {
                for (int j = width; j >= 0; j--) {//Even Rows
                    string mbvq_output = MBVQ(Imagedata[i][j][0], Imagedata[i][j][1], Imagedata[i][j][2]);
                    string corner = get_vertex(mbvq_output, NewImagedataThree[i][j][0], NewImagedataThree[i][j][1], NewImagedataThree[i][j][2]);
                    vector<int> output = get_vector(corner);
                    e = NewImagedataThree[i + 1][j + 1][k] - output[k];
                    NewImagedataThree[i + 1][j - 1][k] += e * 7 / 16.0;
                    NewImagedataThree[i + 2][j][k] += e * 3 / 16.0;
                    NewImagedataThree[i + 2][j + 1][k] += e * 5 / 16.0;
                    NewImagedataThree[i + 2][j + 2][k] += e * 1 / 16.0;
                    OutputImage[i][j][k] = output[k];
                    // OutputImage[i][j][1] = output[1];
                    // OutputImage[i][j][2] = output[2];
                }
            }
        }
    }



//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), (width)*(length)*3, file);
    fclose(file);

    return 0;
}
