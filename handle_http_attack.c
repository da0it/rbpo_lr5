/*@
  requires valid_req_struct(req);
  requires cstrn_valid(req->path, 1024);
  requires req->body == \null || \valid_read(req->body + (0..req->body_len-1));
  assigns \nothing;
*/
void handle_http_attack_vectors(HttpRequest* req);
