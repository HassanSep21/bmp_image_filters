#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            avg = round(
                (image[row][col].rgbtRed + image[row][col].rgbtGreen + image[row][col].rgbtBlue) /
                3.0);
            image[row][col].rgbtRed = avg;
            image[row][col].rgbtGreen = avg;
            image[row][col].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width / 2; col++)
        {
            tmp = image[row][col];
            image[row][col] = image[row][width - 1 - col];
            image[row][width - 1 - col] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurImage[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            blurImage[row][col] = image[row][col];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            int pix = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    if (row + i >= 0 && row + i < height && col + j >= 0 && col + j < width)
                    {
                        red += blurImage[row + i][col + j].rgbtRed;
                        green += blurImage[row + i][col + j].rgbtGreen;
                        blue += blurImage[row + i][col + j].rgbtBlue;
                        pix++;
                    }
                }
            }
            image[row][col].rgbtRed = round(red / (float) pix);
            image[row][col].rgbtGreen = round(green / (float) pix);
            image[row][col].rgbtBlue = round(blue / (float) pix);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary Copy of Image
    RGBTRIPLE copy[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            copy[row][col] = image[row][col];
        }
    }

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Iterates Over Each Pixel
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            int kernelGxR = 0;
            int kernelGxG = 0;
            int kernelGxB = 0;

            int kernelGyR = 0;
            int kernelGyG = 0;
            int kernelGyB = 0;

            int red;
            int green;
            int blue;

            // Make 3x3 Grid Around the Pixel
            for (int i = row - 1, x = 0; i <= row + 1; i++, x++)
            {
                for (int j = col - 1, y = 0; j <= col + 1; j++, y++)
                {
                    if ((i >= 0 && i < height) && (j >= 0 && j < width))
                    {
                        kernelGxR += Gx[x][y] * image[i][j].rgbtRed;
                        kernelGxG += Gx[x][y] * image[i][j].rgbtGreen;
                        kernelGxB += Gx[x][y] * image[i][j].rgbtBlue;

                        kernelGyR += Gy[x][y] * image[i][j].rgbtRed;
                        kernelGyG += Gy[x][y] * image[i][j].rgbtGreen;
                        kernelGyB += Gy[x][y] * image[i][j].rgbtBlue;
                    }
                }
            }

            // Calculate the Root Mean Square
            red = round(sqrt((kernelGxR * kernelGxR) + (kernelGyR * kernelGyR)));
            green = round(sqrt((kernelGxG * kernelGxG) + (kernelGyG * kernelGyG)));
            blue = round(sqrt((kernelGxB * kernelGxB) + (kernelGyB * kernelGyB)));

            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            copy[row][col].rgbtRed = red;
            copy[row][col].rgbtGreen = green;
            copy[row][col].rgbtBlue = blue;
        }
    }

    // Write Back to The Orignal Image
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            image[row][col] = copy[row][col];
        }
    }

    return;
}
