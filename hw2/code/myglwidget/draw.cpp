#include "myglwidget.h"
#include "../glm/gtc/type_ptr.hpp"

void MyGLWidget::drawObj(const Handles & handles){
  glUseProgram(handles.program);
	// 设置 shader 参数, 尽量减少重复计算, 将只需要算一次的放在 CPU 完成

  glm::mat4 model = glm::mat4(1.0f);
  // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));  // 将初始位置置于沿 z 轴负方向平移 10 个单位
  // glm::mat4 view = glm::mat4(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(handles.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
  vec3 camLookAt = objModel.centralPoint;  // 观察点为模型中心
  vec3 camUp = vec3(0, 1, 0);              // 上方向向量
  vec3 camPosition = vec3(
    objModel.centralPoint.x + camDistance * cos(degree * 3.14 / 180.0),
    objModel.centralPoint.y,
    objModel.centralPoint.z + camDistance * sin(degree * 3.14 / 180.0)
  );
  glm::mat4 view = glm::lookAt(camPosition, camLookAt, camUp);
  glUniformMatrix4fv(glGetUniformLocation(handles.program, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glm::mat4 projection = glm::perspective(
    glm::radians(40.0f),
    static_cast<float>(width()) / height(),
    0.1f, 300.0f
  );
  glUniformMatrix4fv(glGetUniformLocation(handles.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  vec3 lightPosition = objModel.centralPoint + vec3(0,100,100);
  glUniform3fv(glGetUniformLocation(handles.program, "lightPos"), 1, glm::value_ptr(lightPosition));
  glUniform3fv(glGetUniformLocation(handles.program, "camPos"), 1, glm::value_ptr(camPosition));
  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
  glUniform3fv(glGetUniformLocation(handles.program, "lightColor"), 1, glm::value_ptr(lightColor));
  vec3 objectColor = vec3(1.0f, 0.7f, 0.7f);
  glUniform3fv(glGetUniformLocation(handles.program, "objectColor"), 1, glm::value_ptr(objectColor));
  float ambientStrength = 0.3;
  vec3 ambient = ambientStrength * lightColor;
  glUniform3fv(glGetUniformLocation(handles.program, "ambient"), 1, glm::value_ptr(ambient));

  // 开始绘制物体
  glBindVertexArray(handles.VAO);
  glDrawArrays(GL_TRIANGLES, 0, handles.verticesCount);
  glBindVertexArray(0);
}
