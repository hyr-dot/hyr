### Rho method
Rho方法使用一种floid环的方法来寻找碰撞。该方法的主要优点是可以节省内存空间。

![image](https://user-images.githubusercontent.com/57925688/181754468-9188a567-6ce5-4a5f-bfb5-de0ac93f267f.png)

与naive birthday attack不同，本次尝试使用随机算法生成消息。生成消息时，函数message_generater()接收消息的最大长度，随后生成随机字符串。

![image](https://user-images.githubusercontent.com/57925688/181755917-0ca6b1d9-d398-4657-9296-2dd344fb4812.png)

随后，生成两个哈希值序列，直到寻找到相等的像。生成序列的函数采用h_i = f(h_(i-1))，如下图所示：

![image](https://user-images.githubusercontent.com/57925688/181756735-f73524d7-d515-44b1-a8ee-59ceca700a39.png)

注：此方法参考了一个Rho方法的实现，代码地址https://github.com/DevNathan/RhoPollard.git

依旧是首先使用前32位哈希值，寻找碰撞。如下图所示：

![image](https://user-images.githubusercontent.com/57925688/181756844-b8bca61e-f016-4340-9c1c-5db149f4ec3b.png)

结果如下，时间约为2分钟。

![image](https://user-images.githubusercontent.com/57925688/181754917-1aba3bc1-f082-453a-b245-40b2bb7f576c.png)

### 48bit 
测试寻找48bit哈希值的用时，结果如下图所示：

![image](https://user-images.githubusercontent.com/57925688/181925995-137a7bb7-533a-42e7-930a-0e99735202b2.png)

时间约为6.7h，但最终也能得到结果。
