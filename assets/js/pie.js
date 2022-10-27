// Copyright 2022 Vincil Lau
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

class PieChartItem {
  constructor(name, hours, timeText) {
    this.name = name; // 数据项的名称
    this.hours = hours; // 编码小时数，浮点数
    this.timeText = timeText; // 时间的可视化字符串，默认为'%H小时%M分'
  }
}

// 饼图，显示编程语言、编辑器、操作系统和项目信息
class PieChart {
  constructor(id, title) {
    this.id = id; // 饼图DOM的ID属性
    this.title = title; // 标题
    this.data = []; // 饼图的数据项，PieChartItem类型的数组
  }

  draw() {
    const dom = document.getElementById(this.id);
    let chart = echarts.init(dom);

    // 按hours从大到小排序
    this.data.sort((item1, item2) => {
      return item2.hours - item1.hours;
    });

    let data = [];
    for (let i = 0; i < this.data.length; i++) {
      data.push({
        name: this.data[i].name,
        value: this.data[i].hours,
        timeText: this.data[i].timeText,
      });
    }

    const option = {
      title: {
        text: this.title,
        left: "center",
      },
      tooltip: {
        trigger: "item",
        formatter: (item) => {
          return item.data.timeText;
        },
      },
      legend: {
        orient: "vertical",
        left: "left",
      },
      series: [
        {
          name: this.title,
          type: "pie",
          radius: "50%",
          data: data,
          emphasis: {
            itemStyle: {
              shadowBlur: 10,
              shadowOffsetX: 0,
              shadowColor: "rgba(0, 0, 0, 0.5)",
            },
          },
        },
      ],
    };

    chart.setOption(option);
  }
}
