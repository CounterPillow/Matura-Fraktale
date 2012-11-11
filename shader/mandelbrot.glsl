#version 400 core
#extension GL_ARB_gpu_shader_fp64 : enable
#extension GL_NV_gpu_shader5 : enable // it told me I needed this.

in vec2 txcoords;
uniform double zoom;
uniform dvec2 offset;
out vec4 fragColor;
uniform unsigned int iter;

void main() {
	double z_real = 0;
	double c_real = double(txcoords.x) * zoom + offset.x;
	double z_imag = 0;
	double c_imag = double(txcoords.y) * zoom + offset.y;
	int i;
	for(i = 0; i < iter; ++i) {
		if(z_real * z_real + z_imag * z_imag > 4.0) {
			break;
		}
		double old_z_r = z_real;
		double old_z_i = z_imag;
		z_real = old_z_r * old_z_r - old_z_i * old_z_i + c_real;
		z_imag = 2.0 * old_z_r * old_z_i + c_imag;
	}
	if(i == iter) {	// Check whether value was inside the set
		fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	} else {
		fragColor = vec4(float(i) / float(iter), 0.0, 0.0, 1.0);
	}
}
