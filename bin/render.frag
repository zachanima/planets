#version 140


out vec4 color;

uniform vec3 inColor;

void main(void) {
  if (inColor == vec3(0.f, 0.f, 0.f)) {
    color = vec4(1.f, 1.f, 1.f, 1.f);
  } else {
    color = vec4(inColor, 1.f);
  }
}
