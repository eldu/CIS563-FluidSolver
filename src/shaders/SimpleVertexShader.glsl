#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 cameraMat;

void main(){

	// Output position of the vertex, in clip plane
	// cameraMat * position
    gl_Position =  cameraMat * vec4(vertexPosition_modelspace,1);
}
