> 借着学习QT的机会，开发了这个转换软件，主要功能是将typecho网站数据库中的文章信息导出为适用于hexo的makedown文件。

注意：这个项目不会再有更新了，可用性未知，我自己都看不懂我自己写的代码了，可以看看issue里面有没有你问的问题，问我我也不知道。

## 相关连接
下载地址：https://github.com/g-mero/dealtypecho/releases
github开源地址：https://github.com/g-mero/dealtypecho

## 如何使用
加下载下来的压缩包解压，运行其中的dealtypecho.exe
首先输入数据库信息：

![](https://cdn.jsdelivr.net/gh/g-mero/gmero@master/img/20200709165204.png)

到文章导出栏，选择附加选项：
 - 修复#是指将typecho文章中不符合hexo规范的#写法修复（再typecho中允许#后不接空格以及# 文字 # 的不符合规范标题写法）
 - 删除回车符是删除\r标识,可选项，目的是修复行与行间距较大的问题，剩下几个选项依赖该选项
 - 标签转换主要是考虑到typecho部分主题使用带标志的标签例如handsome主题能够识别 !> 内容 这种标签，该选项可将其转换为hexo的butterfly主题的标签形式（其他有该功能的主题也一致）
 - 相册转换也是将handsome主题中的插入相册功能转换为butterfly中的相册
 - 同样的对按钮进行转换

![](https://cdn.jsdelivr.net/gh/g-mero/gmero@master/img/20200709165332.png)

选择导出路径，点击开始：
该界面会显示获取到的文章，包括其cid mid 以及文章标题
你可以通过筛选功能选择你要导出的文章

![](https://cdn.jsdelivr.net/gh/g-mero/gmero@master/img/20200709170549.png)

接着就可以进行导出了
