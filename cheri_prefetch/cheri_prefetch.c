#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/vmmeter.h>
#include <vm/vm.h>
static int cheri_prefetch_event_handler(struct module *module, int event_type, void *arg) {

  int retval = 0;                   // function returns an integer error code, default 0 for OK

  switch (event_type) {             // event_type is an enum; let's switch on it
    case MOD_LOAD:                  // if we're loading
      	printf("Soft faults %lu, Major swap in faults %lu, Successful prefetches %lu, Blocking faults %lu\n", VM_CNT_FETCH(v_softfault), VM_CNT_FETCH(v_majfault), VM_CNT_FETCH(v_prefetch), VM_CNT_FETCH(v_intrans_soft));
	set_cheri_prefetch(1);
	VM_CNT_ZERO(v_softfault);
	VM_CNT_ZERO(v_majfault);
	VM_CNT_ZERO(v_prefetch);
	VM_CNT_ZERO(v_intrans_soft);


	break;

    case MOD_UNLOAD:                // if were unloading
      	printf("Soft faults %lu, Major swap in faults %lu, Successful prefetches %lu, Blocking soft aults %lu\n", VM_CNT_FETCH(v_softfault), VM_CNT_FETCH(v_majfault), VM_CNT_FETCH(v_prefetch), VM_CNT_FETCH(v_intrans_soft));
	set_cheri_prefetch(0);
	VM_CNT_ZERO(v_softfault);
	VM_CNT_ZERO(v_majfault);
	VM_CNT_ZERO(v_prefetch);
	VM_CNT_ZERO(v_intrans_soft);

      uprintf("Counters reset\n");      // spit out a loading message
      break;

    default:                        // if we're doing anything else
      retval = EOPNOTSUPP;          // return a 'not supported' error
      break;
  }

  return(retval);                   // return the appropriate value

}

static moduledata_t cheri_prefetch_data = {
  .name = "cheri_prefetch",            // Name of our module
  .evhand = cheri_prefetch_event_handler,        // Name of our module's 'event handler' function
};

DECLARE_MODULE(cheri_prefetch, cheri_prefetch_data, SI_SUB_LAST, SI_ORDER_ANY);

