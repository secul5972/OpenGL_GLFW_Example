#include <learnopengl/shader_s.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

	Shader ourShader("VertexShader.vs", "FragmentShader.fs");


	//두 개의 삼각형을 위한 정점
	float vertices[] = {
		// 위치              // 컬러
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 우측 하단
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 좌측 하단
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 위 
	};

	unsigned int VAO, VBO;
	//Vertex Buffer Object 생성
	glGenBuffers(1, &VBO);
	//Element Buffer Object 생성
	glGenVertexArrays(1, &VAO);

	//VAO bind
	glBindVertexArray(VAO);
	//VBO bind
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//buffer에 verteces copy
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//layout = 0에 지정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind 역순
	//VAO를 사용하는 한 EBO를 unbind해서는 안됨
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		//esc
		processInput(window);

		//back ground color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangles
		ourShader.use();

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		ourShader.setVector("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
}