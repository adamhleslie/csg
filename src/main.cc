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

const char* floor_fragment_shader =
#include "shaders/floor.frag"
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

void testBspTree()
{
	std::vector<Triangle> triangles;
	Triangle t1(glm::vec3(0, 0, 0),
				glm::vec3(0, 1, 0),
				glm::vec3(0, 1, 1));

	Triangle t2(glm::vec3(-1, 2, 0),
				glm::vec3(-1, 3, 0),
				glm::vec3(1, 2, 0));

	// Triangle t3(glm::vec3(-1, 0, 0),
	// 			glm::vec3(-1, 1, 0),
	// 			glm::vec3(-1, 1, 1));

	// Triangle t3(glm::vec3(-1, 0, 0),
	// 			glm::vec3(-1, 1, 0),
	// 			glm::vec3(-1, 1, 1));

	// triangles.push_back(t3);
	triangles.push_back(t2);
	triangles.push_back(t1);

	BspTree test(triangles);
	test.buildTree();

	std::cout << test.mTriangles.size() << std::endl;
	std::cout << glm::to_string(test.mTriangles[0].mA) << std::endl;
	std::cout << glm::to_string(test.mTriangles[0].mB) << std::endl;
	std::cout << glm::to_string(test.mTriangles[0].mC) << std::endl;

	std::cout << test.mFront->mTriangles.size() << std::endl;
	std::cout << glm::to_string(test.mFront->mTriangles[0].mA) << std::endl;
	std::cout << glm::to_string(test.mFront->mTriangles[0].mB) << std::endl;
	std::cout << glm::to_string(test.mFront->mTriangles[0].mC) << std::endl;

	std::cout << test.mBack->mTriangles.size() << std::endl;
	std::cout << glm::to_string(test.mBack->mTriangles[0].mA) << std::endl;
	std::cout << glm::to_string(test.mBack->mTriangles[0].mB) << std::endl;
	std::cout << glm::to_string(test.mBack->mTriangles[0].mC) << std::endl;
	std::cout << glm::to_string(test.mBack->mTriangles[1].mA) << std::endl;
	std::cout << glm::to_string(test.mBack->mTriangles[1].mB) << std::endl;
	std::cout << glm::to_string(test.mBack->mTriangles[1].mC) << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Input model file is missing" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <PMD file>" << std::endl;
		return -1;
	}
	GLFWwindow *window = init_glefw();
	GUI gui(window);

	std::vector<glm::vec4> floor_vertices;
	std::vector<glm::uvec3> floor_faces;
	create_floor(floor_vertices, floor_faces);

	testBspTree();

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
	glm::mat4 floor_model_matrix = glm::mat4(1.0f);
	auto floor_model_data = [&floor_model_matrix]() -> const void* {
		return &floor_model_matrix[0][0];
	}; // This return model matrix for the floor.
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
	auto alpha_data  = [&gui]() -> const void* {
		static const float transparet = 0.5; // Alpha constant goes here
		static const float non_transparet = 1.0;
		// if (gui.isTransparent())
		// 	return &transparet;
		// else
			return &non_transparet;
	};
	// FIXME: add more lambdas for data_source if you want to use RenderPass.
	//        Otherwise, do whatever you like here
	ShaderUniform std_model = { "model", matrix_binder, std_model_data };
	ShaderUniform floor_model = { "model", matrix_binder, floor_model_data };
	ShaderUniform std_view = { "view", matrix_binder, std_view_data };
	ShaderUniform std_camera = { "camera_position", vector3_binder, std_camera_data };
	ShaderUniform std_proj = { "projection", matrix_binder, std_proj_data };
	ShaderUniform std_light = { "light_position", vector_binder, std_light_data };
	ShaderUniform object_alpha = { "alpha", float_binder, alpha_data };
	// FIXME: define more ShaderUniforms for RenderPass if you want to use it.
	//        Otherwise, do whatever you like here

	// std::vector<glm::vec2>& uv_coordinates = mesh.uv_coordinates;
	// RenderDataInput object_pass_input;
	// object_pass_input.assign(0, "vertex_position", nullptr, mesh.vertices.size(), 4, GL_FLOAT);
	// object_pass_input.assign(1, "normal", mesh.vertex_normals.data(), mesh.vertex_normals.size(), 4, GL_FLOAT);
	// object_pass_input.assign(2, "uv", uv_coordinates.data(), uv_coordinates.size(), 2, GL_FLOAT);
	// object_pass_input.assign_index(mesh.faces.data(), mesh.faces.size(), 3);
	// object_pass_input.useMaterials(mesh.materials);
	// RenderPass object_pass(-1,
	// 		object_pass_input,
	// 		{
	// 		  vertex_shader,
	// 		  geometry_shader,
	// 		  fragment_shader
	// 		},
	// 		{ std_model, std_view, std_proj,
	// 		  std_light,
	// 		  std_camera, object_alpha },
	// 		{ "fragment_color" }
	// 		);

	std::vector<glm::uvec2> singleLine = {glm::uvec2(0, 1)};

	RenderDataInput floor_pass_input;
	floor_pass_input.assign(0, "vertex_position", floor_vertices.data(), floor_vertices.size(), 4, GL_FLOAT);
	floor_pass_input.assign_index(floor_faces.data(), floor_faces.size(), 3);
	RenderPass floor_pass(-1,
			floor_pass_input,
			{ vertex_shader, geometry_shader, floor_fragment_shader},
			{ floor_model, std_view, std_proj, std_light },
			{ "fragment_color" }
			);

	bool draw_floor = true;

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

		// Then draw floor.
		if (draw_floor) {
			floor_pass.setup();
			// Draw our triangles.
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, floor_faces.size() * 3, GL_UNSIGNED_INT, 0));
		}

// 		if (draw_object) {
// 			if (gui.isPoseDirty()) {
// 				mesh.updateAnimation();
// 				object_pass.updateVBO(0,
// 						      mesh.animated_vertices.data(),
// 						      mesh.animated_vertices.size());
// #if 0
// 				// For debugging if you need it.
// 				for (int i = 0; i < 4; i++) {
// 					std::cerr << " Vertex " << i << " from " << mesh.vertices[i] << " to " << mesh.animated_vertices[i] << std::endl;
// 				}
// #endif
// 				gui.clearPose();
// 			}
// 			object_pass.setup();
// 			int mid = 0;
// 			while (object_pass.renderWithMaterial(mid))
// 				mid++;
// #if 0	
// 			// For debugging also
// 			if (mid == 0) // Fallback
// 				CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, mesh.faces.size() * 3, GL_UNSIGNED_INT, 0));
// #endif
// 		}

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
#if 0
	for (size_t i = 0; i < images.size(); ++i)
		delete [] images[i].bytes;
#endif
	exit(EXIT_SUCCESS);
}
