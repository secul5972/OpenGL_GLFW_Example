#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//shaders written in GLSL
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
	glCompileShader(vertexShader);

	//shader compile error chk
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragmentShader 생성
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//ShaderProgram 생성(Shader Linking)
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Shader object 제거
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//두 개의 삼각형을 위한 정점
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // 우측 상단
		 0.5f, -0.5f, 0.0f,  // 우측 하단
		-0.5f, -0.5f, 0.0f,  // 좌측 하단
		-0.5f,  0.5f, 0.0f   // 좌측 상단
	};
	//두 개의 삼각형을 그리기 위한 순서
	unsigned int indices[] = {
		0, 1, 3,   // 첫 번째 삼각형
		1, 2, 3    // 두 번째 삼각형
	};

	//glVertexAttribute 설정
	//parameter 1: vertexshader의 layout (location = 0)
	//parameter 2: vertex 크기
	//parameter 3: type
	//parameter 4: nomalization(-1 ~ 1사이로)
	//parameter 5: stride (float vec3이므로 3*sizeof(float))
	//parameter 6: buffer offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
	//layout = 0에 지정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind 역순
	//VAO를 사용하는 한 EBO를 unbind해서는 안됨
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		//esc
		processInput(window);

		//back ground color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw triangles
		glUseProgram(shaderProgram);
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
}