uniform highp vec3 color;
uniform highp int hasTexture;
uniform sampler2D texture;
uniform highp vec2 resolution;
uniform highp float alpha;


varying vec2 fragUV;

struct Rectangle
{
 float xPos;
 float yPos;
 float width;
 float height;
};
uniform Rectangle rect;


void main() {

    //rotate texture
    float sin_factor = sin(90*(3.14/180));
    float cos_factor = cos(90*(3.14/180));
    fragUV = vec2((fragUV.x - 0.5) * (800 / 600), fragUV.y - 0.5) * mat2(cos_factor, sin_factor, -sin_factor, cos_factor);
    fragUV += 0.5;

    highp vec4 tex = texture2D(texture, fragUV).rgba;
    tex.a=alpha;


    if(hasTexture == 1)
        gl_FragColor = vec4(tex);
    else
    {
        gl_FragColor = vec4(1,1,1,1);
    }
}
