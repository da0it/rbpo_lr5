#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/legacy/http_parser_vuln.c"

// ESBMC nondet:
extern unsigned int __VERIFIER_nondet_uint(void);
extern unsigned char __VERIFIER_nondet_uchar(void);

int main() {
    HttpRequest req;
    char request[4096];

    // Заполним байтами и гарантируем '\0' внутри
    for (unsigned i = 0; i < sizeof(request); i++)
        request[i] = (char)__VERIFIER_nondet_uchar();
    request[4095] = '\0';

    int r = parse_http_request(request, &req);

    // safety assertions (цели)
    assert(r == 0 || r == -1);
    if (r == 0) {
        assert(req.header_count >= 0 && req.header_count <= 50);
    }

    // cleanup
    if (req.body) free(req.body);
    return 0;
}
