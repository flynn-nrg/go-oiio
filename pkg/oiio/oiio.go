package oiio

/*
#cgo CXXFLAGS: -std=c++17
#cgo pkg-config: OpenImageIO fmt
#include <stdlib.h>
#include "./oiio_wrapper.h"
*/
import "C"

import (
	"errors"
	"fmt"
	"image"
	"unsafe"

	"github.com/flynn-nrg/go-oiio/pkg/floatimage"
)

func ReadImage(filename string) (*floatimage.FloatNRGBA, error) {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))

	cImage := C.read_image(cFilename)
	if cImage == nil {
		return nil, errors.New("failed to read image")
	}

	width := int(cImage.width)
	height := int(cImage.height)
	numChannels := int(cImage.channels)
	cData := (*[1 << 30]C.float)(unsafe.Pointer(cImage.data))[: width*height*numChannels : width*height*numChannels]

	defer C.free_image(cImage)

	var data []float64

	switch cImage.channels {
	case 3:
		data = toRGBSlice(cData, width, height)
	case 4:
		data = toRGBASlice(cData, width, height, numChannels)
	default:
		return nil, fmt.Errorf("unsupported number of channels: %d", cImage.channels)
	}

	return floatimage.NewFloatNRGBA(image.Rectangle{image.Point{0, 0}, image.Point{width, height}}, data), nil
}

func toRGBASlice(cData []C.float, width int, height int, numChannels int) []float64 {

	data := make([]float64, width*height*numChannels)
	for i := 0; i < len(cData); i++ {
		data[i] = float64(cData[i])
	}
	return data
}

func toRGBSlice(cData []C.float, width int, height int) []float64 {
	data := make([]float64, width*height*4)

	j := 0

	for i := 0; i < len(data); i += 4 {
		data[i] = float64(cData[j])
		j++
		data[i+1] = float64(cData[j])
		j++
		data[i+2] = float64(cData[j])
		j++
		data[i+3] = 1.0 // alpha channel is always 1.0
	}

	return data
}
