# MyPublicTools
1.使用时请添加Include目录为附加包含目录

2.使用时请添加Lib\vc141\Json为附加库目录

3.添加附加包含目录后,请:

  (1)在预编译文件stdafx.h中 #include <YcmBase.h>,并打开Base空间 using namespace Base;
  
  (2)在预编译文件stdafx.cpp中 #include <YCcmBase.cpp>
