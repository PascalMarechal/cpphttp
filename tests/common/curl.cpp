#include "curl.h"
#include <curl/curl.h>
#include <curl/easy.h>

bool checkUrl(const char *url)
{
    CURL *curl;
    CURLcode response;

    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* don't write output to stdout */
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

        /* Perform the request */
        response = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return response == CURLE_OK;
}
