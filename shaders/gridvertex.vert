#version 450
#extension GL_ARB_separate_shader_objects : enable



vec4 quat_from_axis_angle(vec3 axis, float angle)
{ 
  vec4 qr;
  float half_angle = (angle * 0.5) * 3.14159 / 180.0;
  qr.x = axis.x * sin(half_angle);
  qr.y = axis.y * sin(half_angle);
  qr.z = axis.z * sin(half_angle);
  qr.w = cos(half_angle);
  return qr;
}

vec4 quat_conj(vec4 q)
{ 
  return vec4(-q.x, -q.y, -q.z, q.w); 
}
  
vec4 quat_mult(vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);
  return qr;
}

vec3 rotate_vertex_position(vec3 position, vec3 axis, float angle)
{ 
  vec4 qr = quat_from_axis_angle(axis, angle);
  vec4 qr_conj = quat_conj(qr);
  vec4 q_pos = vec4(position.x, position.y, position.z, 0);
  
  vec4 q_tmp = quat_mult(qr, q_pos);
  qr = quat_mult(q_tmp, qr_conj);
  
  return vec3(qr.x, qr.y, qr.z);
}




layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;





layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inTranslate;
layout(location = 3) in vec3 axis;
layout(location = 4) in float angle;


layout(location = 0) out vec4 fragColor;

out gl_PerVertex {
	vec4 gl_Position;
};



void main()
{

	vec3 P = rotate_vertex_position(inPosition + inTranslate, axis, angle);

	vec4 pos = vec4(P, 1.0);
	gl_Position = ubo.proj * ubo.view * ubo.model * pos;
	
	if (gl_InstanceIndex == 200) {
		fragColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else if (gl_InstanceIndex == 201) {
		fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (gl_InstanceIndex % 4 < 2) {
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		fragColor = vec4(inColor.xyz, 0.5);
	}
}
