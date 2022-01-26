#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Transform.h"

class Mesh : public Transform
{
public:
	std::string m_file;
	Mesh(std::string file, GLuint shader);
	~Mesh();
	void Clear();
	void DrawMesh();

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	GLuint m_vbo;
	GLuint m_vuv;
	GLuint m_vno;
	GLuint m_vao;
	GLuint m_modelMatrixID;
	GLuint m_itMatrixID;
	GLuint m_texAID;
	GLuint m_texBID;
	GLuint m_texMID;
	GLuint m_texA;
	GLuint m_texB;
	GLuint m_texM;
	GLuint m_shader;

	void Load();
};
