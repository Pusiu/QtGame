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

struct Rectangle
{
 float xPos;
 float yPos;
 float width;
 float height;
};
uniform Rectangle rect;


void main() {
    //fragNormal = normal;
    fragNormal = (vec4(normal, 0)).xyz;
    fragUV=texCoord;
  // vertexWorldSpace = (modelMatrix * vertex).xyz;
   // gl_Position = projMatrix * viewMatrix * modelMatrix * vertex;

    gl_Position.x=(rect.width*vertex.x)/rect.width*1.0;
    gl_Position.y=(rect.height*vertex.y)/rect.height * 1.0;
    gl_Position.z=0;
    gl_Position.w=1;
}
