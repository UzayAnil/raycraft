#version 400

layout(location = 0) in vec2 position;
out vec2 _position;

void main()
{
	_position = position;
	gl_Position = vec4(position, 0.0, 1.0);
}