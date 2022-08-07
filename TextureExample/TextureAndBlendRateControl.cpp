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

	//double buffer�� ����� window ����
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//framesize����ø��� ȣ��Ǵ� callback�Լ� ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//vertexShader ����, ����, ������
	Shader ourShader("VertexShader.vs", "FragmentShader.fs");

	//�� ���� �ﰢ���� ���� ����
	float vertices[] = {
		// ��ġ              // �÷�             // �ؽ�ó ��ǥ
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ���� ���
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ���� �ϴ�
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ���� �ϴ�
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ���� ���
	};
	//�� ���� �ﰢ���� �׸��� ���� ����
	unsigned int indices[] = {
		0, 1, 3,   // ù ��° �ﰢ��
		1, 2, 3    // �� ��° �ﰢ��
	};

	unsigned int VAO, VBO, EBO;
	//Vertex Buffer Object ����
	glGenBuffers(1, &VBO);
	//Vertex Array Object ����
	glGenBuffers(1, &EBO);
	//Element Buffer Object ����
	glGenVertexArrays(1, &VAO);

	//VAO bind
	glBindVertexArray(VAO);
	//VBO bind
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//EBO bind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//buffer�� verteces copy
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//buffer�� indices copy
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glVertexAttribute ����
	//parameter 1: vertexshader�� layout (location = 0)
	//parameter 2: vertex ũ��
	//parameter 3: type
	//parameter 4: nomalization(-1 ~ 1���̷�)
	//parameter 5: stride (float vec3�̹Ƿ� 3*sizeof(float))
	//parameter 6: buffer offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind ����
	//VAO�� ����ϴ� �� EBO�� unbind�ؼ��� �ȵ�
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//texture1 ����
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// �ؽ�ó wrapping/filtering �ɼ� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// �ؽ�ó �ε� �� ����
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

	//�̹��� ������ y���� �������� �־ �̹��� ������
	stbi_set_flip_vertically_on_load(true);

	//texture2 ����
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// �ؽ�ó wrapping/filtering �ɼ� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// �ؽ�ó �ε� �� ����
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

		//texture ����� ���� active and bind
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		ourShader.setFloat("rate", rate);

		//draw triangles
		glBindVertexArray(VAO);
		//primitive ����, EBO���� �׸��� ���� ����, index type, EBO offset
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//VBO�� �̿��� �׸���
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
