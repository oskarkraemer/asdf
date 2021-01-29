#include <iostream>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <chrono>

#include "bitmap_image.h"
#include "FastNoiseLite.h"

int main(int argc, char* argv[]) {

	//Arguments: <amount> <width> <height> <R> <G> <B> <outputPath>

	if (argc < 8) {
		std::cout << "Arguments: <amount> <width> <height> <R> <G> <B> <outputPath>" << std::endl;
	}
	else {
		const int width = atoi(argv[2]);
		const int height = atoi(argv[3]);

		bitmap_image image(width, height);
		image.set_all_channels(0, 0, 0);

		//Initialize Noise
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

		int start_time = time(0);

		auto start_time_clock = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < atoi(argv[1]); i++) {
				noise.SetSeed(start_time+i);

				image_drawer draw(image);



				for (int x = 0; x < width; x++) {
					float noiseValue = noise.GetNoise((float)(x * .2), .0f);
						float noiseValue2;
						if (x == width - 1) {
							noiseValue2 = noise.GetNoise((float)(x * .2), .0f);
						}
						else {
							noiseValue2 = noise.GetNoise((float)((x + 1) * .2), .0f);
						}

					if (noiseValue < 0) { noiseValue *= -1; }

					draw.pen_color(((atoi(argv[4]) / 255)*noiseValue)*255, ((atoi(argv[5]) / 255) * noiseValue) * 255, ((atoi(argv[6]) / 255)*noiseValue)*255);
					draw.line_segment(x, height - noiseValue * height, height - x + 1, noiseValue2 * height);

				}

				std::stringstream ss;

				if(*argv[7]=='/' || *argv[7]=='\\') {
					ss << i << ".bmp";
				}
				else {
					ss << argv[7] << i << ".bmp";
				}



				image.save_image(ss.str());
				image.clear();
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto time = end_time - start_time_clock;
		std::cout << "took: " << time / std::chrono::milliseconds(1) << "ms\n";

	}

	return 0;
}