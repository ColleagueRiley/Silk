// Compilation command:
// cc main.c -o example.out

#include <stdio.h>

#define SILK_IMPLEMENTATION
#include "../../silk.h"

int main(int argc, const string argv[]) {
    string file_path = "output.ppm";
    pixel_buffer buffer = silkCreatePixelBuffer(
        800, 
        600
    );

    silkClearPixelBufferColor(&buffer, 0xffffffff);

    silkDrawCircle(
        &buffer, 
        (vec2i) { 
            buffer.size.x / 2.0f,
            buffer.size.y / 2.0f
        }, 
        128,
        0xff0000ff
    );
    
    // You can learn more about PPM format here:
    // https://netpbm.sourceforge.net/doc/ppm.html

    FILE* file = fopen(file_path, "w");
    if(!file) {
        silkLogErr("FILE: Couldn't open the file: %s", file_path);
        return SILK_FAILURE;
    }

    fprintf(
        file, 
        "P6\n"      // PPM Header
        "%i %i\n"   // PPM image's width and height
        "255\n",    // PPM max color information (maximum color value can be 225)
        buffer.size.x, 
        buffer.size.y
    );

    if(ferror(file)) {
        fclose(file);
        return SILK_FAILURE;
    }

    for(int i = 0; i < buffer.size.x * buffer.size.y; i++) {
        u8 channels[3] = {
            silkPixelToColor(buffer.buffer[i]).r,
            silkPixelToColor(buffer.buffer[i]).g,
            silkPixelToColor(buffer.buffer[i]).b
        };
        
        fwrite(
            channels, 
            sizeof(channels), 
            1, 
            file  
        );

        if(ferror(file)) {
            fclose(file);
            return SILK_FAILURE;
        }
    }

    fclose(file);

    silkPixelBufferFree(&buffer);

    return 0;
}