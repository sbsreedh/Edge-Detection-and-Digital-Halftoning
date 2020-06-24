

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
    unsigned char Imagedata[length][width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length * width * 3, file);
    fclose(file);


    // HALF-TONING USING ERROR DIFFUSION

    // COLOR HALF-TONING
    // RGB to CMY Conversion

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            Imagedata[i][j][0] = 255 - Imagedata[i][j][0];
            Imagedata[i][j][1] = 255 - Imagedata[i][j][1];
            Imagedata[i][j][2] = 255 - Imagedata[i][j][2];

        }
    }



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


    //Diffusing Error by Serpentine Method

    // Allocating 3D Output Image Data Arrays
//    unsigned char OutputImage[length][width][3];
    int e;

    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < length; i++) {
            if (i % 2 == 0) { // Odd Rows
                for (int j = 0; j < width; j++) {
                    if (NewImagedataThree[i + 1][j + 1][k] > 128) {
                        Imagedata[i][j][k] = 255;
                    } else {
                        Imagedata[i][j][k] = 0;
                    }
                    e = NewImagedataThree[i + 1][j + 1][k] - Imagedata[i][j][k];
                    NewImagedataThree[i + 1][j + 2][k] += e * 7 / 16.0;
                    NewImagedataThree[i + 2][j + 2][k] += e * 1 / 16.0;
                    NewImagedataThree[i + 2][j + 1][k] += e * 5 / 16.0;
                    NewImagedataThree[i + 2][j][k] += e * 3 / 16.0;
                }
            } else {
                for (int j = width; j >= 0; j--) {//Even Rows
                    if (NewImagedataThree[i + 1][j + 1][k] > 128) {
                        Imagedata[i][j][k] = 255;
                    } else {
                        Imagedata[i][j][k] = 0;
                    }
                    e = NewImagedataThree[i + 1][j + 1][k] - Imagedata[i][j][k];
                    NewImagedataThree[i + 1][j - 1][k] += e * 7 / 16.0;
                    NewImagedataThree[i + 2][j][k] += e * 3 / 16.0;
                    NewImagedataThree[i + 2][j + 1][k] += e * 5 / 16.0;
                    NewImagedataThree[i + 2][j + 2][k] += e * 1 / 16.0;
                }
            }
        }
    }


    //CMY to RGB Conversion
    unsigned char r, g, b;

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            r = (unsigned char)(255 - Imagedata[i][j][0]);
            if (r > 255){
                r = 255;
            } else if (r < 0){
                r = 0;
            }
            Imagedata[i][j][0] = r;

            g = (unsigned char)(255 - Imagedata[i][j][1]);
            if (g > 255){
                g = 255;
            } else if (g < 0){
                g = 0;
            }
            Imagedata[i][j][1] = g;

            b = (unsigned char)(255 - Imagedata[i][j][2]);
            if (b > 255){
                b = 255;
            } else if (b < 0){
                b = 0;
            }
            Imagedata[i][j][2] = b;
        }
    }



//
    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(Imagedata, sizeof(unsigned char), (width) * (length)* 3, file);
    fclose(file);

    return 0;

}
