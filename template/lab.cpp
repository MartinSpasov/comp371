// template.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
    


const GLuint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 800;
GLFWwindow* window;

//camera vectors
glm::vec3 pos = glm::vec3(50.0f, 50.0f, 60.0f);
glm::vec3 lookAt = glm::vec3(50.0f, 50.0f, 0.0f);
glm::vec3 whatsUp = glm::vec3(0.0f, 1.0f, 00.0f);

glm::mat4 view_matrix = glm::lookAt(pos, lookAt, whatsUp);

//Horse variables
glm::vec3 horsePos(50.0f, 50.0f, 3.0f);

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  //ESC closes window
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT)) { //Camera zoom in
		pos.z += 0.5f;
	}
	if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT)) { //Camera zoom out
		pos.z -= 0.5f;
	}
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) { //Camera move
		pos.y += 0.5f;
		lookAt.y += 0.5f;
	}
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {//Camera move
		pos.y -= 0.5f;
		lookAt.y -= 0.5f;
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {//Camera move
		pos.x -= 0.5f;
		lookAt.x -= 0.5f;
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {//Camera move
		pos.x += 0.5f;
		lookAt.x += 0.5f;
	}
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) { //Camera move
		//pos.y += 0.5f;
		lookAt.y += 0.5f;
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {//Camera move
		//pos.y -= 0.5f;
		lookAt.y -= 0.5f;
	}
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {//Camera move
		//pos.x -= 0.5f;
		lookAt.x -= 0.5f;
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {//Camera move
		//pos.x += 0.5f;
		lookAt.x += 0.5f;
	}

	view_matrix = glm::lookAt(pos, lookAt, whatsUp);
}


GLuint loadShaders(std::string vertex_shader_path, std::string fragment_shader_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);

	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glBindAttribLocation(ProgramID, 0, "in_Position");

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID); //free up memory
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

int init() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Horse", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void createGrid(std::vector<glm::vec3>& verticies) {
	for (int i = 0; i < 101; i++) {
		verticies.push_back(glm::vec3(i, 0, 0));
		verticies.push_back(glm::vec3(i, 100, 0));
	}
	for (int i = 0; i < 101; i++) {
		verticies.push_back(glm::vec3(0, i, 0));
		verticies.push_back(glm::vec3(100, i, 0));
	}
}

void createAxis(std::vector<glm::vec3>& verticies) {

}

void createCube(std::vector<glm::vec3>& verticies) {
	verticies = {
		glm::vec3(-0.5f,-0.5f,-0.5f), // triangle 1 : begin
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f), // triangle 1 : end
		glm::vec3(0.5f, 0.5f,-0.5f), // triangle 2 : begin
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f), // triangle 2 : end
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f)
	};
}

int main()
{
	if (init() < 0) {
		return -1;
	}
	
	glfwSetKeyCallback(window, key_callback);

	GLuint shaderProgram = loadShaders("vertex.shader", "fragment.shader");
	glUseProgram(shaderProgram);

	std::vector<glm::vec3> gridVertices;
	createGrid(gridVertices);

	std::vector<glm::vec3> cubeVertices;
	createCube(cubeVertices);

	GLuint gridVBO, gridVAO, cubeVBO, cubeVAO = 0;
	glGenVertexArrays(1, &gridVAO);
	glBindVertexArray(gridVAO);
	//these are the verticies
	glGenBuffers(1, &gridVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*gridVertices.size(), gridVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//VBA and VBA for horse
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*cubeVertices.size(), cubeVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	GLuint color_loc = glGetUniformLocation(shaderProgram, "color");
	GLuint model_matrix_loc = glGetUniformLocation(shaderProgram, "model_matrix");
	GLuint view_matrix_loc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint projection_matrix_loc = glGetUniformLocation(shaderProgram, "projection_matrix");

	glm::vec4 gridColor(0.0f, 0.0f, 0.5f, 1.0f);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while (!glfwWindowShouldClose(window)){
		//Check for events
		glfwPollEvents();
		//Swap screen buffers
		glfwSwapBuffers(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model_matrix;
		model_matrix = glm::scale(model_matrix, glm::vec3(1.0f));

		glm::mat4 projection_matrix = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		
		glUniform4fv(color_loc, 1, glm::value_ptr(gridColor));
		glBindVertexArray(gridVAO);
		glDrawArrays(GL_LINES, 0, gridVertices.size());

		//this is rendering horse body
		model_matrix = glm::translate(glm::mat4(1), horsePos);

		glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

		glUniform4fv(color_loc, 1, glm::value_ptr(glm::vec4(1.0f,0.0f,0.0f,1.0f)));

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

	}

}

