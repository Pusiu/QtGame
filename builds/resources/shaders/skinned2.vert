attribute vec4 vertex;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec2 tangent;
attribute vec4 boneDataA;
attribute vec4 boneDataB;

uniform mat4 gBones[200];
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
    mat4 BoneTransform = gBones[uint(boneDataA.x)] * boneDataA.z +
                         gBones[uint(boneDataA.y)] * boneDataA.w +
                         gBones[uint(boneDataB.x)] * boneDataB.z +
                         gBones[uint(boneDataB.y)] * boneDataB.w ;



    //vertex.w=1;
    vec4 pos = BoneTransform * vertex;
    //vec4 pos = vertex;

    fragNormal = (modelMatrix * vec4(normal, 0)).xyz;
    fragUV=texCoord;
    vertexWorldSpace = (modelMatrix * pos).xyz;
    gl_Position = projMatrix * viewMatrix * modelMatrix * pos;
}
