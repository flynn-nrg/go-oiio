#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Image {
    int width;
    int height;
    int channels;
    float *data;
} Image;

Image *read_image(const char *filename);
void free_image(Image *image);

#ifdef __cplusplus
}
#endif

