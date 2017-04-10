Copyright 2012 lianera 
 Author:Lianera
 E-mail:158157488@qq.com
 Blog:www.lianera.com
Licensed under the Apache License, Version 2.0 (the "License");
这是一个用来挖掘贴吧用户ID的程序，能够将某个贴吧的所有用户名都按格式记录下来，可用于@大量用户。
新建任务：
 
开始挖掘：
 
结果保存在对应的文件中：
 
程序用C语言写成的，用到了网络编程。采用socket通信发出get请求，返回含有贴吧ID的页面，得到页面源码后，程序对其进行解析，提取ID信息并格式化保存。

