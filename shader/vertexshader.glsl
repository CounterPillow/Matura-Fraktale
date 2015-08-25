#version 150 core
#extension GL_ARB_gpu_shader_fp64 : enable
in vec2 position;

out vec2 txcoords;

uniform float ratio;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0);
	txcoords = position;
	txcoords.y *= ratio;
}
