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

// 初始化所有<icon>元素
// <icon>的src属性指定了svg图标的url
// svg图标的根元素的id必须是#icon
function initIcon() {
  $("icon")
    .addClass("sidebar-menu-icon")
    .each(function () {
      const src = $(this).attr("src");
      $.ajax({
        url: src,
        el: this,
        success: function (resp) {
          $(this.el).html(resp.getElementById("icon").outerHTML);
        },
        error: function (resp) {
          console.error(resp);
        },
      });
    });
}

$(document).ready(() => {
  initIcon();
});
