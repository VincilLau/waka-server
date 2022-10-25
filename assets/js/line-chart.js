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

class LineChart {
  constructor(id) {
    this.id = id;
    this.data = [];
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

    let formatter = "";
    if (this.data.length <= 7) {
      formatter = (item) => {
        if (item.data == 0) {
          return "";
        }
        return timeTexts[item.dataIndex];
      };
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
            formatter: formatter,
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
