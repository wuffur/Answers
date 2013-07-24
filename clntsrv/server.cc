
#include <stdio.h>
#include <l4/re/env>
#include <l4/re/util/cap_alloc>
#include <l4/re/util/object_registry>
#include <l4/cxx/ipc_server>

#include "shared.h"


static L4Re::Util::Registry_server<> server;

class Encryption_server : public L4::Server_object {

public:
int dispatch(l4_umword_t obj, L4::Ipc::Iostream &ios);

private:
int dispatch_encrypt(L4::Ipc::Iostream &ios);
int dispatch_decrypt(L4::Ipc::Iostream &ios);

int encrypt(unsigned char *inout_data, unsigned long size);
int decrypt(unsigned char *inout_data, unsigned long size);
unsigned char key();
};

int Encryption_server::dispatch_encrypt(L4::Ipc::Iostream &ios) {

unsigned char *buf = NULL;
unsigned long buf_len = 0;

ios >> L4::Ipc::Buf_in<unsigned char>(buf, buf_len);
if(buf_len > (L4_UTCB_GENERIC_DATA_SIZE-2)*sizeof(l4_umword_t))
return -L4_EBADPROTO;
int ret = encrypt(buf, buf_len);
if(ret != L4_EOK)
return ret;
ios << L4::Ipc::Buf_cp_out<unsigned char>(buf, buf_len); // XXX depends on << implementation
return ret;
}

int Encryption_server::dispatch_decrypt(L4::Ipc::Iostream &ios) {

unsigned char *buf = NULL;
unsigned long buf_len = 0;

ios >> L4::Ipc::Buf_in<unsigned char>(buf, buf_len);
if(buf_len > (L4_UTCB_GENERIC_DATA_SIZE-2)*sizeof(l4_umword_t))
return -L4_EBADPROTO;
int ret = decrypt(buf, buf_len);
if(ret != L4_EOK)
return ret;
ios << L4::Ipc::Buf_cp_out<unsigned char>(buf, buf_len); // XXX depends on << implementation
return ret;
}

int Encryption_server::encrypt(unsigned char *inout_data, unsigned long size) {

if(inout_data == NULL && size == 0)
return -L4_EINVAL;

for(unsigned long i=0; i<size; i++)
inout_data[i] ^= this->key();
return L4_EOK;
}

int Encryption_server::decrypt(unsigned char *inout_data, unsigned long size) {

return this->encrypt(inout_data, size);
}

unsigned char Encryption_server::key() {

#define ENCRYPTION_SERVER_KEY 0x55
return ENCRYPTION_SERVER_KEY;
#undef ENCRYPTION_SERVER_KEY
}

int Encryption_server::dispatch(l4_umword_t, L4::Ipc::Iostream &ios) {

l4_msgtag_t t;
ios >> t;


if (t.label() != Protocol::Encryption)
return -L4_EBADPROTO;

L4::Opcode opcode;
ios >> opcode;

switch(opcode) {
case Opcode::func_encrypt:
return dispatch_encrypt(ios);
case Opcode::func_decrypt:
return dispatch_decrypt(ios);
default:
return -L4_ENOSYS;
}
}



int main() {

static Encryption_server encr;

if (!server.registry()->register_obj(&encr, ENCRYPTION_SERVER_NAME).is_valid()) {
printf("Could not register service, is there a \'%s\' in the caps table?\n", ENCRYPTION_SERVER_NAME);
return 1;
}

printf("Welcome to the encryption server!\n");

server.loop();

return 0;
}
