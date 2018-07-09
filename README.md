# Z0BPcTools
一个windows反汇编工具，界面风格防OllyDbg
利用业余开发了一款类似仿OLlyDbg界面的 IDA静态反编译工具，目前是1.0版本，功能不是很强大但是基本功能有了

1.  显示一个PE文件的汇编代码，多种颜色高亮显示，界面风格与OD界面相同，同时也显示二进制十六进制的数据窗口，还有PE头信息的窗口。

2.  目前只支持windowsPE，支持X86 / X64

3.  目前是全内存方式，暂时没有类似IDA的idb文件方式，所以很大的PE 比如50M以上的PE会占比较大的内存

4.  暂时没实现OllyDbg的快捷键

5.  暂时还没实现动态调试。

6.  下面是软件截图

32位的PE文件
![image]( https://github.com/basketwill/Z0BPcTools/blob/master/3bb384bc9b2d4dcead1fb6cfcf010586.png)

64位的pe文件

![image]( https://github.com/basketwill/Z0BPcTools/blob/master/87d6e42df5571d614eda632e9f974b70.png)
