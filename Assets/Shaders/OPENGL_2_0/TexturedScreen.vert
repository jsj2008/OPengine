attribute vec3 aPosition;
attribute vec2 aUV;

uniform mat4 uWorld;

varying vec2 vUV;

void main() {
	vec4 screenPos = uWorld * vec4(aPosition,1);
	gl_Position = screenPos;

	vUV = aUV;
}
