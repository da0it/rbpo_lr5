#include <stddef.h>

/*@ predicate cstrn_valid{L}(const char* s, integer n) =
      \valid_read(s + (0..n-1)) && \exists integer k; 0 <= k < n && s[k] == '\0';
*/

/*@ predicate valid_req_struct(HttpRequest* req) =
      \valid(req) &&
      \valid(req->method + (0..15)) &&
      \valid(req->path + (0..1023)) &&
      \valid(req->version + (0..15)) &&
      \valid(req->headers + (0..49)) &&
      \forall integer i; 0 <= i < 50 ==> \valid(req->headers[i] + (0..255));
*/

/*@
  requires valid_req_struct(req);
  requires 0 <= req->header_count <= 50;
  // Требование: все headers[i] терминированы
  requires \forall integer i; 0 <= i < req->header_count ==> cstrn_valid(req->headers[i],256);

  assigns \nothing;
*/
void process_http_headers(HttpRequest* req);
