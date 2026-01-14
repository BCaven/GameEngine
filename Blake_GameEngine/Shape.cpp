#include "Shape.h"

Shape::Shape(const size_t triangleCount, const std::vector<float>& data)
{
	logger = spdlog::get("shape");
	if (!logger)
	{
		logger = spdlog::stdout_color_mt("shape");
	}

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	int offset = 0; // TODO: compute the offset :)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Shape Shape::fromFile(std::string fileName)
{
	std::fstream input(fileName);
	std::vector<float> floatData;
	if (input.is_open())
	{
		// file is organized: triangle count, triangles (x, y, z), normals (x, y, z)

		std::string line;
		// get the triangle count
		std::getline(input, line, ' ');
		int triCount = std::stoi(line);
		while (std::getline(input, line, ' '))
		{
			int x = std::stoi(line);
			floatData.push_back(x);
		}

		std::shared_ptr<Shape> constructedShape(new Shape(triCount, floatData));
	}
	else
	{
		
	}
}