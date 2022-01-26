#include <stdio.h>
#include <GLEW/GL/glew.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//#include <SFML/OpenGL.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/audio/Sound.hpp>
#include <SFML/Audio.hpp>
#include <complex>
#include <SFML/System.hpp>
#include "hanningFilter.hpp"
#include "unsupported/Eigen/FFT"


#define N 2048
int sampleCount;
int sampleRate;
bool monoTrack;

GLFWwindow* InitWindow()
{
	if (!glfwInit())
	{
		fprintf(stderr, "GLFW Failed.");
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(
		1920,
		1080,
		"J Demo",
		NULL, NULL);
	if (window == NULL)
	{
		std::cout << window;
		fprintf(stderr, "Failed to open window.");
		return NULL;
	}

	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		fprintf(stderr, "GLEW Failed.");
		glfwTerminate();
		return NULL;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	return window;
}



GLuint SetVAO()
{
	static const GLfloat vertexBufferData[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	static const GLfloat colorBufferData[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData),
		vertexBufferData, GL_STATIC_DRAW);

	GLuint vcb = 0;
	glGenBuffers(1, &vcb);
	glBindBuffer(GL_ARRAY_BUFFER, vcb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData),
		colorBufferData, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vcb);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	return vao;
}

GLuint SetVaoShape() {
	{
		static const GLfloat vertexBufferData[] = {
		0.0f,-3.0f,-3.0f,
		0.0f,-3.0f, 3.0f,
		0.0f, 3.0f, 3.0f,

		0.1f, 3.0f,-3.0f,
		0.1f,-3.0f,-3.0f,
		0.1f, 3.0f,-3.0f,
		};

		static const GLfloat colorBufferData[] = {
			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,

			0.583f,  0.771f,  0.014f,
			0.609f,  0.115f,  0.436f,
			0.327f,  0.483f,  0.844f,


		};

		GLuint vbo = 1;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData),
			vertexBufferData, GL_STATIC_DRAW);

		GLuint vcb = 1;
		glGenBuffers(1, &vcb);
		glBindBuffer(GL_ARRAY_BUFFER, vcb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData),
			colorBufferData, GL_STATIC_DRAW);

		GLuint vao = 1;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vcb);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);

		return vao;
	}
}




int main(void)
{



	sf::Music music;
	if (!music.openFromFile("Music\\Music.flac"))
		printf("Error\n");
	//music.play();


	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("Music\\Music.flac"))
		printf("Error");

	sf::Sound sound;
	sound.setBuffer(buffer);
	//buffer.getSamples();

	const int16_t* samples = buffer.getSamples();
	const int sampleCount = buffer.getSampleCount();
	sampleRate = buffer.getSampleRate();
	float time = music.getDuration().asSeconds();

	if (sampleCount / time == 44100)
	{
		std::cout << sampleCount / time << " Mono Track!" << std::endl;
		monoTrack = true;
	}
	else
	{
		monoTrack = false;
		std::cout << sampleCount / time << " Stereo Track!" << std::endl;
	}

	// Create window.
	GLFWwindow* window = InitWindow();
	if (window == NULL)
	{
		return -1;
	}
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	GLuint vao = SetVAO();
	//GLuint vaoShape = SetVaoShape();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	Scene scene;
	//scene.AddMesh("Obj\\bunny.obj");
	//scene.AddMesh("Obj\\3dMaxSphere.obj");
	scene.AddMesh("Obj\\plane.obj");
	//scene.AddMesh("Obj\\PlaneObj3dsMax.obj");

	std::vector<float> timevec(N, 0);
	std::vector<std::complex<float>> freqvec = {};

	Eigen::FFT<float> fft;

	float offset = 0.0, flash_intensity = 0.0, limit_distance = 0.0, top_distance = 0.0;
	uint32_t cur_sample = 0, prev_sample = 0;
	sound.play();
	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// check on which sample the playback is
		offset = sound.getPlayingOffset().asSeconds();
		if (monoTrack)
			cur_sample = offset * sampleRate;
		else
			cur_sample = offset * 2 * sampleRate;

		// if no new samples since the last check, sleep for 10 ms

		if (cur_sample == prev_sample) {
			sf::sleep(sf::milliseconds(10));
			continue;
		}

		prev_sample = cur_sample;

		// if there are enough samples, assign them to timevec and apply Hanning filter
		if ((cur_sample + N) < sampleCount) {
			timevec.assign(samples + cur_sample, samples + cur_sample + N);
			std::transform(timevec.begin(), timevec.end(), back_inserter(timevec), math::hanning_filter);

		}

		// calculate FFT
		fft.fwd(freqvec, timevec);

		float mag = 0;
		glm::vec1 magVectors(N);

		float floats[1022]{};

		for (int i = 0; i < 1022; i++)
		{

			float foo = math::magnitude(freqvec[i]);
			floats[i] = foo;
		}
		for (int i = 0; i < freqvec.size(); i++)
		{
			float foo = math::magnitude(freqvec[i]);
			if (mag < foo) {
				mag = foo;
			}
		}

		scene.DrawScene(mag / 1000, floats);



		glBindVertexArray(vao);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteVertexArrays(1, &vao);
	//glDeleteVertexArrays(2, &vaoShape);

	scene.Clear();
	glfwTerminate();

}

