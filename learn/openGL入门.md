https://learnopengl-cn.github.io/

基本是跟着这个网站来的，感谢大佬们提供了这么好的资源



#### VBO，VAO

首先关注他们的名字：

- 顶点缓冲对象：Vertex Buffer Object，**VBO**
- 顶点数组对象：Vertex Array Object，**VAO**

每个顶点可能会包含一堆数据（常见的有坐标，颜色等），CPU需要把这些数据发送给显卡，为了一次性发送大量数据，会使用一个缓冲来暂存顶点数据，这就是VBO，它仅仅只是一堆本质上没区别的数据，具体如何访问这些数据，就是VAO决定的。

可以认为，VAO对VBO的数据定义了一种分类，组装的方式，将一堆数据拼成了有意义的一个数组，VAO就是把这种访问方式保存下来。

具体而言，我们可以创建一个VBO来保存东西：

```c++
unsigned int VBO;
//glGenXXX就是创建一个对象，为其分配空间
glGenBuffers(1, &VBO);
//将VBO绑定上缓冲GL_ARRAY_BUFFER，可以理解为设置状态：当前存储顶点的位置是VBO
glBindBuffer(GL_ARRAY_BUFFER, VBO);
//往一个缓冲中塞数据，这时的VBO已经绑定GL_ARRAY_BUFFER，这样就能保存在这个VBO当中
glBufferData(GL_ARRAY_BUFFER, sizeof(dataSource), dataSource, GL_STATIC_DRAW);
```

这样就有一块纯纯的数据了，可以用下面两行东西来指定这块东西的解读方式：

```c++
//设置对数据的解读方式
//首先，每个顶点属性有一个属性值（标号）（即vertexShader中的layout）
//这里的意思是：对于属性值为0的属性，它要读取3个float，不需要标准化，下一个顶点的对应属性在3个步长之后，从0开始读。
//然后设置：将属性值为0的顶点属性启用（默认是禁用的）
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

然后就能一行完成渲染啦！第一个参数为图元装配方式，指定为三角形，也就是说会把我们输入的顶点，每三个组成一个三角形传入片元着色器。从0位置开始，一共读3个顶点

```c++
glDrawArrays(GL_TRIANGLES, 0, 3);
```

因为openGL是一个大状态机，在渲染不同物体时，必须绑定对应的VBO，更改对应的解读方式才行。这样换起来太麻烦了，于是有了VAO：它能把这些都保存起来，然后直接绑定VAO就能自动绑定上VBO和切换glVertexAttribPointer。为什么它叫**顶点数组对象**呢？我理解为，它获取到一块数据和对应的解读方式，就直接将顶点分类包装成数组了。它具体存储的是什么，目前理解比较浅。

```c++
unsigned int VAO;
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);
...
glBindVertexArray(0);
```

VAO是工作在bind和unbind之间的，也就是说，在glVertexAttribPointer之前，先将VAO绑定，然后VAO将记录你执行的glVertexAttribPointer，保存这一解读方式（同时也保存了当前绑定的VBO），记录完成后解绑VAO即可。渲染时如果要调用这组数据，直接绑定这个VAO就可以了，将自动设置glVertexAttrib Pointer和VBO，非常方便。

![](https://learnopengl-cn.github.io/img/01/04/vertex_array_objects.png)



##### 更多顶点属性

如果顶点包含两个属性：位置和颜色，在顶点着色器中分别将属性值设置为0和1

属性值只是一种标号，和内存地址无关

```c++
#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
```

然后是VBO中的数据，里面有三个顶点：

```c++
float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};
```

对于这样的情况，解读方式应该是：

```c++
//位置属性
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
//颜色属性
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);
```

注意：属性长度仍然是3，两个属性都只需要读3float的数据，步长是6，因为一个位置属性和下一个位置属性间隔6float，颜色属性的偏移为3，即起始点从3开始，6格6格跳



##### EBO 索引缓冲

实际绘制物体时，可以用一个个小三角拼成，比如要拼一个正方形，顶点数组可以这样：

```c++
float dataSource2[] = {
        0.9f, 0.9f, 0.0f,
        0.9f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,

        0.5f, 0.5f, 0.0f,
        0.5f, 0.9f, 0.0f,
        0.9f, 0.9f, 0.0f
};
```

图元装配时指定装配6个顶点，可以轻松得到一个正方形，它是两个三角形拼起来的

```c++
glDrawArrays(GL_TRIANGLES, 0, 6);
```

但是这样显然有大量冗余，所谓索引缓冲对象，就是指定了遍历顶点的方式，默认是从0..n这样遍历的，

我们可以指定遍历顺序。

```c++
unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 2, // 第一个三角形
        2, 3, 0  // 第二个三角形
};
unsigned int EBO;
glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

这里用和VBO相似的方式，配置了一个EBO，同样地，这个信息可以被VAO存储，只需要写在VAO的bind和unbind之间即可。

```c++
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

把绘图函数改成这个，即可按照当前绑定在GL_ELEMENT_ARRAY_BUFFER上的索引表来遍历。



#### 着色器

主要编写的是顶点着色器和片段着色器

顶点着色器必须给出一个`gl_Position`，它表示这个顶点所在的位置（在**标准化设备坐标**下的，实际参与渲染的位置，不是什么局部坐标世界坐标之类的）

顶点着色器的其他各种输出信息在光栅化阶段会被各种“插值”，变成大量片元输出到片元着色器。比如经典的：用三个顶点坐标显示一个宏观的三角形，实际上片元着色器接受到的片段有成百上千个，但我们仅输入了三个顶点，每一个片段的具体位置均是根据插值得到的。

同时，各种其他顶点输出的信息也都会被插值，比如颜色等等。

片段着色器负责计算一个像素的最终颜色，它可能需要从顶点着色器中接收一些

##### uniform

着色器中可以使用uniform定义一个唯一的变量，在外部通过一些操作更改（实现外部直接把值传入shader任意一个阶段）

```glsl
uniform float offX = 0, offY = 0;
```

外部，传入名称和值即可，glUniform后+数字+类型，如3f表示更改的是一个3维float向量类型

```c++
glUniform1i(glGetUniformLocation(shaderProgram, name), value);
```

外部获取这个值？没有找到资料，可能从外部获取它的值本就不合逻辑

uniform不能在shader内部赋值，但可以初始化（设置缺省值）

##### 特殊的uniform

```c++
uniform sampler2D texture1;
```

用于获取纹理，它的值可以用int方式更改，如果它为2，那么它就表示GL_TEXTURE2这个纹理单元上的纹理图片，在shader中使用 `texture(texture1, Texpos) `获得这张纹理具体位置上的颜色。

`sampler2D` 是一种“不透明类型”，它不能被实例化，只能定义为uniform。

包含有不透明类型的结构体、类同样为不透明类型。



**注意：**始终注意openGL整个都是状态机，在着色器use之后对其设置才有效！



#### 完整管线

顶点进入顶点着色器，经历局部坐标 - 世界坐标 - 观察坐标，变换到观察空间

随后通过一个投影矩阵，这个投影矩阵定义了空间中一块能看见的区域（长方体、平头截体），投影矩阵将这片区域映射到 $[-1,1]^3$ 这个小立方体中，我们最终只绘制这个小立方体内的物体。

这个小立方体就是裁剪空间，也叫**标准设备坐标系，NDC**。后续操作在这个坐标系中进行。

我们在上面的变换中一直认为 $(a,b,c,1)$ 和 $(aw,bw,cw,w)$ 是同一个坐标，转换到NDC之后该归一化一下了，这一步也叫透视除法，就是让每个顶点的 $x,y,z$ 都除以它的 $w$ 分量，$w$ 分量本身保持不变，是为了保留其深度信息以做矫正。

然后进行一个简单的视口变换，在 $xy$ 方向拉伸平移一下，不去动 $z$。

在此之前我们一直对顶点操作，接下来进行图元装配，经过几何着色器我们得到了三角形（或者其他形状）

对三角形进行光栅化，随后每个像素（片段）进入片段着色器，用插值后的数据来计算这个像素的最终颜色并输出。

随后进行深度测试，丢弃一些片段，通常也可以在着色一个像素前就测试并丢弃。



上述视口变换和图元装配，顺序应该无所谓。



#### 理清变换

理清的关键点：

- 使用四阶矩阵，不仅能表达一种变换，也能表达一种**坐标系**。左上三维为三个基向量，右列三个数为原点位置，右下角为 $w$ 分量
  - 可以视为先进行了左上3x3矩阵所示的线性变换，再进行了一个平移

- 对于每个物体，用一个**Transform**矩阵表达其坐标系，它也相当于物体从和世界坐标系重合（单位矩阵）开始，到当前状态经历的变换。对物体的变换都是对整个坐标系的变换
  - 这个变换看起来不是线性变换（移动了原点），但在四维空间下，它是线性变换。
  - 如果想让物体参照世界坐标系变换，就直接在其矩阵左侧乘即可。
  - 如果想让物体以自身坐标系变换，可以运用相似矩阵（相当于矩阵右侧乘）
- 对于单个顶点，才用向量表示，它在物体坐标系下是一个固定的量。
- 相机可以认为是一个物体，它有一个坐标系，它的朝向是一个向量，相对于相机坐标系也固定的，在openGL中应为 $(0,0,-1)$（朝 $z$ 轴负方向）
  - 因此也可以用一个简单的矩阵来描述相机，不必关心其朝向，因为我们直接记录了相机坐标系，朝向固定为 $(0,0-1)$



#### 光照

当我们把一个光照施加到一个物体上时，输出光照颜色和物体颜色的乘积（各个分量各自相乘）即可，注意这里的颜色量值域为(0, 1)，而非(0, 255)

冯氏光照模型主要分三部分：环境光照，漫反射，镜面反射

**环境光照**：始终使物体接受到某个光照，例如(0.1, 0.1, 0.1)

**漫反射**：对于每个片元，取其法向量和光照方向的点积（二者都标准化到模为1）作为光照强度，即正对光照的亮度高，背对的亮度低。（这个阶段我们一般不考虑阻挡、距离等因素）

**镜面反射**：对每个片元，取其光照反射方向（通过法向量计算）和玩家视觉方向的点积，作为光照强度，意义是玩家在某个片元反射方向附近时，能看到它上面的高光。

点积的两个向量都为标准化向量，故结果不大于1，为了更加凸显高光可以对这个值取幂，一般取32次幂为宜，越大则说明高光越集中，反射能力越强。

注意镜面反射仅与观察者位置有关，与观察者视角无关



#### 法线

直观上来讲，一个三角形的法线可以直接由三条边计算出来，但openGL并不会直接帮我们算这个，是有原因的。

法线信息一般放在顶点上，若模型没给定，可以由面法线均值计算得到。

片元法线如果由插值得到，可以获得平滑的光照效果，而如果三个顶点法线都一样，则可以有锐利边缘。

##### 法线变换

物体变换时法线也应该跟着变，法线和一般的顶点位置向量不同，在不等比缩放时，它应当保持**垂直**关系而非进行相同变换；它也不应受位移影响。

简单数理推导可得出，法线变换矩阵应当是`变换矩阵左上3x3矩阵的逆矩阵的转置`

逆矩阵运算对于GPU来说非常慢，故最好在CPU中计算好直接传入。



#### 材质和光照贴图

材质即物体的贴图+上述光照模型中的参数，通过更改三种光的强度以及反光度可以模拟不同物体的反射效果。

并不是每一个参数都值得更改，一般的材质：

```C++
struct Material{
    sampler2D diffuse;
    vec3 specular;
    float spininess;
};
```

- 环境光颜色与漫反射颜色 = 物体自身颜色，通常没必要更改，强度差别取决于光照设定
- 镜面光颜色 = 白 * 一定系数，这个系数决定了镜面光的亮度。
  - 也可以用物体自身颜色，这会使高光更接近物体自身颜色，使用白色则高光更接近光源颜色。
- 反光度/镜面反射率决定了表面反光能力，越大则反光越强，高光越集中，越适用于金属玻璃等材质。

材质参考：http://devernay.free.fr/cours/opengl/materials.html

光照材质：

```C++
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
```

环境光、漫反射、镜面反射的配比取决于我们对光照的设定。通常镜面颜色设置为光源本身颜色，漫反射和环境光设置为光源颜色的0.6, 0.2倍左右。



##### 光照贴图

可以给物体附上多张贴图，在漫反射部分使用漫反射贴图，镜面反射部分使用高光反射贴图，实现同一个物体表面不同部分的不同效果。



#### 网格与模型

网格这个名词一开始让我有点搞不明白，后来我意识到一件事情：3d模型是十分复杂的，除了基本的顶点数据之外，还可能包含多张贴图、动画、多种材质乃至场景信息，不同部分可能应用不同贴图。

“网格”就是一组三角面片，包含一个顶点数组和相应贴图，即先前我们理解中最简单的模型，一个实际的模型包含许多网格。

模型除了用网格表示外，还有点云、体素等多种方式。本项目只考虑网格。

模型文件格式众多，可以使用工具库assimp读取，需要了解的是大多数模型存储的逻辑：使用一个模型文件记录网格信息，在旁边放贴图等资源文件，模型文件中包含了**贴图相对路径**等信息。故通常不能将下载到的模型和资源文件拆开。





#### 缓冲与测试

openGL提供了若干种缓冲（临时数组），我们通常在每一帧绘制前使用 `glClear` 清空这些缓冲。

深度测试和模板测试可以根据缓冲内容选择性地抛弃一些片元，可以用 `glEnable` 启用测试，也有各自的函数来配置筛选方法、写入掩码等等。

深度缓冲和模板缓冲都以屏幕位置为索引，它们在每一次绘制前被清空，绘制内容时被更新，从而影响接下来的绘制。简而言之它们是面向整个场景而非单个物体的。



##### 深度测试

记录屏幕坐标下每个位置上的片元深度（$z$ 坐标），一般情况下，仅当新片元深度更小时才会绘制并更新。可以用 `glglDepthFunc` 配置其他的筛选方式。

关于深度：强大的裁剪矩阵已经帮我们计算好了每个片元的 $x,y$ 位置以及深度，深度测试其实就是记录每个位置的最大深度以及颜色。它使用的缓冲我们需要手动清空 `glClear(GL_DEPTH_BUFFER_BIT);`，记得 `glClear` 是用bitmask的，可以把 `GL_DEPTH_BUFFER_BIT` 和其他东西或起来。

裁剪矩阵对 $z$ 轴的映射也不是线性的，越远的地方粒度越大，精度越低。

具体而言，映射后的深度值（也叫深度精度值？）是与 $1/z$ 正相关的，如近平面距离为2，远平面距离为10，则深度值0.5实际对应了 $\frac{1}{1/2+1/10}$ 即距离2.5的地方。

片段着色器中，`gl_FragCoord`这个内建变量存储了该片段的深度信息，$x,y,z$ 分别表示片段的位置和深度。

##### 深度冲突

如果两个平面恰好重叠在一起，就很难说明谁上谁下了，常常会导致闪烁，实际渲染中应当尽量避免此类情况。



##### 模板测试





#### 面剔除

一种非常聪明的技巧。

试想：在我们观察一个**封闭几何体**的时候，总有几个面是永远看不到的（是指背面，而非里面），对于这几个面，虽然用深度缓冲能覆盖他们，但还不够优秀，我们有办法把它更早一步剔除掉。

如果我们在定义一个面的顶点顺序时，始终假设我们面向它（从封闭几何体的外部），然后逆时针写下这三个顶点。

那么在三角形变换到裁剪空间下后，如果它还是面向我们，三个顶点就一定是逆时针的，否则就一定是顺时针，可以直接将其剔除。

判断顺逆时针是个运算量很小的活，所以面剔除能非常有效地提升性能。

面剔除的结果：在封闭几何体的内部观察时，它透明了。如果我们对一个单独的三角形进行面剔除，会发现只能从一个方向上看到它，这些特性有时也会被利用到。

它可能很像是剔除了一个面的*里面*，但我们都知道图形学本身没有什么内外双面的概念，只是剔除了背面而已。



#### 透明与混合

将半透明的物体绘制到原有颜色缓冲上，需要进行混合。

通常将颜色缓冲中原有的内容叫目标，即将绘制的半透明颜色叫源，将源绘制到目标上。

问题：深度测试，处于前方的透明物体先绘制之后，后面的内容将不会绘制。

解决：透明物体最后绘制，且按到视点的距离排序，由远到近绘制。

问题：距离不好排序，通常采取近似方法，适用于大部分场合。







