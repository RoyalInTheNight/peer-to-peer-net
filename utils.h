#include <inttypes.h>
#include <stdint.h>
#include <algorithm>

class p2p_utils {
public:
    int8_t getlasterror();
    void warnings();
    int8_t send_to();
    int8_t recv_from();
};