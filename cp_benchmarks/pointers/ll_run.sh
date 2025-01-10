sudo sysctl vm.v_cheri_prefetch=1
sudo sysctl vm.stats.vm.v_swapin=0
sudo sysctl vm.v_majorfault=0
sudo sysctl vm.v_softfault=0
sudo sysctl vm.v_majorfault_latency=0
sudo sysctl vm.v_async_io_latency=0
sudo sysctl vm.v_prefetches=0
sudo sysctl vm.v_prefetch_latency=0
sudo sysctl vm.v_blocked_softfault=0
sudo sysctl vm.v_prefetches=0
sudo sysctl vm.v_cheri_softfault=0
/usr/bin/time -al ./ll 17 0 1
sudo sysctl vm.v_majorfault
sudo sysctl vm.v_softfault
sudo sysctl vm.v_cheri_softfault
sudo sysctl vm.v_blocked_softfault
sudo sysctl vm.v_prefetches
sudo sysctl vm.stats.vm.v_swapin

#bin/parsecmgmt -l -k -a run -p canneal -i native > output2 &
#bin/parsecmgmt -l -k -a run -p canneal -i native > output3 &
#bin/parsecmgmt -l -k -a run -p canneal -i native > output4 &
