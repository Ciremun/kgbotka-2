#include <assert.h>
#include <string.h>
#include "./http.h"

static size_t write_to_region(char *data, size_t size, size_t nmemb, Region *region)
{
    void *dest = region_alloc(region, size * nmemb);
    if (dest == NULL) {
        return 0;
    }
    memcpy(dest, data, size * nmemb);
    return nmemb;
}


// TODO: use asynchronous CURL
bool curl_get_request(CURL *curl, Log *log, Region *memory,
                      const char *url, String_View *body)
{
    size_t begin_size = memory->size;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_region);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, memory);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        log_error(log, "COMMAND: CURL GET query of %s has failed failed: %s",
                  url,
                  curl_easy_strerror(res));
        return false;
    }

    assert(begin_size <= memory->size);

    *body = (String_View) {
        .count = memory->size - begin_size,
        .data = memory->buffer + begin_size,
    };

    return true;
}
