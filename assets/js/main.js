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
    url: "/api/summaries",
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

function drawLineChart(summaries) {
  $("#title-day").text(summaries.days.length);
  $("#title-time").text(summaries.total.time_text);

  let date = new Date(Date.parse(summaries.total.start));
  let lineChart = new LineChart("line-chart");
  for (let i = 0; i < summaries.days.length; i++) {
    lineChart.data.push({
      date: dateText(date),
      hours: summaries.days[i].total_msec / 1000 / 3600,
      timeText: summaries.days[i].time_text,
    });
    date = nextDay(date);
  }
  lineChart.draw();
}

function drawCharts(summaries) {
  drawLineChart(summaries);
}

function main() {
  const { start, end } = getStartAndEnd();
  getSummeries(start, end, (resp) => {
    $("#charts").append(`<div id="line-chart" class="card"></div>`);
    drawCharts(resp);
    $(window).resize(() => {
      $("#charts").empty();
      $("#charts").append(`<div id="line-chart" class="card"></div>`);
      drawCharts(resp);
    });
  });
}

$(document).ready(() => {
  main();
});
