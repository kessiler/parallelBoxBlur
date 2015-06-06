#include <amp.h>
#include <vector>
#include <random>

using namespace concurrency;
using std::vector;

const unsigned int SIZE_WIDTH=10;
const unsigned int SIZE_HEIGTH=10;

array<float,2> applyBoxBlur(array_view<float,2> image)
{
	index<2> origin(1,1);
	array<float, 2> imageBlurred(image);
	extent<2> result(image.extent - extent<2>(2,2));

	parallel_for_each(result, [=, &imageBlurred](index<2> idx) restrict(amp)
	{
		float r = 0.0f;
		int samples = 0;
		idx += origin;

		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				r += image[idx + index<2>(dy,dx)];
				samples++;

			}
		}

		imageBlurred[idx] = r/samples;
	});

	return imageBlurred;
}

void printMatrix(vector<float> imgMatrix) {
	for (int y= 0; y < SIZE_HEIGTH; y++)
	{
		for (int x= 0; x < SIZE_WIDTH; x++)
		{
			printf( "%0.1f ", imgMatrix[x + (y*SIZE_WIDTH)]);
		}
		printf("\n");
	}
}

vector<float> generateDummyImage() {
	std::default_random_engine randomEngine;
	vector<float> image (SIZE_WIDTH * SIZE_HEIGTH);
	std::uniform_real_distribution<float> uni(0.0f, 10.0f);

	for ( int x= 0; x < (SIZE_WIDTH * SIZE_HEIGTH); x++)
	{
		image[x] = uni(randomEngine);
	}
	return image;
}

void main()
{	
	vector<float> image = generateDummyImage();
	printf("Before blur .. \n");
	printMatrix(image);

	array_view<float,2> imageToTransport(SIZE_HEIGTH, SIZE_WIDTH, image);
	vector<float> imageBlurred = applyBoxBlur(imageToTransport);

	printf("\n\nAfter blur .. \n");
	printMatrix(imageBlurred);
	system("pause");
}