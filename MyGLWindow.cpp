#include <gl/glew.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include "MyGLWindow.h"
using namespace std;

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

bool checkStatus(GLuint objectID,
  PFNGLGETSHADERIVPROC objectPropertyGetter,
  PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
  GLenum statusType)
{
  GLint status;
  glGetShaderiv(objectID, statusType, &status);
  if (status != GL_TRUE)
  {
    GLint infoLogLength;
    objectPropertyGetter(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* buffer = new GLchar[infoLogLength];

    GLsizei bufferSize;
    getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
    cout << buffer << endl;

    delete[] buffer;
    return false;
  }
  return true;
}

bool checkShaderStatus(GLuint shaderID)
{
  return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
  return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
  ifstream meInput(fileName);
  if (!meInput.good())
  {
    cout << "File failed to load... " << fileName << endl;
    exit(1);
  }
  return std::string(
    std::istreambuf_iterator<char>(meInput),
    std::istreambuf_iterator<char>());
}

void installShaders()
{
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar* adapter[1];

  //adapter[0] = vertexShaderCode;
  string temp = readShaderCode("VertexShaderCode.glsl");
  adapter[0] = temp.c_str();
  cout << adapter[0] << endl;
  glShaderSource(vertexShaderID, 1, adapter, 0);

  //adapter[0] = fragmentShaderCode;
  temp = readShaderCode("FragmentShaderCode.glsl");
  adapter[0] = temp.c_str();
  cout << adapter[0] << endl;
  glShaderSource(fragmentShaderID, 1, adapter, 0);

  glCompileShader(vertexShaderID);
  glCompileShader(fragmentShaderID);

  if (!checkShaderStatus(vertexShaderID)) {
    cout << "It was the Vertex Shader that failed." << endl;
    return;
  }
  if (!checkShaderStatus(fragmentShaderID)) {
    cout << "It was the Fragment Shader that failed." << endl;
    return;
  }

  GLuint programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);

  glUseProgram(programID);

  if (!checkProgramStatus(programID))
  {
    return;
  }
}

const float X_DELTA = 0.1f;
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
const uint MAX_TRIS = 20;
uint numTris = 0;

void sendDataToOpenGL()
{
  const float RED_TRIANGLE_Z = 0.5f;
  const float BLUE_TRIANGLE_Z = -0.5f;
  float verts[] =
  {
    //Vertices          //Color
    +0.0f, +1.0f, RED_TRIANGLE_Z,       +1.0f, +0.0f, +0.0f,
    -1.0f, -1.0f, RED_TRIANGLE_Z,       +0.0f, +1.0f, +0.0f,
    +1.0f, -1.0f, RED_TRIANGLE_Z,       +0.0f, +0.0f, +1.0f,

    +0.0f, -1.0f, BLUE_TRIANGLE_Z,       +0.0f, +0.0f, +1.0f,
    +1.0f, +1.0f, BLUE_TRIANGLE_Z,       +0.0f, +0.0f, +1.0f,
    -1.0f, +1.0f, BLUE_TRIANGLE_Z,       +0.0f, +0.0f, +1.0f
  };

  GLuint vertexBufferID;
  glGenBuffers(1, &vertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
                               //Just allocate bytes, and dont send any data (NULL)
  glBufferData(GL_ARRAY_BUFFER, MAX_TRIS * TRIANGLE_BYTE_SIZE, NULL, GL_STATIC_DRAW);

  // glClear(GL_COLOR_BUFFER_BIT);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

  GLushort indices[] = { 0,1,2, 3,4,5};

  GLuint indexBufferID;
  glGenBuffers(1, &indexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void DrawNewTriangleEachPaint()
{
  if (numTris == MAX_TRIS)
    return;
  const GLfloat THIS_TRI_X = -1 + numTris * X_DELTA;
  GLfloat thisTri[] =
  {
    THIS_TRI_X, 1.0f,0.0f,
    1.0f,0.0f,0.0f,

    THIS_TRI_X + X_DELTA, 1.0f, 0.0f,
    1.0f,0.0f,0.0f,

    THIS_TRI_X, 0.0f, 0.0f,
    1.0f,0.0f,0.0f,
  };
  glBufferSubData(GL_ARRAY_BUFFER, numTris * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, thisTri);
  numTris++;
}
void MyGLWindow::paintGL()
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width(), height());
  //glDrawArrays(GL_TRIANGLES, 0, 6);
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
  DrawNewTriangleEachPaint();
  glDrawArrays(GL_TRIANGLES, (numTris - 1) * NUM_VERTICES_PER_TRI, NUM_VERTICES_PER_TRI);
}
void MyGLWindow::initializeGL()
{
  GLenum errorCode = glewInit();
  assert(errorCode == 0);
  glEnable(GL_DEPTH_TEST);
  sendDataToOpenGL();
  installShaders();
}

