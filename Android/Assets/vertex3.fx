﻿attribute vec3 vPosition; attribute vec2 TexCoordIn; uniform mat4 Model;varying vec2 TexCoordOut; void main() {	vec4 screenPos = Model * vec4(vPosition,1);	gl_Position = screenPos; 	TexCoordOut = TexCoordIn; 	}
