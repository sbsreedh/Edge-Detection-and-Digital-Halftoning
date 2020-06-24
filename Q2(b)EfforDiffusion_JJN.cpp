

#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <cmath>

using namespace std;

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


    // HALF-TONING USING ERROR DIFFUSION

    // ERROR DIFFUSION USING JARVIS ERROR DIFFUSION MATRIX
    // For 5*5 Filter
    unsigned char NewImagedataFive[length + 4][width + 4][1];

    // Creating New Image Data Array by mirroring the second and third last rows and columns to create length+4*width+4 Bayer Array
    for(int i = 1; i < 3 + width; i++){ // Rows added to top and bottom
        NewImagedataFive[0][i+1][0] = Imagedata[2][i-1][0];
        NewImagedataFive[1][i+1][0] = Imagedata[1][i-1][0];
        NewImagedataFive[length+3][i][0] = Imagedata[length-3][i-1][0];
        NewImagedataFive[length+2][i][0] = Imagedata[length-2][i-1][0];
    }

    for(int i = 1; i < 3 + length; i++){ // Columns added to right and left
        NewImagedataFive[i+1][0][0] = Imagedata[i-1][2][0];
        NewImagedataFive[i+1][1][0] = Imagedata[i-1][1][0];
        NewImagedataFive[i][width+3][0] = Imagedata[i-1][width-3][0];
        NewImagedataFive[i][width+2][0] = Imagedata[i-1][width-2][0];
    }

    for(int i = 2; i < length + 3 ; i++) { // Actual Image copy
        for (int j = 2; j < width + 3; j++) {
            NewImagedataFive[i][j][0] = Imagedata[i-2][j-2][0];
        }
    }

    // Corner elements
    //Corner 1

    NewImagedataFive[0][0][0]=NewImagedataFive[0][4][0];
    NewImagedataFive[0][1][0]=NewImagedataFive[0][3][0];
    NewImagedataFive[1][0][0]=NewImagedataFive[1][4][0];
    NewImagedataFive[1][1][0]=NewImagedataFive[1][3][0];

    //Corner 2

    NewImagedataFive[0][width+3][0]=NewImagedataFive[0][width-3][0];
    NewImagedataFive[0][width+2][0]=NewImagedataFive[0][width-2][0];
    NewImagedataFive[1][width+3][0]=NewImagedataFive[1][width-3][0];
    NewImagedataFive[1][width+2][0]=NewImagedataFive[1][width-2][0];

    // Corner 3

    NewImagedataFive[length+3][0][0]= NewImagedataFive[length-1][0][0];
    NewImagedataFive[length+2][0][0]= NewImagedataFive[length][0][0];
    NewImagedataFive[length+3][1][0]= NewImagedataFive[length+1][1][0];
    NewImagedataFive[length+2][1][0]= NewImagedataFive[length][1][0];

    //Corner 4
    NewImagedataFive[length+3][width+3][0] = NewImagedataFive[length+3][width-1][0];
    NewImagedataFive[length+3][width+2][0] = NewImagedataFive[length+3][width][0];
    NewImagedataFive[length+2][width+3][0] = NewImagedataFive[length+2][width-1][0];
    NewImagedataFive[length+3][width+2][0] = NewImagedataFive[length+3][width][0];

    //Diffusing Error by Serpentine Method

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage[length][width][1];
    int e;

    for(int i = 0; i< length; i++){
        if(i % 2 == 0) { // Odd Rows
            for(int j = 0; j< width; j++){
                if (NewImagedataFive[i+1][j+1][0] > 128){
                    OutputImage[i][j][0] = 255;

                }
                else {
                    OutputImage[i][j][0] = 0;
                }
                e = NewImagedataFive[i+1][j+1][0] - OutputImage[i][j][0];
                NewImagedataFive[i+1][j + 2][0] += e * 7 / 48.0;
                NewImagedataFive[i+1][j + 3][0] += e * 5 / 48.0;
                NewImagedataFive[i + 2][j - 1][0] += e * 3 / 48.0;
                NewImagedataFive[i + 2][j][0] += e * 5 / 48.0;
                NewImagedataFive[i + 2][j + 1][0] += e * 7 / 48.0;
                NewImagedataFive[i + 2][j + 2][0] += e * 5 / 48.0;
                NewImagedataFive[i + 2][j + 3][0] += e * 3 / 48.0;
                NewImagedataFive[i + 3][j - 1][0] += e * 1 / 48.0;
                NewImagedataFive[i + 3][j][0] += e * 3 / 48.0;
                NewImagedataFive[i + 3][j + 1][0] += e * 5 / 48.0;
                NewImagedataFive[i + 3][j + 2][0] += e * 3 / 48.0;
                NewImagedataFive[i + 3][j + 3][0] += e * 1 / 48.0;


            }
        }
        else {
            for(int j = width; j >= 0; j--){//Even Rows
                if (NewImagedataFive[i+1][j+1][0] > 128) {
                    OutputImage[i][j][0] = 255;
                }
                else {
                    OutputImage[i][j][0] = 0;
                }
                e = NewImagedataFive[i+1][j+1][0] - OutputImage[i][j][0];
                NewImagedataFive[i+1][j - 1][0] += e * 7 / 48.0;
                NewImagedataFive[i+1][j][0] += e * 5 / 48.0;
                NewImagedataFive[i + 2][j - 1][0] += e * 3 / 48.0;
                NewImagedataFive[i + 2][j][0] += e * 5 / 48.0;
                NewImagedataFive[i + 2][j + 1][0] += e * 7 / 48.0;
                NewImagedataFive[i + 2][j + 2][0] += e * 5 / 48.0;
                NewImagedataFive[i + 2][j + 3][0] += e * 3 / 48.0;
                NewImagedataFive[i + 3][j - 1][0] += e * 1 / 48.0;
                NewImagedataFive[i + 3][j][0] += e * 3 / 48.0;
                NewImagedataFive[i + 3][j + 1][0] += e * 5 / 48.0;
                NewImagedataFive[i + 3][j + 2][0] += e * 3 / 48.0;
                NewImagedataFive[i + 3][j + 3][0] += e * 1 / 48.0;
            }
        }
    }


//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), (width)*(length)*1, file);
    fclose(file);

    return 0;
}
