# MyPublicTools
1.使用时请添加Include目录为附加包含目录

2.使用时请添加Lib\vc141\Json为附加库目录

3.项目,右键,属性,C/C++,预处理器,预处理器定义,增加一个: _WIN32_WINNT=0x0502

4.添加附加包含目录后
如果当前是普通C++项目,请:

  (1)在预编译文件stdafx.h中 添加 #include <YcmBase.h>,打开Base空间 using namespace Base;
  
  ![image](https://user-images.githubusercontent.com/65977067/141692494-c192fc0b-48c2-41a5-81e1-db01fcf27585.png)

  (2)在预编译文件stdafx.cpp中 添加 #include <YcmBase.cpp>
  
  ![image](https://user-images.githubusercontent.com/65977067/141692502-5ebef9bd-c50f-4a96-add2-860547d5a18d.png)

如果当前是QTC++项目,请:

  (1)在预编译文件stdafx.h中 **在所有的QT头文件后添加** #include <YcmQTBase.h>,并打开QTBase空间 using namespace QTBase;
  
  ![image](https://user-images.githubusercontent.com/65977067/141692429-0de5be46-6e08-46fb-bd4d-5d463fb9d32a.png)

  (2)在预编译文件stdafx.cpp中 添加 #include <YcmQTBase.cpp>
  
  ![image](https://user-images.githubusercontent.com/65977067/141692439-46300dbd-2d19-459b-97af-03e5110a983e.png)

  
 **注:YcmQTBase库包含了YcmBase中的所有功能,若当前有意将C++工程转换为QTC++工程,只需修改预编译文件中的包含文件即可**
 

