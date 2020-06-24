

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

    // ERROR DIFFUSION USING FLOYD ERROR DIFFUSION MATRIX

    // Boundary Extension For 3*3 Filter
    float NewImagedataThree[length + 2][width + 2][1];

    // Creating New Image Data Array by mirroring the second last rows and columns to create length+2*width+2 Bayer Array
    for(int i = 1; i < 1 + width; i++){ // Rows added to top and bottom
        NewImagedataThree[0][i][0] = Imagedata[1][i-1][0];
        NewImagedataThree[length+1][i][0] = Imagedata[length-1][i-1][0];
    }

    for(int i = 1; i < 1 + length; i++){
        NewImagedataThree[i][0][0] = Imagedata[i-1][1][0];
        NewImagedataThree[i][width+1][0] = Imagedata[i-1][width-1][0];
    }

    for(int i = 1; i < length + 1 ; i++) {
        for (int j = 1; j < width + 1; j++) {
            NewImagedataThree[i][j][0] = Imagedata[i-1][j-1][0];
        }
    }

    // Corner elements
    NewImagedataThree[0][0][0]=NewImagedataThree[0][2][0];
    NewImagedataThree[0][width+1][0]=NewImagedataThree[0][width-1][0];
    NewImagedataThree[length+1][0][0]= NewImagedataThree[length+1][2][0];
    NewImagedataThree[length+1][width+1][0] = NewImagedataThree[length+1][width-1][0];

    //Diffusing Error by Serpentine Method

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage[length][width][1];
    int e;

    for(int i = 0; i< length; i++){
        if(i % 2 == 0) { // Odd Rows
            for(int j = 0; j< width; j++){
                if (NewImagedataThree[i+1][j+1][0] > 128){
                    OutputImage[i][j][0] = 255;
                }
                else {
                    OutputImage[i][j][0] = 0;
                }
                    e = NewImagedataThree[i+1][j+1][0] - OutputImage[i][j][0];
                    NewImagedataThree[i+1][j + 2][0] += e * 7 / 16.0;
                    NewImagedataThree[i + 2][j + 2][0] += e * 1 / 16.0;
                    NewImagedataThree[i + 2][j+1][0] += e * 5 / 16.0;
                    NewImagedataThree[i + 2][j ][0] += e * 3 / 16.0;
            }
        }
        else {
            for(int j = width; j >= 0; j--){//Even Rows
                if (NewImagedataThree[i+1][j+1][0] > 128) {
                    OutputImage[i][j][0] = 255;
                }
                else {
                    OutputImage[i][j][0] = 0;
                }
                    e = NewImagedataThree[i+1][j+1][0] - OutputImage[i][j][0];
                    NewImagedataThree[i+1][j - 1][0] += e * 7 / 16.0;
                    NewImagedataThree[i + 2][j][0] += e * 3 / 16.0;
                    NewImagedataThree[i + 2][j+1][0] += e * 5 / 16.0;
                    NewImagedataThree[i + 2][j +2][0] += e * 1 / 16.0;
            }
        }
    }


//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), (width)*(length), file);
    fclose(file);

    return 0;
}
