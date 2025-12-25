#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct {
    char method[16];
    char path[1024];
    char version[16];
    char headers[50][256];
    int header_count;
    char* body;
    size_t body_len;
} HttpRequest;

int parse_http_request(const char* request, HttpRequest* req) {
    // Множественные уязвимости парсинга HTTP
    if (!request || !req) return -1;
    
    char buffer[4096];
    // Переполнение буфера
    strcpy(buffer, request);
    
    // Уязвимый парсинг первой строки
    char* line = strtok(buffer, "\r\n");
    if (!line) return -1;
    
    // Переполнение буферов method/path/version
    sscanf(line, "%15s %1023s %15s", req->method, req->path, req->version);
    
    // Уязвимости обработки пути (Path Traversal)
    if (strstr(req->path, "..")) {
        // Недостаточная фильтрация
        char* pos = req->path;
        while ((pos = strstr(pos, "..")) != NULL) {
            memmove(pos, pos + 2, strlen(pos + 2) + 1); // Уязвимое удаление
        }
    }
    
    // Парсинг заголовков с уязвимостями
    req->header_count = 0;
    while ((line = strtok(NULL, "\r\n")) != NULL && *line != '\0') {
        if (req->header_count >= 50) break;
        
        // Переполнение буфера заголовков
        strcpy(req->headers[req->header_count], line);
        req->header_count++;
        
        // Уязвимости Content-Length
        if (strncasecmp(line, "Content-Length:", 15) == 0) {
            int content_len = atoi(line + 15);
            // Целочисленное переполнение
            if (content_len > 0) {
                req->body_len = content_len;
                req->body = malloc(content_len);
                if (!req->body) return -1;
                
                // Уязвимое копирование тела
                char* body_start = strstr(request, "\r\n\r\n");
                if (body_start) {
                    body_start += 4;
                    // Переполнение буфера
                    memcpy(req->body, body_start, content_len);
                }
            }
        }
    }
    
    return 0;
}

void process_http_headers(HttpRequest* req) {
    // Уязвимости обработки HTTP заголовков
    if (!req) return;
    
    for (int i = 0; i < req->header_count; i++) {
        // Уязвимости Host header
        if (strncasecmp(req->headers[i], "Host:", 5) == 0) {
            char host[256];
            // Переполнение буфера
            strcpy(host, req->headers[i] + 5);
            
            // Уязвимость заголовка Host
            if (strstr(host, "\r\n")) {
                // HTTP Response Splitting возможность
            }
        }
        
        // Уязвимости Cookie
        if (strncasecmp(req->headers[i], "Cookie:", 7) == 0) {
            char cookies[1024];
            strcpy(cookies, req->headers[i] + 7);
            
            // Уязвимый парсинг куки
            char* token = strtok(cookies, ";");
            while (token) {
                // XSS возможность через куки
                if (strstr(token, "<script>")) {
                    // Недостаточная фильтрация
                    char* pos = token;
                    while ((pos = strstr(pos, "<script>")) != NULL) {
                        memmove(pos, pos + 8, strlen(pos + 8) + 1);
                    }
                }
                token = strtok(NULL, ";");
            }
        }
    }
}

void handle_http_attack_vectors(HttpRequest* req) {
    // Комбинированные веб-уязвимости
    if (!req) return;
    
    // SQL Injection возможности
    if (strchr(req->path, '?')) {
        char query[2048];
        strcpy(query, req->path);
        
        char* question = strchr(query, '?');
        if (question) {
            char* params = question + 1;
            // Уязвимый парсинг параметров
            char* token = strtok(params, "&");
            while (token) {
                if (strstr(token, "SELECT") || strstr(token, "UNION")) {
                    // Примитивная фильтрация SQL injection
                    char* pos = token;
                    while ((pos = strstr(pos, "SELECT")) != NULL) {
                        memmove(pos, pos + 6, strlen(pos + 6) + 1);
                    }
                }
                token = strtok(NULL, "&");
            }
        }
    }
    
    // Buffer overflow в обработке тела
    if (req->body && req->body_len > 0) {
        char temp[1024];
        // Переполнение буфера
        if (req->body_len < sizeof(temp)) {
            memcpy(temp, req->body, req->body_len);
            temp[req->body_len] = '\0';
        } else {
            memcpy(temp, req->body, sizeof(temp) - 1);
            temp[sizeof(temp) - 1] = '\0';
        }
        
        // XSS в теле запроса
        if (strstr(temp, "<script>") || strstr(temp, "javascript:")) {
            // Недостаточная санитизация
        }
    }
    
    // Утечка памяти в обработке больших тел
    if (req->body_len > 1000000) {
        free(req->body);
        req->body = NULL;
        // Но может быть использован после освобождения
    }
}
