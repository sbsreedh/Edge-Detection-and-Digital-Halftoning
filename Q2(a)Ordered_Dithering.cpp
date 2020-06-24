
#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <vector>

using namespace std;

//Function for Creating Dithering Matrix
vector<vector<int>> dither(vector<vector<int>> dith_seed, int present_size){
    vector<vector<int>> dith_next;
    vector<int> vect_inner;
    for(int i=0; i<present_size*2; i++){
        vect_inner.clear();
        for(int j=0; j<present_size*2; j++){
            vect_inner.push_back(0);
        }
        dith_next.push_back(vect_inner);
    }
    for(int i=0, k=0; i<present_size; i++, k++){
        for(int j=0, l=0; j<present_size; j++, l++){
            dith_next[i][j] = 4*dith_seed[k][l] + 1;
        }
    }

    for(int i=0, k=0; i<present_size; i++, k++){
        for(int j=present_size, l=0; j<present_size*2; j++, l++){
            dith_next[i][j] = 4*dith_seed[k][l] + 2;
        }
    }

    for(int i=present_size, k=0; i<present_size*2; i++, k++){
        for(int j=0, l=0; j<present_size; j++, l++){
            dith_next[i][j] = 4*dith_seed[k][l] + 3;
        }
    }

    for(int i=present_size, k=0; i<present_size*2; i++, k++){
        for(int j=present_size, l=0; j<present_size*2; j++, l++){
            dith_next[i][j] = 4*dith_seed[k][l];
        }
    }
    return dith_next;
}


int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    int Size = 256;
    int length = 400;
    int width = 600;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is greyscale or color
    if (argc < 4){
        BytesPerPixel = 1; // default is grey image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5){
            Size = atoi(argv[4]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[length][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length*width*BytesPerPixel, file);
    fclose(file);

    // Dithering Matrices for I2, I4 and I8

    //int I2[2][2] = {{1,2},{3,0}};
    vector<vector<int>> dith_next={
            {1, 2},
            {3, 0}
    };

    int present_size = 2, final_size = 32;

    while(present_size != final_size){
        dith_next = dither(dith_next, present_size);
        present_size *= 2;
    }

    //Printing Matix
    for(int i=0; i<final_size; i++){
        for(int j=0; j<final_size; j++){
            cout << dith_next[i][j]<< " ";
        }
        cout << endl;
    }


    //int I4[4][4] = {{5,9,6,10},{13,1,14,2},{7,11,4,8},{15,3,12,0}};
    //int I8[8][8] = {{21,37,25,41,22,38,26,42},{53,5,57,9,54,6,58,10},{29,45,17,33,30,46,18,34},{61,13,49,1,62,14,50,2},{23,39,27,43,20,36,24,40},{55,7,59,11,52,4,56,8},{31,47,19,35,28,44,16,32},{63,15,51,3,60,12,48,0}};


    // Thresholding using Dithering Matrix

    // Allocating Thresholding Matrices
//    unsigned char ThreshI2[2][2][1];
//    unsigned char ThreshI4[4][4][1];
//    unsigned char ThreshI8[8][8][1];
    unsigned char ThreshI32[32][32][1];

//    // I2
//    for(int i =0; i < 2; i++){
//        for(int j = 0; j < 2; j++){
//            ThreshI2[i][j][0] = (unsigned char)(((dith_next[i][j] + 0.5)/ 4)* 255);
//        }
//    }

//    // I4
//    for(int i =0; i < 4; i++){
//        for(int j = 0; j < 4; j++){
//            ThreshI4[i][j][0] = (unsigned char)(((dith_next[i][j] + 0.5)/ 16)* 255);
//        }
//    }

//    // I8
//    for(int i =0; i < 8; i++){
//        for(int j = 0; j < 8; j++){
//            ThreshI8[i][j][0] = (unsigned char)(((dith_next[i][j] + 0.5)/ 64)* 255);
//        }
//    }

    //I32
    for(int i =0; i < 32; i++){
        for(int j = 0; j < 32; j++){
            ThreshI32[i][j][0] = (unsigned char)(((dith_next[i][j] + 0.5)/ 1024)* 255);
        }
    }

    //Halftoning by Comparing with Thresholding matrix
    // Allocating Output Matrices
//    unsigned char OutputI2[length][width][1];
//    unsigned char OutputI4[length][width][1];
//    unsigned char OutputI8[length][width][1];
    unsigned char OutputI32[length][width][1];


//    //I2
//    for(int i =0; i < length; i++){
//        for(int j = 0; j < width; j++){
//            if(Imagedata[i][j][0] > ThreshI2[i % 2][j % 2][0] && Imagedata[i][j][0] < 256){
//                OutputI2[i][j][0] = 255;
//            }
//            else if(Imagedata[i][j][0] <= ThreshI2[i % 2][j % 2][0] && Imagedata[i][j][0] >= 0){
//                OutputI2[i][j][0] = 0;
//            }
//        }
//    }


//
//    //I4
//    for(int i =0; i < length; i++){
//        for(int j = 0; j < width; j++){
//            if(Imagedata[i][j][0] > ThreshI4[i % 4][j % 4][0] && Imagedata[i][j][0] < 256){
//                OutputI4[i][j][0] = 255;
//            }
//            else{
//                OutputI4[i][j][0] = 0;
//            }
//        }
//    }


//    //I8
//    for(int i =0; i < length; i++){
//        for(int j = 0; j < width; j++){
//            if(Imagedata[i][j][0] > ThreshI8[i % 8][j % 8][0] && Imagedata[i][j][0] < 256){
//                OutputI8[i][j][0] = 255;
//            }
//            else{
//                OutputI8[i][j][0] = 0;
//            }
//        }
//    }


    //I32
    for(int i =0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(Imagedata[i][j][0] > ThreshI32[i % 32][j % 32][0] && Imagedata[i][j][0] < 256){
                OutputI32[i][j][0] = 255;
            }
            else{
                OutputI32[i][j][0] = 0;
            }
        }
    }



////
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputI32, sizeof(unsigned char), width*length*1, file);
    fclose(file);

    return 0;
}
