/***********************************************
 Geometry shader to convert lines into triangle strips
 Memo Akten
 www.memo.tv

************************************************/

#version 330
layout (lines_adjacency) in;
layout (triangle_strip, max_vertices=7) out;
 
uniform mat4 modelViewProjectionMatrix;
/*layout (std140) uniform Matrices {
    mat4 projModelViewMatrix;
    mat3 normalMatrix;
};*/
 
in VertexData {
	vec4 color;
} VertexIn[4];
 
out VertexData {
    vec4 color;
} VertexOut;

uniform float thickness;

const float zero_length = 0.01;

vec3 normalizedAdjacentDirection(vec3 adjacent, vec3 current) {
	/* If the length of adjacent vector is to small (step()  will return 0.0)
		then we use the current vector as the direction vector
		otherwise we use the adjacent vector. 
		The mix() does this choice as the step() will only return 0. or 1.*/
	return normalize( mix( current , adjacent, step( 0.01, length( adjacent )) ) );
}



void main() {
	vec3 prev = gl_in[0].gl_Position.xyz;
	vec3 begin = gl_in[1].gl_Position.xyz;
	vec3 end = gl_in[2].gl_Position.xyz;
	vec3 next = gl_in[3].gl_Position.xyz;
	
	vec3 up = vec3(0, 0, 1);	// arbitrary up vector

	vec3 current_dir = normalize(end - begin);	// direction of segment to draw

	/*compute direction of previous segment; 
	  if begin and previous points match, then set the direction to segment direction */
	vec3 prev_dir = normalizedAdjacentDirection( (begin - prev), current_dir );
	vec3 next_dir = normalizedAdjacentDirection( (next - end), current_dir );
	
	vec3 prev_ortho = cross(prev_dir, up);
	vec3 current_ortho = cross(current_dir, up);
	vec3 next_ortho = cross(next_dir, up);

	//vec3 begin_v = normalize(current_ortho+prev_ortho);
	//vec3 begin_v = thickness * current_ortho;
	//if( length(prev_dir + current_dir)>0.2 ){
	vec3 begin_v = normalize( cross( prev_dir + current_dir, up) );
	float begin_miter = dot(begin_v,current_ortho);
	begin_miter = sign(begin_miter)*clamp(abs(begin_miter),0.1,1.0);
	begin_v *= (thickness / begin_miter);
	
	vec3 end_v = normalize(current_ortho+next_ortho);
	float end_miter = dot(end_v,current_ortho);
	end_v *= (thickness / end_miter);
	
	vec3 right = normalize(cross(current_dir, up));	// right vector
	vec3 norm = cross(right, current_dir);
	
	prev_ortho *= thickness;
	current_ortho *= thickness;
	next_ortho *= thickness;
	right *= thickness;

	
	gl_Position = modelViewProjectionMatrix * vec4(begin + begin_v, 1.0);
	VertexOut.color = VertexIn[1].color;
	EmitVertex();
	
	
	gl_Position = modelViewProjectionMatrix * vec4(begin - begin_v, 1.0);
	VertexOut.color = VertexIn[1].color;
	EmitVertex();
	
	gl_Position = modelViewProjectionMatrix * vec4(end + end_v, 1.0);
	VertexOut.color = VertexIn[1].color;
	EmitVertex();

	gl_Position = modelViewProjectionMatrix * vec4(end - end_v, 1.0);
	VertexOut.color = VertexIn[1].color;
	EmitVertex();

	
	EndPrimitive();

}