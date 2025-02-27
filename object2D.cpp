#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTrapezoid(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	float width,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length - width / 2, width, 0), color),
		VertexFormat(corner + glm::vec3(width / 2, width, 0), color)
	};

	Mesh* trapezoid = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	if (!fill) {
		trapezoid->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	trapezoid->InitFromData(vertices, indices);
	return trapezoid;
}


Mesh* object2D::CreateSemicircle(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float radius,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(corner, color));
	for (int i = 0; i <= 100; i++) {
		float angle = M_PI * i / 100;
		vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(angle), radius * sin(angle), 0), color));
	}

	Mesh* semicircle = new Mesh(name);
	std::vector<unsigned int> indices;
	for (int i = 1; i <= 100; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	if (!fill) {
		semicircle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		semicircle->SetDrawMode(GL_TRIANGLES);
	}

	indices.push_back(0);
	indices.push_back(101);
	indices.push_back(1);

	semicircle->InitFromData(vertices, indices);
	return semicircle;
}


Mesh* object2D::CreateRectangle(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	float width,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* object2D::CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, bool fill) {
	glm::vec3 corner = center;

	std::vector<VertexFormat> vertices;
	vertices.push_back(VertexFormat(corner, color));
	for (int i = 0; i <= 100; i++) {
		float angle = 2 * M_PI * i / 100;
		vertices.push_back(VertexFormat(corner + glm::vec3(radius * cos(angle), radius * sin(angle), 0), color));
	}

	Mesh* circle = new Mesh(name);
	std::vector<unsigned int> indices;
	for (int i = 1; i <= 100; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	if (!fill) {
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		circle->SetDrawMode(GL_TRIANGLES);
	}

	indices.push_back(0);
	indices.push_back(101);
	indices.push_back(1);

	circle->InitFromData(vertices, indices);
	return circle;
}

Mesh* object2D::CreateLine(const std::string& name, const glm::vec2& start, const glm::vec2& end, const glm::vec3& color) {
	std::vector<VertexFormat> vertices = {
		VertexFormat(glm::vec3(start.x, start.y, 0), color),
		VertexFormat(glm::vec3(end.x, end.y, 0), color)
	};

	std::vector<unsigned int> indices = { 0, 1 };

	Mesh* line = new Mesh(name);
	line->SetDrawMode(GL_LINES);
	line->InitFromData(vertices, indices);

	return line;
}
