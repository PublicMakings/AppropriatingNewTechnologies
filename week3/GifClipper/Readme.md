# GifClipper

Turn slices of a 3D model into frames of a gif and use this gif as a clipping mask for another gif.

GifClipper is built from the SlicesExample found in the [Appropriating New Technologies GitHub repository](https://github.com/kylemcdonald/AppropriatingNewTechnologies/tree/master/week3/SlicesExample).

## Usage

Both gifs need to have the same number of frames. Set one gif as the clipping mask which will be stored in the `mask` variable.

## Process

### 1. 123d Make

I downloaded some models from [NASA](http://www.nasa.gov/multimedia/3d_resources/models.html) and wanted to use the Gemini space suit but there were holes in the mesh and 123D Make was unable to export the model as slices. So I used a packaged generic rocket ship. 123D Make turned the model into an EPS of slices of the model to be cut with a laser cutter.

### 2. 