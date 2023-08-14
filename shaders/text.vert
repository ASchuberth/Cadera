#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;



layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inTextColor;
layout(location = 2) in vec3 inBackgroundColor;
layout(location = 3) in vec2 inTexCoord;


layout(location = 0) out vec4 fragTextColor;
layout(location = 1) out vec4 fragBackgroundColor;
layout(location = 2) out vec2 fragTexCoord;

out gl_PerVertex {
	vec4 gl_Position;
	float gl_PointSize;
};



void main()
{
	gl_PointSize = 7.0;
	vec4 pos = vec4(inPosition.xyz, 1.0);
	gl_Position = ubo.proj * ubo.view * ubo.model * pos;
	

	fragTextColor = vec4(inTextColor.xyz, 1.0);
	fragBackgroundColor = vec4(inBackgroundColor.xyz, 1.0);
	fragTexCoord = inTexCoord;
}
