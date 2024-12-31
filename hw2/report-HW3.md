<div class="cover" style="page-break-after:always;font-family:方正公文仿宋;width:100%;height:100%;border:none;margin: 0 auto;text-align:center;">
    <div style="width:70%;margin: 0 auto;height:0;padding-bottom:10%;">
        </br>
        <img src="../sysu-name.jpg" alt="校名" style="width:100%;"/>
    </div>
    </br></br></br></br>
    <div style="width:60%;margin: 0 auto;height:0;padding-bottom:40%;">
        <img src="../sysu.jpg" alt="校徽" style="width:100%;"/>
	</div>
</br></br></br></br></br></br></br></br></br>
    <span style="font-family:华文黑体Bold;text-align:center;font-size:20pt;margin: 10pt auto;line-height:30pt;">本科生实验报告</span>
    </br>
    </br>
    <table style="border:none;text-align:center;width:72%;font-family:仿宋;font-size:14px; margin: 0 auto;">
    <tbody style="font-family:方正公文仿宋;font-size:12pt;">
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:center;">实验课程</td>
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">计算机图形学第三次作业</td>
      </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:center;">专业名称</td>
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">计算机科学与技术</td>
      </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:center;">学生姓名</td>
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">李世源</td>
      </tr>
    	<tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:center;">学生学号</td>
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">22342043</td>
      </tr>
      <tr style="font-weight:normal;"> 
    		<td style="width:20%;text-align:center;">提交时间</td>
    		<td style="width:40%;font-weight:normal;border-bottom: 1px solid;text-align:center;font-family:华文仿宋">2024年12月31日</td>
      </tr>
    </tbody>              
    </table>
</div>

# 项目程序介绍

程序运行后的控制按键定义如下：

```cpp cpp
void MyGLWidget::keyPressEvent(QKeyEvent *e) {
  switch (e->key()) {
    case Qt::Key_Q:
      scene = (scene + 1) % 5;
      switch(scene){
        case 0 : objModel.loadModel("./objs/singleTriangle.obj");
        case 1: objModel.loadModel("./objs/teapot_600.obj"); break;
        case 2: objModel.loadModel("./objs/teapot_8000.obj"); break;
        case 3: objModel.loadModel("./objs/rock.obj"); break;
        case 4: objModel.loadModel("./objs/cube.obj"); break;
        default : objModel.loadModel("./objs/singleTriangle.obj");
      }
      break;
    case Qt::Key_E: degree += 1; break;
    case Qt::Key_W: camDistance = camDistance == 0 ? 0 : camDistance - 10; break;
    case Qt::Key_S: camDistance = camDistance == 300 ? 0 : camDistance + 10; break;
  }
  update();
}
```

控制按键：
- `Q` 切换物体
- `E` 旋转物体（实际代码实现是相机绕着物体旋转）
- `W` 相机靠近物体
- `S` 相机远离物体

`myglwidget\myglwidget.h` 中我设置了一个新数据结构 `Handles` 存储和 GPU 交互时的一些必要句柄、变量：

```cpp cpp
struct Handles {
  GLuint program;
  GLuint VBO;
  GLuint VAO;
  GLsizei verticesCount;
};
```

# 通过 fragment shader 实现 Phong shading

## 在 vertex shader 中输出法向量

顶点着色器中输入模型读取的原始顶点和法向量数据，也就是模型空间顶点位置和模型空间法向量。

同时设置 uniform 变量的 model, view, projection 矩阵。也就是：
- **模型矩阵** $model$：将物体坐标系转换到世界坐标系
- **视图矩阵** $view$：将世界坐标系转换到视图坐标系（相机坐标系）
- **投影矩阵** $proj$：将视图坐标系转换到裁剪坐标系

模型矩阵用来将模型空间的顶点位置和法向量转化为世界空间的顶点位置和法向量，输出到片元着色器作为 Phong 算法的输入。

顶点的最终位置通过将模型、视图和投影矩阵相乘来计算。数学表达式如下：

给定一个顶点位置 $P$，齐次坐标表示为：

$$
\begin{pmatrix} x \\ y \\ z \\ 1 \end{pmatrix}
$$

最终位置 $P'$ 计算为：

$$
P' = proj \times view \times model \times P
$$

这里除了法向量，还需要将上述变换过程通过输入到 `gl_Position` 这个特殊变量实现。最终片元着色器的输出位置也会由这个决定。

```cpp cpp
#version 450 core

layout(location = 0) in vec3 aPos;     // 模型空间顶点位置
layout(location = 1) in vec3 aNormal;  // 模型空间法向量

out vec3 FragPos;     // 世界空间片段位置
out vec3 FragNormal;  // 世界空间法向量

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  FragPos = vec3(model * vec4(aPos, 1.0));
  FragNormal = mat3(transpose(inverse(model))) * aNormal;
  gl_Position = projection * view * vec4(FragPos, 1.0);  
}
```

GLSL 会自动插值并输入 fragment shader。

## 在 fragment shader 中通过 Phonq shading 计算三类反射

Phong 的原理和应用在上一个实验中已经阐述，这里不再赘述。

这里环境反射，也就是环境光由于是固定常量，所以通过 uniform 形式输入。另外两种漫反射和镜面反射则具体计算如下：

```cpp cpp
#version 450 core

in vec3 FragPos;      // 世界空间片段位置
in vec3 FragNormal;   // 世界空间法向量
out vec4 FragColor;   // 输出颜色

uniform vec3 lightPos;    // 光源位置
uniform vec3 camPos;      // 相机位置
uniform vec3 lightColor;  // 光源颜色
uniform vec3 objectColor; // 物体颜色
uniform vec3 ambient;     // 环境光

void main()
{
  // 漫反射
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  // 镜面反射
  float specularStrength = 0.5;
  vec3 viewDir = normalize(camPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  // 最终颜色
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}
```

# 使用 VBO 存储顶点与连接关系

使用先前实验的 `Model` 类，对一个已经加载好数据的模型，我在 `loadBuffer.cpp` 中将数据读入 VBO 并设置好顶点、法向量两个数据的 VAO。实现如下：

```cpp cpp
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
```

# 使用多个细分迭代次数讨论以下内容

## 对比 Phong shading 与 OpenGL 自带的 smoothing shading 的区别

期望的现象是对于顶点和面较少的模型，smoothing shading 更具有优势；对于顶点和面较少的模型，自己编写的 Phong shading 更具有优势。

使用 smoothing shading，顶点着色器程序就计算输出颜色：

```cpp cpp
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 fragColor;  // 输出颜色

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
  // 计算法线和光照
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - position);
  // 漫反射光照
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  // 计算最终颜色
  fragColor = (diffuse + objectColor) / 2.0;
  gl_Position = vec4(position, 1.0);
}
```

顶点着色器负责计算每个顶点的颜色。片元着色器使用插值后的颜色进行最终的片段颜色输出：

```cpp cpp
#version 450 core
in vec3 fragColor; // 接收来自顶点着色器的颜色
out vec4 color;

void main() {
  // 直接使用插值后的颜色
  color = vec4(fragColor, 1.0);
}
```

Phong shading：
- 计算位置：光照计算发生在片元（像素）级别，使用插值法将顶点法线插值到片元。
- 效果：提供更高质量的视觉效果，尤其在高光和细节方面更为真实。
- 性能：由于片元级别的计算，计算量较大，可能对性能产生影响。

OpenGL 自带的 Smoothing Shading：
- 计算位置：光照计算发生在顶点级别，输出颜色后进行插值。
- 效果：效果较为简单，适合实时渲染，但在高光等细节处可能不够准确。
- 性能：计算量较小，适用于需要快速渲染的场景。

## 使用 VBO 进行绘制及通过 glVertex 进行绘制的区别

实际上，上一个实验中就是使用 glVertex 进行绘制，当时就可以看出性能比较糟糕。此时所有的缓存都在内存里，所有的颜色信息都在 CPU 完成，没有任何 GPU 并行的提速所以性能相当差。

VBO 进行绘制的特点：
- 性能：VBO 可以将顶点数据存储在 GPU 内存中，仅在创建和填充 VBO 需要一定的初始化时间，后续无需再填充数据，减少 CPU-GPU 之间的数据传输，提高渲染性能。
- 批量绘制：通过一次性传输大量顶点数据，可以有效减少绘制调用的次数。
- 灵活性：支持更复杂的数据结构（如法线、纹理坐标等），可操纵性更强。

`glVertex` 进行绘制的特点：
- 简单：适合简单场景或快速原型开发，代码更直接。
- 即时反馈：数据可以随时更改，适合动态场景。
- 性能低下：每次调用 `glVertex` 都需要 CPU 和 GPU 之间的通信，性能较差。
- 限制性：不适合处理大量顶点数据，尤其是在复杂场景中。

## 讨论 VBO 中是否使用 index array 的效率区别

期望的现象是对于顶点和面较少的模型，不使用 Index Array 更具有优势；对于顶点和面较少的模型，使用 Index Array 更具有优势。

使用 Index Array：
- 内存节省：多个顶点共享相同的顶点数据（如在一个三角形中），可以减少内存使用。
- 减少顶点重复：避免在绘制相同的顶点时重复发送数据，提高效率。
- 提高性能：索引数组可以降低顶点缓存（cache）未命中的概率，提高 GPU 性能。

不使用 Index Array：
- 简单性：对于简单或小型模型，直接使用顶点数组可能更容易实现。
- 动态绘制：适合频繁变化的顶点数据，因为不需要维护索引。
- 内存浪费：相同的顶点会被多次存储，增加内存开销。
- 性能下降：重复传输相同的顶点数据会增加 CPU 和 GPU 通信的开销。

# 关于在 CPU 还是 GPU 计算的讨论

在我一开始的实现，我尝试在顶点着色程序中进行旋转变换，并将必要的信息输出给片元着色器，包括：
- `FragPos` 世界空间片段位置
- `Normal` 世界空间法向量
- `VertColor` 物体顶点颜色
- `lightPos` 光源的世界空间坐标
- `camPos` 相机位置
- `lightColor` 光源颜色

```cpp cpp
#version 450 core
layout(location = 0) in vec3 aPos;     // 模型空间顶点位置
layout(location = 1) in vec3 aNormal;  // 模型空间法向量

out vec3 FragPos;     // 世界空间片段位置
out vec3 Normal;      // 世界空间法向量
out vec3 VertColor;   // 物体顶点颜色
out vec4 lightPos;    // 光源的世界空间坐标
out vec3 camPos;      // 相机位置
out vec3 lightColor;  // 光源颜色

uniform mat4 projection;  // PROJECTIN 矩阵
uniform mat4 model;       // MODEL 矩阵
uniform mat4 view;        // VIEW 矩阵
uniform float angle;      // 物体旋转角度
uniform vec3 camPos;      // 相机位置
uniform vec3 lightColor;  // 光源颜色

void main(){
  float s = sin(angle * length(aPos.xy));
  float c = cos(angle * length(aPos.xy));
  vec3 Pos = vec3(
    Pos.x = c * aPos.x - s * aPos.y,
    Pos.y = s * aPos.x + c * aPos.y,
    Pos.z = aPos.z
  );
  gl_Position = projection * view * model * vec4(Pos, 1.0);
  VertColor = (vec3(gl_Position.xyz) + vec3(0.1f,0.1f,0.1f)) * 0.5f;      // 设置物体顶点颜色
  FragNormal = mat3(transpose(inverse(model))) * normal;                  // 计算法向量经过模型变换后值
  FragPos = vec3(model * vec4(aPos, 1.0));                                // 计算顶点在世界坐标系中的位置
  lightPos = view * model * vec4(-2.0f, -2.0f, 0.0f,1.0f);       	        // 光源的位置
}
```

此外，在片元着色器我还按照 Phong 算法计算了环境光：

```cpp cpp
#version 450 core
in vec3 FragPos;     // 世界空间片段位置
in vec3 Normal;      // 世界空间法向量
in vec3 VertColor;   // 物体顶点颜色
in vec4 lightPos;    // 光源的世界空间坐标
in vec3 camPos;      // 相机位置
in vec3 lightColor;  // 光源颜色
out vec4 FragColor;  // 最终渲染的颜色

void main()
{
  // 环境光
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor * vertColor;
  // 漫反射
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  // 镜面反射
  float specularStrength = 0.5;
  vec3 viewDir = normalize(camPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;
  // 最终颜色
  vec3 result = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(result, 1.0);
}
```

后面发现有一些计算是可以提前在 CPU 完成的，不必在 GPU 每一个顶点或片元都计算一次。顶点输出的 `VertColor` 也不是必要的，也不必将光照信息交给顶点着色器，只需要把计算最终位置的任务交给顶点着色器就行。旋转角度就是可以先计算好放在 `view` 矩阵里面，在 CPU 就可以一次性完成。而在片元着色器中，也不必再计算一次环境光，因为环境光本身就定义为常量，直接通过 uniform 传递即可。至于其他变量包括光源位置、相机位置、光源颜色、物体颜色都是可以将 CPU 计算好的结果直接通过 uniform 传递即可。这样就可以大大提高效率。最终我再 CPU 完成的计算以及传递 uniform 参数过程如下：

```cpp cpp
void MyGLWidget::drawObj(const Handles & handles){
  glUseProgram(handles.program);
  glm::mat4 model = glm::mat4(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(handles.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
  vec3 camLookAt = objModel.centralPoint;  // 观察点为模型中心
  vec3 camUp = vec3(0, 1, 0);              // 上方向向量
  vec3 camPosition = vec3(
    objModel.centralPoint.x + 70 * cos(degree * 3.14 / 180.0),
    objModel.centralPoint.y,
    objModel.centralPoint.z + 70 * sin(degree * 3.14 / 180.0)
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
```