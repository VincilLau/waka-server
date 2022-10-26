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

class PieChart {
  constructor(id) {
    this.id = id;
    this.title = "";
    this.data = [];
  }

  draw() {
    this.data.sort((item1, item2) => {
      return item2.value - item1.value;
    });

    const dom = document.getElementById(this.id);
    let chart = echarts.init(dom);

    const option = {
      title: {
        text: this.title,
        left: "center",
      },
      tooltip: {
        trigger: "item",
        formatter: (item) => {
          return item.data.time;
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
          data: this.data,
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
