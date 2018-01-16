#version 410 core

/*
 * OpenGL Programming Guide - Double Write Example
 */

// Buffer containing the rendered image
layout (binding = 0, rgba32f) uniform image2D output_image;

// This is the output color
layout (location = 0) out vec4 color;

void main(void)
{
    color = vec4(imageLoad(output_image, ivec2(gl_FragCoord.xy)).xxxx) / 255.0;
}
