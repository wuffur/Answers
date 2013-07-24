/*
 * (c) 2008-2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>,
 *               Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
#include <l4/re/env>
#include <l4/sys/ipc.h>
#include <l4/sys/vcon>

#include <l4/sys/err.h>
#include <l4/sys/types.h>
#include <l4/re/env>
#include <l4/re/util/cap_alloc>
#include <l4/cxx/ipc_stream>
#include <string.h>
#include <stdio.h>

#include "shared.h"

static int
func_encrypt_call(L4::Cap<void> const &server, char *result,
              char *message, unsigned long size)
{
  L4::Ipc::Iostream s(l4_utcb());
  s << l4_umword_t(Opcode::func_encrypt) << L4::Ipc::Buf_cp_out<char>(message, size);
  int retrun = l4_error(s.call(server.cap(), Protocol::Encryption));
  if (retrun)
    return retrun; // failure
  //result = new char(sizeof(s));
  s >> L4::Ipc::Buf_cp_in<char>(result, size);
  return 0; // ok
}

static int
func_decrypt_call(L4::Cap<void> const &server, char *result, 
             char *message, unsigned long size)
{
  L4::Ipc::Iostream s(l4_utcb());
  s << l4_umword_t(Opcode::func_decrypt) << L4::Ipc::Buf_cp_out<char>(message, size);
  int retrun = l4_error(s.call(server.cap(), Protocol::Encryption));
  if (retrun)
    return retrun; // failure
  s >> L4::Ipc::Buf_cp_in<char>(result, size);
  return 0; // ok
}

int
main()
{
  L4::Cap<void> server = L4Re::Env::env()->get_cap<void>("crypt_server");
 

  if (!server.is_valid())
    {
      fprintf(stderr ,"Could not get server capability!\n");
      return 1;
    }

  puts("this is da encryptin' bro!\n");
  char str[13]="Hello world!";
  char encstr[13],outstr[13];
  for(long int i=0;i<13;i++)
	printf("%c",str[i]);
  printf("\n");	
  int retrun = func_encrypt_call(server, str, encstr, 13*sizeof(char));
  if (retrun !=L4_EOK)
    {
      printf("Error talking to server\n");
      return 1;
    }
  printf("Result of da encryptin':");
  for(long int i=0;i<13;i++)
	printf("%c",encstr[i]);
  printf("\n");	
  puts("this is da decryptin' bro!\n");
  retrun = func_decrypt_call(server, encstr, outstr, 13*sizeof(char));
  if (retrun != L4_EOK)
    {
      printf("Error talking to server\n");
      return 1;
    }
  printf("Result of da decryptin' call:"); 
  for(long int i=0;i<13;i++)
	printf("%c",outstr[i]);
  printf("\n");	
//  delete str;
//  delete encstr;
//  delete outstr;
  return 0;
}
