#include "myglwidget.h"

Handles MyGLWidget::initShader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;
  std::stringstream vertexStream;
  std::stringstream fragmentStream;
  const char *vertexShaderSource;
  const char *fragmentShaderSource;
  Handles handles;

  // 将相对路径转化成绝对路径
  std::string vertexDir = QDir::currentPath().toStdString() + "/" + vertexPath;
  std::string fragmentDir = QDir::currentPath().toStdString() + "/" + fragmentPath;

  //加载顶点着色器代码
  vertexShaderFile.open(vertexDir);
  vertexStream << vertexShaderFile.rdbuf();
  vertexShaderFile.close();

  //加载片元着色器代码
  fragmentShaderFile.open(fragmentDir);
  fragmentStream << fragmentShaderFile.rdbuf();
  fragmentShaderFile.close();

  //字符转换
  vertexCode = vertexStream.str();
  fragmentCode = fragmentStream.str();
  vertexShaderSource = vertexCode.c_str();
  fragmentShaderSource = fragmentCode.c_str();

  //编译顶点着色器，在控制台输出LOG
  int success;
  char infoLog[512];
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
    std::cout << "error in vertexshader: compilation failed\n" << infoLog << std::endl;
  }
  else
    std::cout << vertexPath <<" compiled successfully" << std::endl;

  //编译片元着色器，在控制台输出LOG
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
    std::cout << "error in fragmentshader: compilation failed\n" << infoLog << std::endl;
  }
  else
    std::cout << fragmentPath << " compiled successfully" << std::endl;

  //绑定并链接着色器，在控制台输出LOG
  handles.program = glCreateProgram();
  glAttachShader(handles.program, vertShader);
  glAttachShader(handles.program, fragShader);
  glLinkProgram(handles.program);
  glGetProgramiv(handles.program, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(handles.program, 512, NULL, infoLog);
    std::cout << "error: link failed\n" << infoLog << std::endl;
  }
  else
    std::cout << "Program " << handles.program << " link successfully" << std::endl;

  //清理
	glDetachShader(handles.program, vertShader);
	glDetachShader(handles.program, fragShader);
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  //创建并绑定 VAO, VBO 指针
	glGenVertexArrays(1, &handles.VAO);
	glGenBuffers(1, &handles.VBO);

	return handles;
}