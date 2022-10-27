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

// 获取从start到end的汇总数据
// 在成功获取到数据时调用callback
function getSummary(start, end, callback) {
  $.ajax({
    url: "/api/summary",
    data: {
      start: dateText(start),
      end: dateText(end),
    },
    success: function (resp) {
      callback(resp);
    },
    error: function (resp) {
      console.error(resp);
    },
  });
}

// 绘制折线图
function drawLineChart(summary) {
  const id = "line-chart";
  $("#charts").append(`<div id="${id}"></div>`);

  let date = new Date(Date.parse(summary.total.start));
  let chart = new LineChart(id);

  for (let i = 0; i < summary.days.length; i++) {
    const hours = summary.days[i].total_msec / 1000 / 3600;
    const item = new LineChartItem(
      dateText(date),
      hours,
      summary.days[i].time_text
    );

    chart.data.push(item);
    date = nextDay(date);
  }

  chart.draw();
}

// 绘制编程语言饼图
function drawLanguagesChart(summary) {
  const id = "languages-chart";
  $("#left-charts").append(`<div id="${id}" class="pie-chart"></div>`);
  let chart = new PieChart(id, "编程语言");

  for (let i = 0; i < summary.languages.length; i++) {
    const item = new PieChartItem(
      summary.languages[i].name,
      summary.languages[i].total_msec / 1000 / 3600,
      summary.languages[i].time_text
    );
    chart.data.push(item);
  }

  chart.draw();
}

// 绘制编辑器饼图
function drawEditorsChart(summary) {
  const id = "editors-chart";
  $("#right-charts").append(`<div id="${id}" class="pie-chart"></div>`);
  let chart = new PieChart(id, "编辑器");

  for (let i = 0; i < summary.editors.length; i++) {
    const item = new PieChartItem(
      summary.editors[i].name,
      summary.editors[i].total_msec / 1000 / 3600,
      summary.editors[i].time_text
    );
    chart.data.push(item);
  }

  chart.draw();
}

// 绘制操作系统饼图
function drawOssChart(summary) {
  const id = "oss-chart";
  $("#left-charts").append(`<div id="${id}" class="pie-chart"></div>`);
  let chart = new PieChart(id, "操作系统");

  for (let i = 0; i < summary.oss.length; i++) {
    const item = new PieChartItem(
      summary.oss[i].name,
      summary.oss[i].total_msec / 1000 / 3600,
      summary.oss[i].time_text
    );
    chart.data.push(item);
  }

  chart.draw();
}

// 绘制项目饼图
function drawProjectsChart(summary) {
  const id = "projects-chart";
  $("#right-charts").append(`<div id="${id}" class="pie-chart"></div>`);
  let chart = new PieChart(id, "项目");

  for (let i = 0; i < summary.projects.length; i++) {
    const item = new PieChartItem(
      summary.projects[i].name,
      summary.projects[i].total_msec / 1000 / 3600,
      summary.projects[i].time_text
    );
    chart.data.push(item);
  }

  chart.draw();
}

// 绘制标题
function drawTitle(summary) {
  $("#title-day").text(summary.days.length);
  $("#title-time").text(summary.total.time_text);
}

// 绘制所有图表
function drawCharts(summary) {
  drawLineChart(summary);
  $("#charts").append(`<div id="left-charts"></div>`);
  $("#charts").append(`<div id="right-charts"></div>`);
  drawLanguagesChart(summary);
  drawEditorsChart(summary);
  drawOssChart(summary);
  drawProjectsChart(summary);
}

function main() {
  const start = aWeekAgoFromToday();
  const end = today();

  getSummary(start, end, (summary) => {
    drawTitle(summary);
    drawCharts(summary);
    $(window).resize(() => {
      $("#charts").empty();
      drawCharts(summary);
    });
  });
}

$(document).ready(() => {
  main();
});
