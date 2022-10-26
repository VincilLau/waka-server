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

function getStartAndEnd() {
  const defaultStart = aWeekAgoFromToday();
  const defaultEnd = today();

  if (window.location.search.length == 0) {
    return {
      start: defaultStart,
      end: defaultEnd,
    };
  }

  const params = new URLSearchParams(window.location.search);
  const startParam = params.get("start");
  const endParam = params.get("end");

  if (startParam == null || endParam == null) {
    return {
      start: defaultStart,
      end: defaultEnd,
    };
  }

  const re = /\d{4}-\d{2}-\d{2}/;
  if (!re.test(startParam) || !re.test(endParam)) {
    return {
      start: defaultStart,
      end: defaultEnd,
    };
  }

  const start = Date.parse(startParam);
  const end = Date.parse(endParam);
  if (isNaN(start) || isNaN(end)) {
    return {
      start: defaultStart,
      end: defaultEnd,
    };
  }

  return {
    start: new Date(start),
    end: new Date(end),
  };
}

function getSummeries(start, end, callback) {
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

function drawLineChart(summary) {
  let date = new Date(Date.parse(summary.total.start));
  let lineChart = new LineChart("line-chart");
  for (let i = 0; i < summary.days.length; i++) {
    lineChart.data.push({
      date: dateText(date),
      hours: summary.days[i].total_msec / 1000 / 3600,
      timeText: summary.days[i].time_text,
    });
    date = nextDay(date);
  }
  lineChart.draw();
}

function drawLanguagesChart(summary) {
  let pieChart = new PieChart("languages-chart");
  pieChart.title = "编程语言";
  for (let i = 0; i < summary.languages.length; i++) {
    pieChart.data.push({
      name: summary.languages[i].name,
      value: summary.languages[i].total_msec,
      time: summary.languages[i].time_text,
    });
  }
  pieChart.draw();
}

function drawEditorsChart(summary) {
  let pieChart = new PieChart("editors-chart");
  pieChart.title = "编辑器";
  for (let i = 0; i < summary.editors.length; i++) {
    pieChart.data.push({
      name: summary.editors[i].name,
      value: summary.editors[i].total_msec,
      time: summary.editors[i].time_text,
    });
  }
  pieChart.draw();
}

function drawOssChart(summary) {
  let pieChart = new PieChart("oss-chart");
  pieChart.title = "操作系统";
  for (let i = 0; i < summary.oss.length; i++) {
    pieChart.data.push({
      name: summary.oss[i].name,
      value: summary.oss[i].total_msec,
      time: summary.oss[i].time_text,
    });
  }
  pieChart.draw();
}

function drawProjectsChart(summary) {
  let pieChart = new PieChart("projects-chart");
  pieChart.title = "项目";
  for (let i = 0; i < summary.projects.length; i++) {
    pieChart.data.push({
      name: summary.projects[i].name,
      value: summary.projects[i].total_msec,
      time: summary.projects[i].time_text,
    });
  }
  pieChart.draw();
}

function drawCharts(summary) {
  $("#title-day").text(summary.days.length);
  $("#title-time").text(summary.total.time_text);
  drawLineChart(summary);
  drawLanguagesChart(summary);
  drawEditorsChart(summary);
  drawOssChart(summary);
  drawProjectsChart(summary);
}

function main() {
  const { start, end } = getStartAndEnd();
  getSummeries(start, end, (resp) => {
    $("#charts").append(`<div id="line-chart" class="card"></div>`);
    $("#charts").append(
      `<div id="languages-chart" class="card left-pie-chart"></div>`
    );
    $("#charts").append(
      `<div id="editors-chart" class="card right-pie-chart"></div>`
    );
    $("#charts").append(
      `<div id="oss-chart" class="card left-pie-chart"></div>`
    );
    $("#charts").append(
      `<div id="projects-chart" class="card right-pie-chart"></div>`
    );
    drawCharts(resp);
    $(window).resize(() => {
      $("#charts").empty();
      $("#charts").append(`<div id="line-chart" class="card"></div>`);
      $("#charts").append(
        `<div id="languages-chart" class="card left-pie-chart"></div>`
      );
      $("#charts").append(
        `<div id="editors-chart" class="card right-pie-chart"></div>`
      );
      $("#charts").append(
        `<div id="oss-chart" class="card left-pie-chart"></div>`
      );
      $("#charts").append(
        `<div id="projects-chart" class="card right-pie-chart"></div>`
      );
      drawCharts(resp);
    });
  });
}

$(document).ready(() => {
  main();
});
