# C++ Image Processor

A terminal-based image processing application written in modern C++17. Load any image, apply a rich set of filters one by one, undo/redo your changes, and save the result — all from a colourful, interactive command-line menu.

---

## Table of Contents

- [Features](#features)
- [Demo](#demo)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [Filters Reference](#filters-reference)
- [Requirements](#requirements)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Supported Image Formats](#supported-image-formats)
- [Adding a New Filter](#adding-a-new-filter)
- [Contributors](#contributors)

---

## Features

- 🖼️ **Load & Save** — Load images from the `assets/` folder; save results anywhere (directories are created automatically).
- 🎨 **22 Built-in Filters** — From basic colour adjustments to artistic effects (full list [below](#filters-reference)).
- ↩️ **Undo / Redo** — Full history stack lets you step backwards and forwards through every change.
- 🔗 **Chained Filters** — Apply any number of filters in sequence before saving.
- 🖥️ **Colourful CLI** — ANSI-coloured prompts and status messages for a pleasant terminal experience.
- ♻️ **Scalable OOP Design** — Adding a new filter requires only creating one new class; no other source files need to change.

---

## Demo

### Menu
![Menu](docs/Menu.jpg)

### Video walkthrough
[Watch on YouTube](https://www.youtube.com/watch?v=BYMyUjpoP5g)

---

## Project Structure

```
cpp-image-processor/
├── assets/                  # Sample images (loaded at runtime)
├── docs/                    # Documentation assets
├── include/
│   ├── core/
│   │   ├── CurrentImage.h   # Manages the active image & undo/redo stacks
│   │   └── Menu.h           # CLI menu logic
│   ├── filters/
│   │   ├── Filter.h         # Abstract base class for all filters
│   │   ├── ConvolutionFilter.h  # Generic kernel-based convolution helper
│   │   └── *.h              # One header per filter
│   └── utils/
│       ├── Colors.h         # ANSI colour macros
│       └── Utils.h          # Path resolution helpers
├── src/
│   ├── core/
│   │   ├── CurrentImage.cpp
│   │   └── Menu.cpp
│   ├── filters/
│   │   └── *.cpp            # One implementation file per filter
│   ├── utils/
│   │   └── Utils.cpp
│   ├── main.cpp             # Entry point
│   └── stb_image.cpp        # stb_image compilation unit
├── third_party/
│   ├── Image_Class.h        # Thin wrapper around stb for pixel access
│   ├── stb_image.h
│   └── stb_image_write.h
└── CMakeLists.txt
```

---

## Architecture

The application follows the **Object-Oriented Programming** paradigm with a clean separation of concerns.

```
┌──────────────────────────────────────────────────────────┐
│                        main.cpp                          │
│  • Creates CurrentImage                                  │
│  • Instantiates all Filter objects (shared_ptr)          │
│  • Runs the main event loop via Menu                     │
└────────────────┬─────────────────────┬───────────────────┘
                 │                     │
    ┌────────────▼──────┐   ┌──────────▼──────────┐
    │   CurrentImage    │   │       Menu           │
    │  ─────────────    │   │  ──────────────      │
    │  Image img        │   │  showMenuOptions()   │
    │  stack Undo       │   │  setResponse()       │
    │  stack Redo       │   │  welcomeMsg()        │
    │  load() / save()  │   └─────────────────────-┘
    │  undo() / redo()  │
    └───────────────────┘

    ┌──────────────────────────────────────────┐
    │           Filter  (abstract)             │
    │  ─────────────────────────────────────   │
    │  + apply()   = 0  (pure virtual)         │
    │  + getNeeds()= 0  (prompt for params)    │
    │  + getName() = 0                         │
    │  + static getId()                        │
    │  # Image& image                          │
    └──────────────────┬───────────────────────┘
                       │  inherits
       ┌───────────────┼──────────────────┐
       │               │                  │
  GreyScale         Blur            OilPainting
  WhiteAndBlack     EdgeDetection   ArtisticBrush
  Invert            Frame           (22 filters total)
  … etc.            … etc.
```

Each filter:
1. Inherits from `Filter` and overrides `apply()`, `getNeeds()`, `getName()`, and `getId()`.
2. Holds a **reference** to the shared `Image` object — no copies made during filtering.
3. Is registered once in `main.cpp`; the menu dispatches by `getId()` string.

---

## Filters Reference

| # | ID | Filter | Description |
|---|----|--------|-------------|
| 1 | `1` | **Greyscale** | Converts each pixel to the average of its RGB channels. |
| 2 | `2` | **Black & White** | Applies a luminance threshold to produce a pure black-or-white image. |
| 3 | `3` | **Invert** | Inverts every channel (`255 - value`). |
| 4 | `4` | **Merge** | Blends the loaded image with a second image (pixel average). Supports *stretch-to-fit* or *common-area* merge for differently sized images. |
| 5 | `5` | **Flip** | Flips the image horizontally (`h`) or vertically (`v`). |
| 6 | `6` | **Rotate** | Rotates by 90°, 180°, or 270° using a centre-based affine transform. |
| 7 | `7` | **Brightness** | Multiplies every channel by a user-supplied factor (e.g., `1.5` = +50% brightness, `0.5` = −50%). |
| 8 | `8` | **Crop** | Crops a rectangular region given a starting corner and dimensions. |
| 9 | `9` | **Frame** | Adds a solid-colour border. Optional *decorative* mode adds a contrasting inner white line. Accepts named colours or custom RGB. |
| 10 | `10` | **Edge Detection** | Detects edges using Sobel kernels (horizontal + vertical), producing a greyscale edge map. |
| 11 | `11` | **Resize** | Rescales the image to arbitrary dimensions using nearest-neighbour sampling. |
| 12 | `12` | **Blur** | Fast box blur using a 2-D prefix-sum approach with a configurable radius. |
| 13 | `13` | **Sunlight** | Simulates warm sunlight by boosting the red and green channels. |
| 14 | `14` | **Oil Painting** | Stylises the image as an oil painting by sampling the most frequent intensity level in a neighbourhood. |
| 15 | `15` | **Old TV** | Applies scan lines and noise to mimic a vintage CRT television effect. |
| 16 | `16` | **Night** | Creates a blue-tinted low-light night vision effect. |
| 17 | `17` | **Infrared** | Swaps channel intensities to simulate an infrared camera image. |
| 18 | `18` | **Horizontal Skew** | Shears the image horizontally by a given angle using a tangent-based pixel shift. |
| 19 | `19` | **Bloody** | Tints the image with deep red tones for a dramatic effect. |
| 20 | `20` | **Grass** | Overlays a green grass-tinted effect on the lower portion of the image. |
| 21 | `21` | **Sky** | Applies a sky-blue tint gradient to the upper portion of the image. |
| 22 | `22` | **Artistic Brush** | An oil-painting variant with an adjustable brush radius for a looser, painterly look. |

---

## Requirements

| Tool | Minimum Version |
|------|----------------|
| C++ Compiler (GCC / Clang / MSVC) | C++17 |
| CMake | 3.15 |

No external libraries are required — image I/O is handled by the bundled [stb_image](https://github.com/nothings/stb) headers in `third_party/`.

---

## Build Instructions

### Linux / macOS

```bash
# 1. Clone the repository
git clone https://github.com/mohammadabdulhakim/cpp-image-processor.git
cd cpp-image-processor

# 2. Configure with CMake
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build --config Release

# 4. Run
./build/ImageProcessing
```

### Windows (Visual Studio / MSVC)

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\ImageProcessing.exe
```

> **Note:** CMake automatically copies the `assets/` folder next to the produced executable via a post-build step, so sample images are always available.

---

## Usage

```
====================
    Image Filters
=====================
[l] Load Image
[0] Exit
# l

Please enter image name you want to apply filter on: img.jpg
```

Once an image is loaded the full filter menu appears:

```
[l] Load Image
=====================
    Choose Filters
======================
     [1] Grey Scale Filter.
     [2] White and Black Filter.
     ...
     [s] Save the Image.
     [u] Undo     [r] Redo
[0] Exit
# 1       ← apply Greyscale
# s       ← save result
```

**Key commands**

| Key | Action |
|-----|--------|
| `l` | Load an image from `assets/` |
| `1`–`22` | Apply the corresponding filter |
| `s` | Save the current image to `output/` |
| `u` | Undo the last filter |
| `r` | Redo an undone filter |
| `0` | Exit the application |

Saved images are written to an `output/` directory (created automatically) next to the executable.

---

## Supported Image Formats

Any format supported by **stb_image** can be loaded:

- JPEG / JPG
- PNG
- BMP
- TGA
- GIF (first frame)
- PSD (flat composite)
- HDR

Saved output uses **stb_image_write** and supports JPEG, PNG, BMP, and TGA based on the file extension you provide.

---

## Adding a New Filter

The design makes adding filters straightforward — no existing files need to be modified except `main.cpp`.

1. **Create a header** `include/filters/MyFilter.h` that inherits `Filter`:

   ```cpp
   #pragma once
   #include "Filter.h"

   class MyFilter : public Filter {
   public:
       MyFilter(Image &img);
       std::string getName() override;
       static std::string getId();
       void apply() override;
       void getNeeds() override;
   };
   ```

2. **Create the implementation** `src/filters/MyFilter.cpp` with your pixel-manipulation logic.

3. **Register** it in `main.cpp` by adding one line to the `filters` vector:

   ```cpp
   {MyFilter::getId(), make_shared<MyFilter>(currentImage.img)},
   ```

The menu will automatically display and dispatch the new filter.

---

## Contributors

| Student ID | Name | Assigned Filters |
|------------|------|-----------------|
| 20242295 | **Mohammad Abdulhakim Ramadan** | White & Black, Flip, Crop, Resize, Oil Painting, Infrared, Artistic Brush, Bloody, Sky, Grass |
| 20242110 | **Hamza Mohammad Zaki** | Greyscale, Merge, Brightness (Darken/Lighten), Edge Detection |
| 20240588 | **Mostafa Ahmed Ali** | Invert, Rotate, Frame, Blur, Sunlight, Night, Old TV, Skew |

---

> 📂 **Repository:** https://github.com/mohammadabdulhakim/cpp-image-processor  
> 🎬 **Demo Video:** https://www.youtube.com/watch?v=BYMyUjpoP5g  
> 📄 **Documentation:** [Google Docs](https://docs.google.com/document/d/1qCHJ92_c90hBn0wo34rJ349e9NXzKGfr/edit?usp=sharing)
