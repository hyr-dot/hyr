# SM3 Length Extension Attack
本Project包含两个文件。
- main.py：实现长度扩展攻击的主要代码
- mysm3.py：修改版的sm3实现部分，主要目的是得到加密后向量（IV）的值

注. mysm3.py的实现借鉴了sm3官方文档以及网上的一种python实现方法：https://blog.csdn.net/qq_43710705/article/details/103915133

## 变量声明

变量的声明如下图所示，M为初始消息，append为附加消息，这两个消息任意值均可。为了方便检查结果，本例中固定二者取值。

![image](https://user-images.githubusercontent.com/57925688/182009729-91428095-ccda-432a-8779-9da5d0cdeb84.png)

## 长度扩展

函数entend用于生成原消息+填充+附加消息的哈希值。参数含义分别为：
 - M_hash: M的hash值
 - M_len: M的长度
 - append: 附加的消息

最终返回结果是生成的哈希值。

![image](https://user-images.githubusercontent.com/57925688/182009918-8f24b42b-6180-47be-8b7b-94356ea9cd54.png)

其中消息填充部分的函数代码如下图所示：

![image](https://user-images.githubusercontent.com/57925688/182009932-644884cc-3b6c-447e-82e5-c7afc6b9a52b.png)

## 结果检验

1. 随机生成一个消息，计算哈希值。
2. 生成附加消息，首先用第一步得到的哈希值推算出这一次加密结束后8个向量的值，再以它们作为初始向量，去加密m’，得到另一个哈希值。
3. 计原消息+填充+附加消息的哈希值。
4. 验证是否成功：若成功，第2、3步生成的哈希值应该相等。

![image](https://user-images.githubusercontent.com/57925688/182009943-36e4f411-6b71-45e0-843a-8f9efe1d5d27.png)

本例的运行结果如下：

![image](https://user-images.githubusercontent.com/57925688/182009987-7547af00-ad92-46d0-a20f-f62eac8d2ae6.png)
