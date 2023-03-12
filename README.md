建设目标：

1. 经纬度格式化工具
2. 经纬度行编辑控件
3. 自定义插件

NOTE

- 谨慎对外提供格式化后的字符串转回 double 的接口。如果提供上述接口会造成精度丢失，又无法避免用户滥用。

# 为什么造轮子

没有可以用的轮子。

项目中一共存在三个类似的封装：每个都有各自的问题

- `QLineEditExt` `CLineEditLon` `CLineEditLat` in `Utils/LineEditRegex.h`

    > 为了在表格中使用，注入 row/column 变量和接口；为了和图层交互，重写 `focusInEvent` 接口（应该使用表格的选中事件）；经纬度组件提供了获取弧度的接口（没有意义）；

- `CLineEditLatExt` `CLineEditLonExt` in `Utils/LineEditRegexExt.h`

    > 修改按键的默认行为，多余；修改按下鼠标的默认行为，和表格互动，更多余。这两次重写行为非常像是试验 qt 组件的练习，降低效率，滥竽充数，完全没有价值。

- `SeCommonLineEditLon` `SeCommonLineEditLat` in `commonlineedit.h`

    > 经纬度组件提供了获取弧度的接口（没有意义）；输入、输出不一致，反直觉；过多的变量和状态，以及滥用 `validator` ，不知所云。

看这样的代码真的浪费生命，一无所得，一无是处。全是写代码时的雷点、槽点，竟然能全中。