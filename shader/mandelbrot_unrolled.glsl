#version 150 core
#extension GL_ARB_gpu_shader_fp64 : enable

in vec2 txcoords;
uniform double zoom;
uniform dvec2 offset;
out vec4 fragColor;
uniform int iter;

uniform sampler1D palette;

void main() {
	double c_real = double(txcoords.x) * zoom + offset.x;
	double z_real = c_real;
	double c_imag = double(txcoords.y) * zoom + offset.y;
	double z_imag = c_imag;
	double z_r_q = 0;
	double z_i_q = 0;
	double z_imag_2 = z_real * z_imag * 2.0 + c_imag;
	double z_real_2 = z_real * z_real - z_imag * z_imag + c_real;
	int i;
	float ni;
	for(i = 0; i < iter; i = i+2) {
		z_r_q = z_real_2 * z_real_2;	// z_real^2
		z_i_q = z_imag_2 * z_imag_2;	// z_imag^2
		if(z_r_q + z_i_q > 4.0) {
			break;
		}
		z_imag = 2.0 * z_real_2 * z_imag_2 + c_imag;
		z_real = z_r_q - z_i_q + c_real;
		z_imag_2 = z_real * z_imag * 2.0 + c_imag;
		z_real_2 = z_real * z_real - z_imag * z_imag + c_real;
	}
	if(z_imag * z_imag + z_real * z_real > 4.0) {
		--i;	// Step one iteration back

		ni = float(i) + 1.0 - log(log(float(sqrt(z_imag * z_imag + z_real * z_real)))) / 0.69315; // 0.69315 = log(2.0)
	} else {
		ni = float(i) + 1.0 - log(log(float(sqrt(z_r_q + z_i_q)))) / 0.69315; // 0.69315 = log(2.0)
	}
	if(i >= iter) {	// Check whether value was inside the set
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		fragColor = texture(palette, abs(ni / float(iter)));
	}
}
