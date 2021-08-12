#include "omega-common/net.h"


#include <comdef.h>
#include <MsXml6.h>

#include <Windows.h>
#include <winhttp.h>
//#include <WinInet.h>


#include <icu.h>

#pragma comment(lib,"winhttp.lib")




namespace OmegaCommon {

    class WinHTTPHttpClientContext : public HttpClientContext {
        HINTERNET hinternet;
    public:
        WinHTTPHttpClientContext(){
            hinternet = WinHttpOpen(L"OmegaCommon",WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,WINHTTP_FLAG_ASYNC);
        }
        std::future<HttpResponse> makeRequest(HttpRequestDescriptor descriptor){
            size_t urlLen = descriptor.url.size();
            UChar url_w[urlLen];
            u_charsToUChars(descriptor.url.data(),url_w,(int32_t)urlLen);
            URL_COMPONENTS url_components;
            WinHttpCrackUrl(reinterpret_cast<LPCWSTR>(url_w), urlLen, ICU_DECODE, &url_components);

            HINTERNET connection = WinHttpConnect(hinternet,url_components.lpszHostName,INTERNET_DEFAULT_HTTPS_PORT,NULL);

            HINTERNET request = WinHttpOpenRequest(connection,L"GET",url_components.lpszUrlPath,NULL,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,NULL);
            WinHttpSendRequest(request,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0);
            WinHttpReceiveResponse(request,NULL);
            WinHttpCloseHandle(connection);

            std::future<HttpResponse> response;
            return response;
        }
        ~WinHTTPHttpClientContext(){
            WinHttpCloseHandle(hinternet);
        }


    };



    std::shared_ptr<HttpClientContext> HttpClientContext::Create() {
        return std::make_shared<WinHTTPHttpClientContext>();
    }
}