#include<sys/sysctl.h>
#include<assert.h>

const char* important_stat[] = {
        "vm.v_prefetches", 
        "vm.v_majorfault",
        "vm.v_majorfault_latency",
        "vm.v_async_io_latency",
        "vm.v_prefetches",
        "vm.v_prefetch_latency",
        "vm.v_blocked_softfault",
        "vm.v_prefetches"
        };

struct run_stat{
        char *name;
        //bool cheri_picking_enabled;
        //int cycle_per_node;
        int depth;
        struct counter{
                char *name;
                int value;
        } counters[5];
        int duration_second;
};

int sysctlRead(const char * name) {

       int mib[2];
       size_t mib_len, len;
       int rv;
       int value;

       mib_len = 2;
       rv = sysctlnametomib(name, mib, &mib_len);
       if (rv) {
               printf("ERROR: sysctlnametomib %s, RV: %d, errno: %d\n",
                              name, rv, errno);
               exit(1);

       }


       // Get the len of the sysctl value
       rv = sysctl(mib, mib_len, NULL, &len, NULL, 0);
       if (rv) {
               printf("ERROR: sysctl %s, RV: %d, errno: %d\n",
                               name, rv, errno);
               exit(1);
       }


       rv = sysctl(mib,  mib_len, &value, &len, NULL, 0);
       if (rv) {
               printf("ERROR: sysctl %s, RV: %d, errno: %d\n",
                               name, rv, errno);
               exit(1);

       }

       return value;
}

int sysctlWrite(const char * name, int value) {

       int mib[2];
       size_t mib_len, len;
       int rv;
       int oldValue;

       mib_len = 2;
       rv = sysctlnametomib(name, mib, &mib_len);
       if (rv) {
               printf("ERROR: sysctlnametomib %s, RV: %d, errno: %d\n",
                              name, rv, errno);
               exit(1);

       }

       // Get the len of the sysctl value
       rv = sysctl(mib, mib_len, NULL, &len, NULL, 0);
       if (rv) {
               printf("ERROR: sysctl %s, RV: %d, errno: %d\n",
                               name, rv, errno);
               exit(1);
       }


       int newValue = value;
       rv = sysctl(mib,  mib_len, &oldValue, &len, &newValue, len);
       if (rv) {
               printf("ERROR: sysctl %s, RV: %d, errno: %d\n",
                               name, rv, errno);
               exit(1);

       }
       
       if(newValue != value) {
               printf("ERROR: sysctl %s, new value not set."
                      "expectedNewValue: %d actualnewValue: %d\n",
                      name, value, newValue);
                               
               exit(1);

       }

       return value;
}

void clearCounters() {
        for (int i = 0; i < sizeof(important_stat)/sizeof(char*); i++) {
                //printf("Clearning: %s\n", important_stat[i]);
                sysctlWrite(important_stat[i], 0);

        }
}


void printCounters() {
        for (int i = 0; i < sizeof(important_stat)/sizeof(char*); i++) {
                int value = sysctlRead(important_stat[i]);
                printf("\"%s\": %d,", important_stat[i], value);

        }
}
