#version 400
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
uniform highp float variance;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(FragColor.x, FragColor.y*variance, FragColor.z, FragColor.w);
}