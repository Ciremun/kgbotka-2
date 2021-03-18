#ifndef HTTP_H_
#define HTTP_H_

#include <stdbool.h>

#include <curl/curl.h>

#include "./log.h"
#include "./region.h"

bool curl_get_request(CURL *curl, Log *log, Region *memory, const char *url, String_View *body);

#endif // HTTP_H_
