/*@
  requires valid_req_struct(req);
  requires 0 <= req->header_count <= 50;
  // Требование: все headers[i] терминированы
  requires \forall integer i; 0 <= i < req->header_count ==> cstrn_valid(req->headers[i],256);

  assigns \nothing;
*/
void process_http_headers(HttpRequest* req);
