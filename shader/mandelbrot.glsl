#extension GL_ARB_gpu_shader_fp64 : enable

varying dvec2 txcoords;
uniform unsigned int iter;

void main() {
	double z_real = 0;
	double c_real = txcoords.x; //* 4.0;
	double z_imag = 0;
	double c_imag = txcoords.y; // * ratio * 4.0;
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
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	} else {
		gl_FragColor = vec4(float(i) / float(iter), 0.0, 0.0, 1.0);
	}
}
