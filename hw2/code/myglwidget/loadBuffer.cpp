#include "myglwidget.h"
#include "../utils.h"


void MyGLWidget::loadBuffer(Model objModel, Handles & handles){
  //绑定数组指针
  glBindVertexArray(handles.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, handles.VBO);
  
  //写入顶点 + 法向量数据
  handles.verticesCount = objModel.triangleCount * 3;
  std::vector<float> buffer;
  for(int i = 0; i < objModel.triangleCount; i++){
    Triangle tri = objModel.getTriangleByID(i);
    for(int j = 0; j < 3; j++){
			int vertexIndex = objModel.triangles[i][j] ; 
			int normalIndex = objModel.triangle_normals[i][j] ;
			buffer.push_back(objModel.vertices_data[vertexIndex].x);
			buffer.push_back(objModel.vertices_data[vertexIndex].y);
			buffer.push_back(objModel.vertices_data[vertexIndex].z);
			buffer.push_back(objModel.normals_data[normalIndex].x);
			buffer.push_back(objModel.normals_data[normalIndex].y);
			buffer.push_back(objModel.normals_data[normalIndex].z);
    }
  }
  glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_STATIC_DRAW);

  //设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  // 设置法向量属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  //解绑VAO
  glBindVertexArray(0);
}
