attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 texCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform highp int hasTexture;
uniform sampler2D texture;

varying highp vec3 fragNormal;
varying highp vec3 vertexWorldSpace;
varying highp vec2 fragUV;

struct Light {
    highp vec3 position;
    highp vec3 ambient;
    highp vec3 diffuse;
};
uniform Light light;

void main() {
    //fragNormal = normal;
    fragNormal = (modelMatrix * vec4(normal, 0)).xyz;
    fragUV=texCoord;
    vertexWorldSpace = (modelMatrix * vertex).xyz;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vertex;
}
