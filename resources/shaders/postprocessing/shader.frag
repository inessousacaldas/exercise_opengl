#version 330 core

in vec2 uv;
out vec4 finalColor;

uniform sampler2D scene;

uniform vec2 offsets[9];
uniform float blurKernel[9];

uniform bool blur;
uniform bool shake;

uniform int height;

void main() {

    finalColor = vec4(0.0f);
    vec3 textureSample[9];

    if (shake || blur) 
	{
        for(int i = 0; i < 9; i++) 
		{
            textureSample[i] = vec3(texture(scene, uv.st + offsets[i]));    
        }
    }

	if (blur) 
	{
        for(int i = 0; i < 9; i++) 
		{
			if(gl_FragCoord.y < height)
			{

            	finalColor += vec4(textureSample[i] * blurKernel[i], 0.0f);
			}
			else 
			{
				finalColor =  texture(scene, uv);
			}
        }
		finalColor.a = 1.0f;
    }

    // process effects
    if (shake) 
	{
        for(int i = 0; i < 9; i++) 
		{
            finalColor += vec4(textureSample[i] * blurKernel[i], 0.0f);
        }
        finalColor.a = 1.0f;
    }


    // no effects
    if (!blur && !shake) 
	{
        finalColor =  texture(scene, uv);
    }
}
