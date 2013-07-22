/***
** Not working currently
***/
#include <l4/re/env.h>
#include <l4/sys/ipc.h>
#include <l4/sys/vcon.h>
//#include <l4/sys/memdesc.h>
#include <l4/re/c/namespace.h>
#include <l4/re/c/util/cap_alloc.h>

#include <string.h>
#include <unistd.h>
#include <stdio.h>

int
main(void)
{
        l4re_env_t *env = l4re_env();       //get environment
        l4_msg_regs_t *mr = l4_utcb_mr();   //get msg regs

//      [...]

        mr->mr[0] = L4_VCON_WRITE_OP;
        mr->mr[1] = 7; // 'hello\n = 6 chars + \0 char
        memcpy(&mr->mr[2], "hello\n",7);

        l4_msgtag_t tag, ret;
        tag = l4_msgtag(L4_PROTO_LOG, 4, /* 4 msg words */
                            0, L4_IPC_NEVER);      



        l4_timeout_s t[2];
        t[0].t = 1; t[1].t = 2;
        l4_timeout_t tmout = l4_timeout(t[0], t[1]);

   
        ret = l4_ipc_send(env->log, l4_utcb(), tag, tmout); //System Call!
                         
                             
                             
        l4_uint32_t err;
        if (err = l4_msgtag_has_error(ret)) {
                //error handling
                fprintf(stderr,"Error has occured");
                //printf("Error has occured %i\n", err);
                return 1;
        }
        free(mr);
        return 0;
}
                     

 
