#include "oiio_wrapper.h"
#include <OpenImageIO/imageio.h>

using namespace OIIO;

Image *read_image(const char *filename) {
  Image *image = new Image();

  auto inp = ImageInput::open(filename);
  if (!inp)
    return nullptr;

  const ImageSpec &spec = inp->spec();
  int xres = spec.width;
  int yres = spec.height;
  int nchannels = spec.nchannels;

  image->width = xres;
  image->height = yres;
  image->channels = nchannels;

  image->data = new float[xres * yres * nchannels];

  inp->read_image(0, 0, 0, nchannels, TypeDesc::FLOAT, image->data);
  inp->close();

  return image;
}

void free_image(Image *image) {
  delete[] image->data;
  delete image;
}
