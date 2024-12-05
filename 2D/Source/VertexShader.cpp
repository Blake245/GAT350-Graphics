#include "VertexShader.h"
#include "Shader.h"

//#define GOURAUD	

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex)
{
	glm::mat4 mvp = Shader::uniforms.projection * Shader::uniforms.view * Shader::uniforms.model;
	overtex.position = mvp * glm::vec4{ ivertex.position, 1 };
	glm::mat4 mv = Shader::uniforms.view * Shader::uniforms.model;
	overtex.normal = glm::vec3{ glm::normalize(glm::mat3{ mv } *ivertex.normal) };
	overtex.vposition = mv * glm::vec4{ ivertex.position, 1 };


#ifdef GOURAUD
	glm::vec3 light_position = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
	glm::vec3 light_direction = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };

	glm::vec3 vposition = mv * glm::vec4{ ivertex.position, 1 };
	glm::vec3 light_dir = glm::normalize(light_position - vposition); // Point Light
	//glm::vec3 light_dir = glm::normalize(light_direction); // Directional Light

	float intensity = std::max(0.0f, glm::dot(light_dir, overtex.normal));
	color3_t light_color = Shader::uniforms.light.color;

	color3_t diffuse = light_color * intensity;
	overtex.color = Shader::uniforms.ambient + diffuse;
#endif // 0
}
