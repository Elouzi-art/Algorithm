#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Minimal P2 (ASCII grayscale) PGM reader.
std::string nextToken(std::istream& input) {
    std::string token;
    char ch;

    while (input.get(ch)) {
        if (ch == '#') {
            input.ignore(100000, '\n');
            continue;
        }
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            token += ch;
            break;
        }
    }

    while (input.get(ch)) {
        if (std::isspace(static_cast<unsigned char>(ch))) break;
        if (ch == '#') {
            input.ignore(100000, '\n');
            break;
        }
        token += ch;
    }

    if (token.empty()) {
        throw std::runtime_error("Unexpected end of PGM file.");
    }
    return token;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " input.pgm output.pgm threshold\n"
                  << "Input format: P2 (ASCII grayscale PGM).\n";
        return 1;
    }

    try {
        const int threshold = std::stoi(argv[3]);
        if (threshold < 0) {
            throw std::invalid_argument("Threshold must be non-negative.");
        }

        std::ifstream input(argv[1]);
        if (!input) throw std::runtime_error("Cannot open input image.");

        if (nextToken(input) != "P2") {
            throw std::runtime_error("Only P2 (ASCII) PGM images are supported.");
        }

        const int width = std::stoi(nextToken(input));
        const int height = std::stoi(nextToken(input));
        const int maxValue = std::stoi(nextToken(input));

        if (width <= 0 || height <= 0 || maxValue <= 0 || maxValue > 65535) {
            throw std::runtime_error("Invalid PGM dimensions or maximum value.");
        }

        std::vector<int> pixels(static_cast<std::size_t>(width) * height);
        for (int& pixel : pixels) {
            pixel = std::stoi(nextToken(input));
            if (pixel < 0 || pixel > maxValue) {
                throw std::runtime_error("Pixel value is outside the PGM range.");
            }
            pixel = pixel * 255 / maxValue;
        }

        const int gxKernel[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
        };
        const int gyKernel[3][3] = {
            {-1, -2, -1},
            { 0,  0,  0},
            { 1,  2,  1}
        };

        std::vector<int> edges(pixels.size(), 0);
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                int gx = 0;
                int gy = 0;

                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        const int value = pixels[(y + ky) * width + (x + kx)];
                        gx += value * gxKernel[ky + 1][kx + 1];
                        gy += value * gyKernel[ky + 1][kx + 1];
                    }
                }

                const double magnitude = std::sqrt(
                    static_cast<double>(gx) * gx + static_cast<double>(gy) * gy
                );
                edges[y * width + x] = magnitude >= threshold ? 255 : 0;
            }
        }

        std::ofstream output(argv[2]);
        if (!output) throw std::runtime_error("Cannot create output image.");

        output << "P2\n" << width << ' ' << height << "\n255\n";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                output << edges[y * width + x] << ' ';
            }
            output << '\n';
        }

        std::cout << "Sobel edge map saved to " << argv[2] << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
