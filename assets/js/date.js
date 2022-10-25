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

function today() {
  let d = new Date();
  d.setHours(0);
  d.setMinutes(0);
  d.setSeconds(0);
  d.setMilliseconds(0);
  return d;
}

function dateText(date) {
  const y = date.getFullYear().toString().padStart(4, "0");
  const m = (date.getMonth() + 1).toString().padStart(2, "0");
  const d = date.getDate().toString().padStart(2, "0");
  return `${y}-${m}-${d}`;
}

function aWeekAgoFromToday() {
  const t = today();
  const d = t - 6 * 24 * 3600 * 1000;
  return new Date(d);
}

function aYearAgoFromToday() {
  let d = today();
  d.setFullYear(d.getFullYear() - 1);
  d -= -24 * 3600 * 1000;
  return new Date(d);
}

function nextDay(date) {
  const duration = -24 * 3600 * 1000;
  return new Date(date - duration);
}
