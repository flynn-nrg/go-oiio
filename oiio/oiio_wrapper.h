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

Image *read_image(const char *filename, char **error_msg);
void free_image(Image *image);

// Returns 0 on success, non-zero error code on failure
// hdr: 1 for HDR, 0 for LDR
int write_image(const char *filename, Image *image, char **error_msg, int hdr);

#ifdef __cplusplus
}
#endif
