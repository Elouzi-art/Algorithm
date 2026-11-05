# Sobel Edge Detection

The Sobel operator detects image edges by approximating horizontal and vertical intensity derivatives.

## Kernels

Horizontal gradient:

```text
Gx = [-1  0  1]      Gy = [-1 -2 -1]
     [-2  0  2]           [ 0  0  0]
     [-1  0  1]           [ 1  2  1]
```

For each pixel, the implementation computes:

```text
magnitude = sqrt(Gx * Gx + Gy * Gy)
```

A threshold converts the gradient magnitude into a binary edge map: values at or above the threshold become white (255), otherwise black (0).

## Input and output

The program reads and writes grayscale **P2 (ASCII PGM)** images. This keeps the example dependency-free and makes the pixel format easy to inspect. Border pixels are set to black.

## Compile and run

```bash
g++ -std=c++17 -Wall -Wextra -O2 cpp/sobel.cpp -o sobel
./sobel input.pgm edges.pgm 100
```

The last argument is the gradient-magnitude threshold. Increase it to retain stronger edges only; decrease it to detect weaker transitions.

## Complexity

For an image of width (W) and height (H), the convolution takes (O(WH)) time and the pixel buffers require (O(WH)) memory.

## Limitations

This educational implementation expects P2 PGM input and produces a binary edge map. It does not perform smoothing, non-maximum suppression, or hysteresis as in the Canny detector.
