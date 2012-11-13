#version 400 core
#extension GL_ARB_gpu_shader_fp64 : enable

in vec2 txcoords;
uniform double zoom;
uniform dvec2 offset;
out vec4 fragColor;
uniform unsigned int iter;

uniform sampler1D palette;

void main() {
	double c_real = double(txcoords.x) * zoom + offset.x;
	double z_real = c_real;
	double c_imag = double(txcoords.y) * zoom + offset.y;
	double z_imag = c_imag;
	double z_r_q = 0;
	double z_i_q = 0;
	int i;
	for(i = 0; i < iter; ++i) {
		z_r_q = z_real * z_real;	// z_real^2
		z_i_q = z_imag * z_imag;	// z_imag^2
		if(z_r_q + z_i_q > 4.0) {
			break;
		}
		z_imag = 2.0 * z_real * z_imag + c_imag;
		z_real = z_r_q - z_i_q + c_real;
	}
	if(i == iter) {	// Check whether value was inside the set
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		float ni = float(i) + 1.0 - log(log(float(sqrt(z_r_q + z_i_q)))) / log(2.0);
		//fragColor = vec4(0.0, 0.0, ni / float(iter), 1.0);
		fragColor = texture(palette, abs(ni / float(iter)));
	}
}
