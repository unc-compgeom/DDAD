#version 330

/*
 * Unlit fragment shader
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2014-03-25
 */

// input: fragment color
in vec4 f_color;

void main() {
    gl_FragColor = f_color;
}
