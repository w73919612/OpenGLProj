const char* vertexShaderCode =
"#version 430\r\n"
"\n"
"in layout(location=0) vec2 position;\n"
"in layout(location=1) vec3 vertexColor;\n"
"\n"
"out vec3 theColor;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 0, 1.0);\n"
"    theColor = vertexColor;\n"
"}\n";

const char* fragmentShaderCode =
"#version 430\r\n"
"\n"
"out vec4 daColor;\n"
"in vec3 theColor;\n"
"\n"
"void main()\n"
"{\n"
"    daColor = vec4(theColor, 1.0);\n"
"}\n";