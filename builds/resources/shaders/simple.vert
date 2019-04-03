attribute vec4 vertex;
attribute vec3 normal;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
varying highp vec3 fragNormal;
varying highp vec3 vertexWorldSpace;

struct Light {
    highp vec3 position;
    highp vec3 ambient;
    highp vec3 diffuse;
};
uniform Light light;

void main() {
    //fragNormal = normal;
    fragNormal = (modelMatrix * vec4(normal, 0)).xyz;
    vertexWorldSpace = (modelMatrix * vertex).xyz;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vertex;
}
