#version 110

attribute vec3 position;
attribute vec2 tex_coord;
varying vec2 texcoord;

uniform mat4 proj_matrix;

void main()
{
	gl_Position = proj_matrix * vec4(position, 1.0);
	texcoord = tex_coord;
}
