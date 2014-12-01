# C++ Style

无需必要勿增实体

####头文件
- [通用](#header.common)

####注释
- [通用](#comment.common)
- [文件](#comment.file)
- [函数](#comment.func)

####命名
- [通用](#name.common)
- [目录](#name.dir)
- [文件](#name.file)
- [宏](#name.marco)
- [名字空间](#name.namespace)
- [类型](#name.type)
- [变量](#name.var)
- [常量](#name.const)
- [函数](#name.func)



####头文件
----
##### <a name="header.common"></a>通用
- 头文件中必须添加防止被多重包含的语句，格式如下(头文件filename.h)

  ```
  #ifndef __FILENAME_H__ // 开头和结尾均为两个连续下划线
  #define __FILENAME_H__ // 文件名中的点和下划线，在宏名中表示为下划线
   
   ...
   
  #endif
  ```

####注释
----
##### <a name="comment.common"></a>通用
- 注释文字一律使用自然语言，一句话完成之后要加英文句号

##### <a name="comment.file"></a>文件
- 每个文件都要包含版权声明，许可证版本(Apache2.0,BSD,GPL,BSD)，原作者信息，文件内容描述
- 对于已经纳入版本库管理的项目，没必要再在文件注释中列出创建，修改等关键时间
- 严禁在头文件和实现文件之间进行简单的拷贝操作

  ```  
  // Copyright 2014 Google Inc. 
  // License: GPL
  //
  // Author: xus
  // E-Mail: xushvai@gmail.com
  // 
  // Overview:
  // Description for the file content.
  ```
##### <a name="comment.func"></a>函数
- 注释中要对函数的参数，返回值进行说明

  ```
  // 计算给出的两个整数的和 
  //
  // Parameters
  //   a - 被加数
  //   b - 加数
  // Return
  //   返回计算结果
  int Sum(int a, int b);
  ```




####命名
----

##### <a name="name.common"></a>通用
- 使用正规英文单词，尽量给出详细描述，力求见名知意，不用在意名字长短
- 除函数名需要使用动词之外，其余实体均使用名词命名
- 除非名字特别冗长且缩写后仍能表述清晰，否则，不要使用缩写
- 使用缩写时，必须使用正规的缩写，不要随意忽略掉单词中的某些字母作为该单词的缩写

##### <a name="name.dir"></a>目录
- 一律使用小写，单词之间用短线"-"连接

##### <a name="name.file"></a>文件

##### <a name="name.marco"></a>宏
##### <a name="name.namespace"></a>名字空间
##### <a name="name.type"></a>类型
##### <a name="name.var"></a>变量
- 一律用小写，单词间用下划线"_"连接，比较短的变量名也可以不分割单词
- 全局变量加"g_"前缀
- 类的成员变量加下划线"_"后缀，结构体的成员变量不用加









##### <a name="name.const"></a>常量
##### <a name="name.func"></a>函数

