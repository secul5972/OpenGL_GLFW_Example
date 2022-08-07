#define STB_IMAGE_IMPLEMENTATION
#include <learnopengl/shader_s.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float rate = 0.2;

int main()
{
	glfwInit();
	//glfw version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "learnOpenGL", 0, 0);
	if (window == NULL)
	{
		std::cout << "Fail\n";
		glfwTerminate();
		return -1;
	}

	//double buffer로 사용할 window 설정
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//framesize변경시마다 호출되는 callback함수 등록
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//vertexShader 생성, 저장, 컴파일
	Shader ourShader("VertexShader.vs", "FragmentShader.fs");

	//두 개의 삼각형을 위한 정점
	float vertices[] = {
		// 위치              // 컬러             // 텍스처 좌표
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 좌측 상단
	};
	//두 개의 삼각형을 그리기 위한 순서
	unsigned int indices[] = {
		0, 1, 3,   // 첫 번째 삼각형
		1, 2, 3    // 두 번째 삼각형
	};

	unsigned int VAO, VBO, EBO;
	//Vertex Buffer Object 생성
	glGenBuffers(1, &VBO);
	//Vertex Array Object 생성
	glGenBuffers(1, &EBO);
	//Element Buffer Object 생성
	glGenVertexArrays(1, &VAO);

	//VAO bind
	glBindVertexArray(VAO);
	//VBO bind
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//EBO bind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//buffer에 verteces copy
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//buffer에 indices copy
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glVertexAttribute 설정
	//parameter 1: vertexshader의 layout (location = 0)
	//parameter 2: vertex 크기
	//parameter 3: type
	//parameter 4: nomalization(-1 ~ 1사이로)
	//parameter 5: stride (float vec3이므로 3*sizeof(float))
	//parameter 6: buffer offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind 역순
	//VAO를 사용하는 한 EBO를 unbind해서는 안됨
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//texture1 생성
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// 텍스처 wrapping/filtering 옵션 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//이미지 파일의 y축이 뒤집어져 있어서 이미지 뒤집기
	stbi_set_flip_vertically_on_load(true);

	//texture2 생성
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// 텍스처 wrapping/filtering 옵션 설정
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	//uniform variable;
	ourShader.use();
	ourShader.setInt("Texture1", 0);
	ourShader.setInt("Texture2", 1);

	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		//esc
		processInput(window);

		//back ground color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//texture 사용읠 위해 active and bind
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		ourShader.setFloat("rate", rate);

		//draw triangles
		glBindVertexArray(VAO);
		//primitive 종류, EBO에서 그리는 정점 개수, index type, EBO offset
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//VBO를 이용한 그리기
		//glDrawArrays(mode, idx_start, cnt);
		glBindVertexArray(0);

		//dowble buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//blend rate control
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rate += 0.01;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rate -= 0.01;
	if (rate < 0)
		rate = 0;
	if (rate > 1)
		rate = 1;
}
