#include <GL/glew.h>
#include <dirent.h>

#include "procedure_geometry.h"
#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "bsp_tree.h"
#include "triangle.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>
#include <material.h>

int window_width = 800, window_height = 600;
const std::string window_title = "Skinning";

const char* vertex_shader =
#include "shaders/default.vert"
;

const char* geometry_shader =
#include "shaders/default.geom"
;

const char* fragment_shader =
#include "shaders/default.frag"
;

const char* mesh_fragment_shader =
#include "shaders/mesh.frag"
;

const char* normal_fragment_shader =
#include "shaders/normal.frag"
;

const char* line_fragment_shader =
#include "shaders/line.frag"
;

const char* line_vertex_shader =
#include "shaders/line.vert"
;

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

GLFWwindow* init_glefw()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);
	CHECK_SUCCESS(ret != nullptr);
	glfwMakeContextCurrent(ret);
	glewExperimental = GL_TRUE;
	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	return ret;
}

int main(int argc, char* argv[])
{
	// if (argc < 2)
	// 	std::cerr << "Input model file is missing" << std::endl;
	// 	std::cerr << "Usage: " << argv[0] << " <PMD file>" << std::endl;
	// 	return -1;
	// }
	GLFWwindow *window = init_glefw();
	GUI gui(window);

	// std::vector<glm::vec4> floor_vertices;
	// std::vector<glm::uvec3> floor_faces;
	// create_floor(floor_vertices, floor_faces);

	// Generate BspTree
	std::vector<Triangle> originalTriangles, meshTriangles;
	generateSphere(originalTriangles, 5, 1, 0);

	// extendTriangles(originalTriangles, meshTriangles, glm::vec3(.1, .1, .1));
	// extendTriangles(meshTriangles, originalTriangles, glm::vec3(-.1, 0, 0));
	// extendTrianglesNormalized(originalTriangles, meshTriangles, .5, true);
	// extendTrianglesSpherically(meshTriangles, originalTriangles, glm::vec3(0), true);
	// extendTrianglesSpherically(originalTriangles, meshTriangles, glm::vec3(0), true);
	// extendTrianglesSpherically(meshTriangles, originalTriangles, glm::vec3(0), true);
	// extendTriangles(meshTriangles, originalTriangles, 1.3, true);
	// extendTriangles(originalTriangles, meshTriangles, 1.1, true);
	// extendTriangles(meshTriangles, originalTriangles, 1.05, true);

	BspTree mesh(originalTriangles);
	mesh.buildTree();

	std::vector<glm::vec4> mesh_vertices;
	std::vector<glm::uvec3> mesh_faces;
	std::vector<glm::vec4> mesh_normals;
	std::vector<glm::vec3> mesh_colors;

	std::vector<glm::vec4> line_vertices;
	std::vector<glm::uvec2> line_lines;

	std::vector<Triangle> triangles;

	mesh.getTriangles(triangles);
	for (Triangle& triangle : triangles)
	{
		triangle.addToRenderBuffer(mesh_vertices, mesh_faces, mesh_normals, mesh_colors);
		triangle.addLinesToRenderBuffer(line_vertices, line_lines);
		// std::cout << triangle << std::endl;
	}

	glm::vec4 light_position = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
	MatrixPointers mats; // Define MatrixPointers here for lambda to capture

	/*
	 * In the following we are going to define several lambda functions to bind Uniforms.
	 * 
	 * Introduction about lambda functions:
	 *      http://en.cppreference.com/w/cpp/language/lambda
	 *      http://www.stroustrup.com/C++11FAQ.html#lambda
	 */
	auto matrix_binder = [](int loc, const void* data) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)data);
	};
	auto vector_binder = [](int loc, const void* data) {
		glUniform4fv(loc, 1, (const GLfloat*)data);
	};
	auto vector3_binder = [](int loc, const void* data) {
		glUniform3fv(loc, 1, (const GLfloat*)data);
	};
	auto float_binder = [](int loc, const void* data) {
		glUniform1fv(loc, 1, (const GLfloat*)data);
	};

	/*
	 * These lambda functions below are used to retrieve data
	 */
	auto std_model_data = [&mats]() -> const void* {
		return mats.model;
	}; // This returns point to model matrix
	auto std_view_data = [&mats]() -> const void* {
		return mats.view;
	};
	auto std_camera_data  = [&gui]() -> const void* {
		return &gui.getCamera()[0];
	};
	auto std_proj_data = [&mats]() -> const void* {
		return mats.projection;
	};
	auto std_light_data = [&light_position]() -> const void* {
		return &light_position[0];
	};

	// auto diffuse_data  = []() -> const void* {
	// 	static const glm::vec4 diffuse(0.776, 0.506, 0.325, 1);
	// 	return &diffuse;
	// };
	// auto ambient_data  = []() -> const void* {
	// 	static const glm::vec4 ambient(0.388, 0.253, 0.1625, 1);
	// 	return &ambient;
	// };
	// auto specular_data  = []() -> const void* {
	// 	static const glm::vec4 specular(0.2, 0.2, 0.2, 1);
	// 	return &specular;
	// };
	// auto shininess_data  = []() -> const void* {
	// 	static const float shininess = 5;
	// 	return &shininess;
	// };
	auto alpha_data  = [&gui]() -> const void* {
		static const float transparent = 0.5; // Alpha constant goes here
		static const float non_transparent = 1.0;
		// if (gui.isTransparent())
		// 	return &transparet;
		// else
			return &non_transparent;
	};

	ShaderUniform std_model = { "model", matrix_binder, std_model_data };
	ShaderUniform std_view = { "view", matrix_binder, std_view_data };
	ShaderUniform std_camera = { "camera_position", vector3_binder, std_camera_data };
	ShaderUniform std_proj = { "projection", matrix_binder, std_proj_data };
	ShaderUniform std_light = { "light_position", vector_binder, std_light_data };
	ShaderUniform alpha = { "alpha", float_binder, alpha_data };
	// ShaderUniform diffuse = { "diffuse", vector_binder, diffuse_data };
	// ShaderUniform ambient = { "ambient", vector_binder, ambient_data };
	// ShaderUniform specular = { "specular", vector_binder, specular_data };
	// ShaderUniform shininess = { "shininess", float_binder , shininess_data };

	RenderDataInput mesh_pass_input;
	mesh_pass_input.assign(0, "vertex_position", mesh_vertices.data(), mesh_vertices.size(), 4, GL_FLOAT);
	mesh_pass_input.assign(1, "vertex_normal", mesh_normals.data(), mesh_normals.size(), 4, GL_FLOAT);
	mesh_pass_input.assign(2, "vertex_color", mesh_colors.data(), mesh_colors.size(), 3, GL_FLOAT);
	mesh_pass_input.assign_index(mesh_faces.data(), mesh_faces.size(), 3);
	RenderPass mesh_pass(-1,
			mesh_pass_input,
			{ vertex_shader, nullptr, normal_fragment_shader },
			{ std_model, std_view, std_proj, alpha },
			{ "fragment_color" }
			);

	RenderDataInput line_pass_input;
	line_pass_input.assign(0, "vertex_position", line_vertices.data(), line_vertices.size(), 4, GL_FLOAT);
	line_pass_input.assign_index(line_lines.data(), line_lines.size(), 2);
	RenderPass line_pass(-1,
			line_pass_input,
			{ line_vertex_shader, nullptr, line_fragment_shader },
			{ std_model, std_view, std_proj },
			{ "fragment_color" }
			);

	// bool draw_floor = true;
	bool draw_mesh = true;
	bool draw_lines = true;
	size_t currentMeshID = 0;

	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

		gui.updateMatrices();
		mats = gui.getMatrixPointers();

		size_t meshID = gui.getDrawnMeshID();
		if (currentMeshID != meshID)
		{
			// Switch to proper mesh
			std::cout << "Switching to mesh: " << meshID << std::endl;
			currentMeshID = meshID;
		}

		if (draw_mesh) {
			mesh_pass.setup();
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, mesh_faces.size() * 3, GL_UNSIGNED_INT, 0));
		}

		if (gui.isDrawingLines()) {
			line_pass.setup();
			CHECK_GL_ERROR(glDrawElements(GL_LINES, line_lines.size() * 2, GL_UNSIGNED_INT, 0));
		}

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
