/*
 * (c) 2008-2009 Adam Lackorzynski <adam@os.inf.tu-dresden.de>,
 *               Alexander Warg <warg@os.inf.tu-dresden.de>
 *     economic rights: Technische Universität Dresden (Germany)
 *
 * This file is part of TUD:OS and distributed under the terms of the
 * GNU General Public License 2.
 * Please see the COPYING-GPL-2 file for details.
 */
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
  s << l4_umword_t(Opcode::func_encrypt) << L4::Ipc::buf_cp_out(message, size);
  int retrun = l4_error(s.call(server.cap(), Protocol::Encryption));
  if (retrun)
    return retrun; // failure
  //result = new char(sizeof(s));
  s >> L4::Ipc::buf_cp_in(result, size);
  return 0; // ok
}

static int
func_decrypt_call(L4::Cap<void> const &server, char *result, 
             char *message, unsigned long size)
{
  L4::Ipc::Iostream s(l4_utcb());
  s << l4_umword_t(Opcode::func_decrypt) << L4::Ipc::buf_cp_out(message, size);
  int retrun = l4_error(s.call(server.cap(), Protocol::Encryption));
  if (retrun)
    return retrun; // failure
  s >> L4::Ipc::buf_cp_in(result, size);
  return 0; // ok
}

int
main()
{
//  sleep(1);
  L4::Cap<void> server = L4Re::Env::env()->get_cap<void>("crypt_server");
 

  if (!server.is_valid())
    {
      fprintf(stderr ,"Could not get server capability!\n");
      return 1;
    }

  puts("this is da encryptin' bro!\n");
  char str[14]="Hello world!";
  char encstr[14],outstr[14];
  if (func_encrypt_call(server, str, encstr, sizeof(str)))
    {
      fprintf(stderr, "Error talking to server\n");
      return 1;
    }
  printf("Result of da encryptin': %s\n", encstr);
  puts("this is da decryptin' bro!\n");
  if (func_decrypt_call(server, encstr, outstr, sizeof(encstr)))
    {
      fprintf(stderr, "Error talking to server\n");
      return 1;
    }
  printf("Result of da decryptin' call: %s\n", outstr);

  return 0;
}
