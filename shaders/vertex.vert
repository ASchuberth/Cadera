#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;



layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;


layout(location = 0) out vec4 fragColor;

out gl_PerVertex {
	vec4 gl_Position;
	float gl_PointSize;
};



void main()
{
	gl_PointSize = 7.0;
	vec4 pos = vec4(inPosition.xyz, 1.0);
	gl_Position = ubo.proj * ubo.view * ubo.model * pos;
	

	fragColor = vec4(inColor.xyz, 1.0);
}
