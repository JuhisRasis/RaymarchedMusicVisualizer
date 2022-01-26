#include <stdio.h>
#include <GLEW/GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "Scene.h"
#include <GLFW/glfw3.h>
#include <iostream>

float t = 0;
float RotationAngle = 0;

Scene::Scene()
{

	m_shaderID = LoadShaders(
		"Shaders\\SimpleVertexShader.vertexshader",
		"Shaders\\visualizerFrag.fragmentshader");
	m_viewMatrixID = glGetUniformLocation(m_shaderID, "vm");
	m_projectionMatrixID = glGetUniformLocation(m_shaderID, "pm");
	m_lightAmountID = glGetUniformLocation(m_shaderID, "lightAmo");
	m_lightDirectionID = glGetUniformLocation(m_shaderID, "lightDir");
	MMMatrixID = glGetUniformLocation(m_shaderID, "mm");

	m_timeID = glGetUniformLocation(m_shaderID, "time");
	m_frequencyToSendID = glGetUniformLocation(m_shaderID, "frequencyToSend");
	m_magArrayToSendID = glGetUniformLocation(m_shaderID, "magVecToSend");

	m_projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


	// HARDCODED, REMOVE! DEBUG ONLY!
}


Scene::~Scene()
{

}

void Scene::AddMesh(std::string file)
{
	Mesh m(file, m_shaderID);
	m.Rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_meshes.push_back(m);
	printf("Mesh added to the scene: %s\n", file.c_str());
}

void Scene::DrawScene(int frequencyToSend, float magArray[])
{
	m_light.UpdateMatrix();
	glUseProgram(m_shaderID);
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);
	glUniformMatrix4fv(m_projectionMatrixID, 1, GL_FALSE, &m_projectionMatrix[0][0]);
	glUniform1f(m_lightAmountID, m_light.m_amount);
	glm::vec3 d = m_light.Forward();
	glUniform3f(m_lightDirectionID, d.x, d.y, d.z);
	float sinTime = (float)glm::sin((double)t * .005f) * 3;

	glUniform1f(m_timeID, t / 100);
	//std::cout << frequencyToSend << "\n";
	glUniform1f(m_frequencyToSendID, frequencyToSend);
	glUniform1fv(m_magArrayToSendID, 1022, magArray);

	t++;
	glm::mat4 rotatePlane = glm::mat4(1);


	for (int i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i].DrawMesh();
	}

	glm::mat4 model = glm::mat4(1);
	glm::mat4 rot = glm::mat4(1);
	glm::mat4 tra = glm::mat4(1);
	m_meshes[0].Rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 90));

	//tra = glm::translate(tra, glm::vec3(0, 0, 0));
	//model = tra * rot;

	m_light.Rotate(glm::radians(-45.0f), glm::vec3(0, 1, 0));

	glUniformMatrix4fv(MMMatrixID, 1, GL_FALSE, &model[0][0]);


	m_viewMatrix = glm::lookAt(
		glm::vec3(50, 3, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
}

void Scene::Clear()
{
	printf("Clearing scene\n");
	for (int i = 0; i < m_meshes.size(); ++i)
	{
		m_meshes[i].Clear();
	}
	m_meshes.clear();
	glDeleteProgram(m_shaderID);
}
