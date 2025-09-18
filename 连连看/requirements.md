# First Level Heading

Paragraph.

## Second Level Heading

Paragraph.

- bullet
+ other bullet
* another bullet
    * child bullet

1. ordered
2. next ordered

### Third Level Heading

Some *italic* and **bold** text and `inline code`.

An empty line starts a new paragraph.

Use two spaces at the end  
to force a line break.

A horizontal ruler follows:

---

Add links inline like [this link to the Qt homepage](https://www.qt.io),
or with a reference like [this other link to the Qt homepage][1].

    Add code blocks with
    four spaces at the front.

> A blockquote
> starts with >
>
> and has the same paragraph rules as normal text.

First Level Heading in Alternate Style
======================================

Paragraph.

Second Level Heading in Alternate Style
---------------------------------------

Paragraph.

[1]: https://www.qt.io




# 连连看
图现在是绘制出来了，但是它完全没有交互。我需要将它实现连连看的游戏。现在我的连连看规则如下：

### 基本功能
1.消子
对玩家选中的两个图案进行判断，是否符合消除的规则。只有符合以下条件的图案对才会消失：

- [x] (1)一条直线连通
- [x] (2)两条直线连通
- [x] (3)三条直线连通
- [x] 如果可以消除，从游戏地图中提示连接路径，然后消除这两种图片，并计算相应的积分。如果不能消除，则保持原来的游戏地图。

2.判断胜负
当游戏完成后，需要判断游戏的胜负。不同模式下，判断胜负的规则不同。
- [x] 基本模式时，如果在5分钟内，将游戏地图中所有的图片都消除，则提示玩家获胜。否则将停止交互功能，提示玩家失败。

3.其他组件的功能

- [x] 提示按钮:（还未确定如果不存在提示是否可以正确提示-- 可）
可以提示界面上能够消除的一对图片。

- [x] 重排按钮:
根据随机数，重新排列游戏地图上图片。
对游戏地图中剩下的图片进行重新排列，重新排列只是将所有的图片的位置随机互换，不会增加图片的种类与个数。重排之前没有图片的位置，重排之后也不会有图片。

- [x] 计时器（我暂时还没在ui里面设计，请告诉我应该怎么设计比较合适）
设定一定时间来辅助游戏是否结束。

- [x] 在基本模式时，使用进度条对游戏进行计时。每局游戏时间为300秒，游戏开始从300秒开始倒计时。

### 待修正功能
- [x] 两条直线越界问题
- [x] 两条直线距离过远无法消除问题
- [x] 三条直线消除无效
- [x] 按钮禁用问题
- [x] 暂停游戏功能----对应的按钮禁止交互问题
### 可优化功能
- [ ] 积分显示
- [ ] 画线功能优化
- [x] 选中特效
- [ ] 选中特效优化
- [ ] 设置按钮
- [ ] 帮助按钮
