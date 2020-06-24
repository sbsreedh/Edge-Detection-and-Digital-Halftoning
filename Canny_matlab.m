I = imread('Pig.jpg');
%imshow(I)
I2 =rgb2gray(I)
%imshow(I2)

%BW1 = edge(I2,'sobel', 0);
BW2 = edge(I2,'canny');
figure;
%imshowpair(BW1,BW2,'montage')
%title('Sobel Filter                                   Canny Filter');
imshow(BW2);
