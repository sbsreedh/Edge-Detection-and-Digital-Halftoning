

#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>

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


    //Digital Half-Toning
    // Dithering Using Random Number Generator

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage[length][width][1];

    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            int temp = rand() % 256;
           if(Imagedata[i][j][0] < temp){
               OutputImage[i][j][0] = (unsigned char) 0;
           }
           else{
               OutputImage[i][j][0] = (unsigned char) 255;
           }
        }
    }


//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), length*width*1, file);
    fclose(file);

    return 0;
}
