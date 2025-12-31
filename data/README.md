# Sample Data Placeholder

This directory should contain input images for edge detection processing.

## Adding Images

Place your test images in this directory with formats such as:
- `.jpg` / `.jpeg`
- `.png`
- `.bmp`
- `.tiff`

## Sample Images

You can download sample images from:
- [OpenCV Samples](https://github.com/opencv/opencv/tree/master/samples/data)
- [Free Stock Photos](https://unsplash.com/)
- [Test Images](https://homepages.cae.wisc.edu/~ece533/images/)

## Example Files

```
data/
├── sample.jpg          # Sample image 1
├── sample2.jpg         # Sample image 2
├── landscape.png       # Landscape photo
└── architecture.jpg    # Building/architecture photo
```

## Configuration

Reference these images in your `config.txt` file:

```ini
[image]
input_path = data/sample.jpg
output_prefix = output/sample
threshold1 = 50.0
threshold2 = 150.0
```

## Creating Test Images

If you don't have test images, you can create simple test patterns:

```bash
# Using ImageMagick (if installed)
convert -size 512x512 gradient:white-black data/gradient.png
convert -size 512x512 pattern:checkerboard data/checkerboard.png

# Using Python with PIL
python3 -c "from PIL import Image; Image.new('L', (512, 512), 128).save('data/gray.png')"
```
