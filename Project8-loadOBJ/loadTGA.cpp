#define _CRT_SECURE_NO_DEPRECATE
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <stdio.h>
#include <stdlib.h>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
#include <sstream>
// Include GLFW
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>

struct TGAHeader {
    unsigned char identsize;
    unsigned char color_map_type;
    unsigned char image_type;
    unsigned char color_map_spec[5];
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char bits_per_pixel;
    unsigned char image_descriptor;
};

GLuint loadTGA_custom(const char* imagepath) {
    int width, height;
    std::vector<unsigned char> buffer;
    // Data read from the header of the BMP file
    std::ifstream file(imagepath, std::ios::binary);
    if (!file) {
        std::cerr << "Unable to open file: " << imagepath << std::endl;
        return false;
    }

    TGAHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(TGAHeader));
    width = header.width;
    height = header.height;

    if (header.image_type != 2) {
        std::cerr << "Unsupported TGA image type: " << header.image_type << std::endl;
        return false;
    }

    int bytesPerPixel = header.bits_per_pixel / 8;
    int bytesToRead = width * height * bytesPerPixel;
    buffer.resize(bytesToRead);
    file.read(reinterpret_cast<char*>(buffer.data()), bytesToRead);
    file.close();
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, buffer.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // ÏßÐÔ¹ýÂË
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
};