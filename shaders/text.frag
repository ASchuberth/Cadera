#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragTextColor;
layout(location = 1) in vec4 fragBackgroundColor;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;


float median(float a, float b, float c) {
    return max(min(a, b), min(max(a, b), c));
}

void main()
{

	vec3 msd = texture(texSampler, fragTexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = 2.0 * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	outColor = mix(fragBackgroundColor, fragTextColor, opacity);

}
