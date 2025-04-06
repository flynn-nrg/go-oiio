# Golang bindings for OpenImageIO
[![Go](https://github.com/flynn-nrg/go-oiio/actions/workflows/go.yml/badge.svg)](https://github.com/flynn-nrg/go-oiio/actions/workflows/go.yml)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)


## Introduction
This package provides Go bindings to read and write images using [OpenImageIO](https://openimageio.readthedocs.io)

The image data is always in float64 format as the use case when building this was to allow [Izpi](https://github.com/flynn-nrg/izpi) to easily import texture assets in many different formats.

## Building
Use your package manager to install the following dependencies:

* pkgconf
* fmt
* OpenImageIO

You can verify this by running the following command:

```shell
$ pkgconf --list-all| egrep '(OpenImage|fmt)'
fmt                            fmt - A modern formatting library
OpenImageIO                    OpenImageIO - OpenImageIO is a library for reading and writing images.
```

## Example usage

```golang
package main

import (
	"fmt"
	"image/png"
	"log"
	"os"

	"github.com/flynn-nrg/go-oiio/pkg/oiio"
)

func main() {
	floatImage, err := oiio.ReadImage("test.exr")
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("image size: %d x %d\n", floatImage.Bounds().Dx(), floatImage.Bounds().Dy())

	f, err := os.Create("test.png")
	if err != nil {
		log.Fatal(err)
	}

	if err := png.Encode(f, floatImage); err != nil {
		f.Close()
		log.Fatal(err)
	}

	if err := f.Close(); err != nil {
		log.Fatal(err)
	}
}
```

## Limitations

* Only 3 and 4 channel images are supported for now.
* [OpenEXR](https://openexr.com) deep pixels are not yet supported.