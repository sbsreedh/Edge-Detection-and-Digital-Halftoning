


#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include<algorithm>

using namespace std;


//Function for Convolution Operation

// For Linear Filter of 3*3
float convolve(int i, int j, float filter[3][3], unsigned char image[323][483][1]) {
    float sum = 0;
    for (int m = -1; m < 2; m++){
        for (int n = -1; n < 2; n++){
            sum += filter[m+1][n+1]*image[i+m+1][j+n+1][0];
        }
    }
    return sum;
}

int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    int Size = 256;
    int length = 321;
    int width = 481;

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
    unsigned char Imagedata[length][width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length*width*3, file);
    fclose(file);



    // EDGE DETECTION USING SOBEL MASK

    // Allocate image data array for Gray scale Conversion
    unsigned char ImagedataGray[length][width][1];

    //Converting RGB to Gray using Luminosity weighted average method
    for (int i = 0 ; i < length ; i++){
        for (int j = 0 ; j < width ; j++){
            ImagedataGray[i][j][0] = (unsigned char)((0.21*Imagedata[i][j][0]) + (0.72*Imagedata[i][j][1]) + (0.07*Imagedata[i][j][2]));
        }
    }

    // Allocating New Image Data Arrays with Boundary Extensions:
    // Boundary Extension For 3*3 Filter
    unsigned char NewImagedataThree[323][483][1];

    // Creating New Image Data Array by mirroring the second last rows and columns to create length+2*width+2 Bayer Array
    for(int i = 1; i < 1 + width; i++){ // Rows added to top and bottom
        NewImagedataThree[0][i][0] = ImagedataGray[1][i-1][0];
        NewImagedataThree[length+1][i][0] = ImagedataGray[length-1][i-1][0];
    }

    for(int i = 1; i < 1 + length; i++){
        NewImagedataThree[i][0][0] = ImagedataGray[i-1][1][0];
        NewImagedataThree[i][width+1][0] = ImagedataGray[i-1][width-1][0];
    }

    for(int i = 1; i < length + 1 ; i++) {
        for (int j = 1; j < width + 1; j++) {
            NewImagedataThree[i][j][0] = ImagedataGray[i-1][j-1][0];
        }
    }

    // Corner elements
    NewImagedataThree[0][0][0]=NewImagedataThree[0][2][0];
    NewImagedataThree[0][width+1][0]=NewImagedataThree[0][width-1][0];
    NewImagedataThree[length+1][0][0]= NewImagedataThree[length+1][2][0];
    NewImagedataThree[length+1][width+1][0] = NewImagedataThree[length+1][width-1][0];


    //Defining Sobel Mask
    float SobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    float SobelY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};


    // Allocating 3D Output Image Data Arrays
    float XgradientIm[length][width][1];
    unsigned char XgradientOut[length][width];
    float YgradientIm[length][width][1];
    unsigned char YgradientOut[length][width];
    float GradientMap[length][width][1];
    unsigned char GradientOut[length][width];
    unsigned char EdgeOutput[length][width][1];


    // Extracting X and Y gradients of the Gray Scale Image using X and Y gradient Sobel Masks respectively
    float Xgradient;
    for(int i = 0; i < length; i++) {
        for(int j = 0;j < width; j++){
            Xgradient = convolve(i,j, SobelX, NewImagedataThree);
//            if (Xgradient<0){
//                cout << Xgradient << endl;
//            }
            XgradientIm[i][j][0] = Xgradient;
        }
    }

    //MinMax Pixels
    float Pixel_min = XgradientIm[0][0][0];
    float Pixel_max = XgradientIm[0][0][0];
    //MinMax pixels for normalizing
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(XgradientIm[i][j][0] > Pixel_max){
                Pixel_max =  XgradientIm[i][j][0];
            }
            else if(XgradientIm[i][j][0] < Pixel_min){
                Pixel_min =  XgradientIm[i][j][0];
            }
        }
    }
//    cout << (int)Pixel_max << endl;
//    cout << (int)Pixel_min << endl;

    //Normalize
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            XgradientOut[i][j] = (unsigned char)(((XgradientIm[i][j][0]- Pixel_min)/(Pixel_max - Pixel_min))*255.0);
        }
    }

    //Y gradient
    float Ygradient;
    for(int i = 0; i < length; i++) {
        for(int j = 0;j < width; j++){
            Ygradient = convolve(i,j, SobelY, NewImagedataThree);
            YgradientIm[i][j][0] = Ygradient;
        }
    }

    //MinMax Pixels
    float Pixel_min1 = YgradientIm[0][0][0];
    float Pixel_max1 = YgradientIm[0][0][0];
    //MinMax pixels for normalizing
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(YgradientIm[i][j][0] > Pixel_max1){
                Pixel_max1 =  YgradientIm[i][j][0];
            }
            else if(YgradientIm[i][j][0] < Pixel_min1){
                Pixel_min1 =  YgradientIm[i][j][0];
            }
        }
    }

    //Normalize
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            YgradientOut[i][j] = (unsigned char)(((YgradientIm[i][j][0]- Pixel_min1)/(Pixel_max1 - Pixel_min1))*255.0);
        }
    }

    //Gradient Mapping
    int pixel, max_pixel = 0;
    for(int i=0;i<length;i++){
        for(int j=0;j<width;j++){
            GradientMap[i][j][0] = (float)((sqrt(pow(XgradientIm[i][j][0],2) + pow(YgradientIm[i][j][0],2))));
            pixel = GradientMap[i][j][0];
            if (pixel > max_pixel){
                max_pixel = pixel;
                // cout << max_pixel << endl;
            }
        }
    }

    float Pixel_min2 = GradientMap[0][0][0];
    float Pixel_max2 = GradientMap[0][0][0];
    //MinMax pixels for normalizing
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(GradientMap[i][j][0] > Pixel_max2){
                Pixel_max2 =  GradientMap[i][j][0];
            }
            else if(YgradientIm[i][j][0] < Pixel_min2){
                Pixel_min2 =  GradientMap[i][j][0];
            }
        }
    }

    // cout << Pixel_max2 << " " << Pixel_min2 << endl;

    //Normalize
    // for(int i = 0; i < length; i++){
    //     for(int j = 0; j < width; j++){
    //         GradientOut[i][j] = (unsigned char)(((GradientMap[i][j][0]- Pixel_min2)/(Pixel_max2 - Pixel_min2))*255.0);
    //     }
    // }

    // Initialize Intensity
    float hist_intensity[int(Pixel_max2)] = { 0 };
    // int pixel, max_pixel = 0;

    //Create Histogram of Image Intensities
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            pixel = (int)GradientMap[i][j][0];
            hist_intensity[pixel] += 1;
            //cout << "intensity"<< hist_intensity;
        }
    }

    float CDF[(int)Pixel_max2] = { 0 };
    CDF[0] = hist_intensity[0];
    for (int i = 1 ; i < (int)Pixel_max2 ; i++)
    {
        CDF[i] = CDF[i-1] + hist_intensity[i];
        // cout << i << " " << hist_intensity[i] << " " << CDF[i] << endl;
    }

    // cout << CDF[0] << " " << CDF[(int)(Pixel_max2-1)] <<  endl;

    float threshold_percentage = 0.70*CDF[(int)(Pixel_max2-1)];
    // cout << threshold_percentage << endl;
    float threshold;
    // Find Threshold value from CDF
    for (int i = 1 ; i < 256 ; i++){
        if(CDF[i] > threshold_percentage){
            threshold = i;
            break;
        }
    }

    // cout << threshold << endl;
    //Binarize with Thresholding
    //cout<<threshold<<endl;
    for (int i = 0; i < length; i++){
        for (int j = 0; j < width; j++){
            if (GradientMap[i][j][0] > threshold)
                EdgeOutput[i][j][0] = 0;
            else
                EdgeOutput[i][j][0] = 255;
        }
    }


//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(EdgeOutput, sizeof(unsigned char), (width)*(length)*1, file);
    fclose(file);

    return 0;
}
