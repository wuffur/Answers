/*
 * (c) 2008-2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>,
 *               Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#include <stdio.h>
#include <l4/re/env>
#include <l4/re/util/cap_alloc>
#include <l4/re/util/object_registry>
#include <l4/cxx/ipc_server>
#include <ctype.h>

#include "shared.h"

static L4Re::Util::Registry_server<> server;

class Encryption_server : public L4::Server_object
{
public:
  int dispatch(l4_umword_t obj, L4::Ipc::Iostream &ios);
 int encrypt(char *buffer, unsigned long buffer_size);
 int decrypt(char *buffer, unsigned long buffer_size);
private:
 char backverse(const char c);

};

int
Encryption_server::dispatch(l4_umword_t, L4::Ipc::Iostream &ios)
{
  l4_msgtag_t t;
  ios >> t;

  // We're only talking the calculation protocol
  if (t.label() != Protocol::Encryption)
    return -L4_EBADPROTO;
  L4 :: Opcode opcode;
  ios >> opcode;
  int retrun;
  char *buffer = NULL;
  unsigned long buffer_size = 0;
  switch (opcode)
    {
    case Opcode::func_encrypt:
      ios >> L4::Ipc::Buf_in<char>(buffer, buffer_size);
      if(buffer_size > (L4_UTCB_GENERIC_DATA_SIZE-2)*sizeof(l4_umword_t))
	    return  -L4_EBADPROTO;	
      retrun = encrypt(buffer, buffer_size);
      if(retrun != L4_EOK)
	   return retrun;
      ios << L4::Ipc::Buf_cp_out<char>(buffer, buffer_size);
      return L4_EOK;
    case Opcode::func_decrypt:
      ios >> L4::Ipc::Buf_in<char>(buffer, buffer_size);
      if(buffer_size > (L4_UTCB_GENERIC_DATA_SIZE-2)*sizeof(l4_umword_t))
	    return  -L4_EBADPROTO;	
      retrun = decrypt(buffer, buffer_size);
      if(retrun != L4_EOK)
	   return retrun;
      ios << L4::Ipc::Buf_cp_out<char>(buffer, buffer_size);
      return L4_EOK;
    default:
      return -L4_ENOSYS;
    };
}
int Encryption_server::encrypt(char *buffer, unsigned long buffer_size){
	if(buffer == NULL && buffer_size == 0)
		return -L4_EINVAL;
	       char temp[buffer_size];	
	for(unsigned long i=0; i<buffer_size; i++)
       		temp[i]	= buffer[i];

	for(unsigned long i=0; i<buffer_size; i++)
		buffer[buffer_size-i-1] = backverse(temp[i]);
	return 0;
}

int Encryption_server::decrypt(char *buffer, unsigned long buffer_size){
	if(buffer == NULL && buffer_size == 0)
		return -L4_EINVAL;
       char temp[buffer_size];	
	for(unsigned long i=0; i<buffer_size; i++)
       		temp[i]	= buffer[i];

	for(unsigned long i=0; i<buffer_size; i++)
		buffer[buffer_size-i-1] = backverse(temp[i]);
	return 0;
}
 char Encryption_server::backverse(const char c){
      if (islower(c))
	return ('z'-c+'a');
      if (isupper(c))
	return ('Z'-c+'A');
      else
	return c;
	}

int
main()
{
  static Encryption_server encr;



  // Register calculation server
  if (!server.registry()->register_obj(&encr, "crypt_server").is_valid())
    {
      printf("Something not let me doing server, is there 'encryption_server' in the caps table?\n");
      return 1;
    }

  printf("Welcome to dat Encryption server!\n"
         "I am wonderous magnificient part of program, doing magic.\n");

  // Wait for client requests
  server.loop();

  return 0;
}

int
main()
{
  static Encryption_server encr;



  // Register calculation server
  if (!server.registry()->register_obj(&encr, "crypt_server").is_valid())
    {
      printf("Something not let me doing server, is there 'encryption_server' in the caps table?\n");
      return 1;
    }

  printf("Welcome to dat Encryption server!\n"
         "I am wonderous magnificient part of program, doing magic.\n");

  // Wait for client requests
  server.loop();

  return 0;
}
