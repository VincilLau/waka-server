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

#ifndef WAKA_SRC_COMMON_HTTP_HPP_
#define WAKA_SRC_COMMON_HTTP_HPP_

namespace waka::common {

enum HttpStatus {
  kContinue = 100,
  kSwitchingProtocols = 101,
  kProcessing = 102,
  kEarlyHints = 103,
  kOK = 200,
  kCreated = 201,
  kAccepted = 202,
  kNonAuthoritativeInformation = 203,
  kNoContent = 204,
  kResetContent = 205,
  kPartialContent = 206,
  kMultiStatus = 207,
  kAlreadyReported = 208,
  kIMUsed = 226,
  kMultipleChoice = 300,
  kMovedPermanently = 301,
  kFound = 302,
  kSeeOther = 303,
  kNotModified = 304,
  kUseProxy = 305,
  kUnused = 306,
  kTemporaryRedirect = 307,
  kPermanentRedirect = 308,
  kBadRequest = 400,
  kUnauthorized = 401,
  kPaymentRequired = 402,
  kForbidden = 403,
  kNotFound = 404,
  kMethodNotAllowed = 405,
  kNotAcceptable = 406,
  kProxyAuthenticationRequired = 407,
  kRequestTimeout = 408,
  kConflict = 409,
  kGone = 410,
  kLengthRequired = 411,
  kPreconditionFailed = 412,
  kPayloadTooLarge = 413,
  kURITooLong = 414,
  kUnsupportedMediaType = 415,
  kRangeNotSatisfiable = 416,
  kExpectationFailed = 417,
  kIMATeapot = 418,
  kMisdirectedRequest = 421,
  kUnprocessableEntity = 422,
  kLocked = 423,
  kFailedDependency = 424,
  kTooEarly = 425,
  kUpgradeRequired = 426,
  kPreconditionRequired = 428,
  kTooManyRequests = 429,
  kRequestHeaderFieldsTooLarge = 431,
  kUnavailableForLegalReasons = 451,
  kInternalServerError = 500,
  kNotImplemented = 501,
  kBadGateway = 502,
  kServiceUnavailable = 503,
  kGatewayTimeout = 504,
  kHTTPVersionNotSupported = 505,
  kVariantAlsoNegotiates = 506,
  kInsufficientStorage = 507,
  kLoopDetected = 508,
  kNotExtended = 510,
  kNetworkAuthenticationRequired = 511,
};

}  // namespace waka::common

#endif  // WAKA_SRC_COMMON_HTTP_HPP_
