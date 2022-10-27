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

class LineChartItem {
  constructor(date, hours, timeText) {
    this.date = date; // YYYY-MM-DD格式的日期
    this.hours = hours; // 编码小时数，浮点数
    this.timeText = timeText; // 时间的可视化字符串，默认为'%H小时%M分'
  }
}

// 显示一周内编码时间的折线图
class LineChart {
  constructor(id) {
    this.id = id; // 折线图DOM的ID属性
    this.data = []; // 折线图的数据，LineChartItem类型的数组
  }

  draw() {
    const dom = document.getElementById(this.id);
    let chart = echarts.init(dom);

    let xAxisData = [];
    let seriesData = [];
    let timeTexts = [];

    for (let i = 0; i < this.data.length; i++) {
      xAxisData.push(this.data[i].date);
      seriesData.push(this.data[i].hours);
      timeTexts.push(this.data[i].timeText);
    }

    const option = {
      xAxis: {
        type: "category",
        data: xAxisData,
      },
      yAxis: {
        type: "value",
      },
      series: [
        {
          data: seriesData,
          type: "line",
          smooth: true,
          label: {
            show: true,
            position: "bottom",
            formatter: (item) => {
              if (item.data == 0) {
                return "";
              }
              return timeTexts[item.dataIndex];
            },
            textStyle: {
              fontSize: 12,
            },
          },
        },
      ],
    };

    chart.setOption(option);
  }
}
