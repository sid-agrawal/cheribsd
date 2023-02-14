

struct run_stat{
        char *name;
        bool cheri_picking_enabled;
        int cycle_per_node;
        int depth;
        struct counter{
                char *name;
                int value;
        } counters[5];
        int duration_second;
};
