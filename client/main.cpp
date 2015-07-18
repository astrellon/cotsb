#include <cotsb/client.h>

int main(int argc , char *argv[])
{
    cotsb::Client client(8888);
    client.start_client();
    return 0;
}
