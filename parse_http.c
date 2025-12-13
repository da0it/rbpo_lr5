/*@
  requires cstrn_valid(request, 4096);          // иначе buffer переполнится
  requires valid_req_struct(req);
  assigns req->method[0..15], req->path[0..1023], req->version[0..15],
          req->headers[0..49][0..255], req->header_count,
          req->body, req->body_len;

  ensures \result == 0 ==> 0 <= req->header_count <= 50;
  ensures \result == 0 ==> cstrn_valid(req->method,16);
  ensures \result == 0 ==> cstrn_valid(req->path,1024);
  ensures \result == 0 ==> cstrn_valid(req->version,16);
*/
int parse_http_request(const char* request, HttpRequest* req);
