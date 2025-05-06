#include "oiio_wrapper.h"
#include <OpenImageIO/imageio.h>

using namespace OIIO;

Image *read_image(const char *filename, char **error_msg) {
  Image *image = new Image();

  auto inp = ImageInput::open(filename);
  if (!inp) {
    *error_msg = strdup("Could not open image file");
    return nullptr;
  }

  const ImageSpec &spec = inp->spec();
  int xres = spec.width;
  int yres = spec.height;
  int nchannels = spec.nchannels;

  image->width = xres;
  image->height = yres;
  image->channels = nchannels;

  image->data = new float[xres * yres * nchannels];

  if (!inp->read_image(0, 0, 0, nchannels, TypeDesc::FLOAT, image->data)) {
    *error_msg = strdup(inp->geterror().c_str());
    delete[] image->data;
    delete image;
    inp->close();
    return nullptr;
  }

  inp->close();
  return image;
}

void free_image(Image *image) {
  delete[] image->data;
  delete image;
}

int write_image(const char *filename, Image *image, char **error_msg, int hdr) {
  std::unique_ptr<ImageOutput> out = ImageOutput::create(filename);
  if (!out) {
    *error_msg = strdup("Could not create ImageOutput");
    return 1;
  }

  ImageSpec spec;
  spec.width = image->width;
  spec.height = image->height;
  spec.nchannels = image->channels;
  spec.format = hdr ? TypeDesc::FLOAT : TypeDesc::UINT8;
  spec.channelnames = {"R", "G", "B", "A"};

  if (!out->open(filename, spec)) {
    *error_msg = strdup(out->geterror().c_str());
    return 1;
  }

  if (hdr) {
    // For HDR formats, write the float data directly
    if (!out->write_image(TypeDesc::FLOAT, image->data)) {
      *error_msg = strdup(out->geterror().c_str());
      out->close();
      return 1;
    }
  } else {
    // For LDR formats, convert float to UINT8
    size_t num_pixels = image->width * image->height * image->channels;
    std::vector<uint8_t> uint8_data(num_pixels);
    for (size_t i = 0; i < num_pixels; ++i) {
      uint8_data[i] = static_cast<uint8_t>(
          std::min(255.0f, std::max(0.0f, image->data[i] * 255.0f)));
    }
    if (!out->write_image(TypeDesc::UINT8, uint8_data.data())) {
      *error_msg = strdup(out->geterror().c_str());
      out->close();
      return 1;
    }
  }

  out->close();
  return 0;
}
