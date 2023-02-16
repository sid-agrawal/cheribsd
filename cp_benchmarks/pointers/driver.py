#!/usr/local64/bin/python

import argparse
import sys
import subprocess
from python_sysctl import sysctlRead, sysctlWrite
import pprint
import re
import json
import tempfile
import time


important_stat = ( 
        "vm.v_prefetches", 
        "vm.v_majorfault",
        "vm.v_majorfault_latency",
        "vm.v_async_io_latency",
        "vm.v_prefetches",
        "vm.v_prefetch_latency",
        "vm.v_blocked_softfault",
        "vm.v_prefetches"
        )

def debugPrint(args):
    print()

def runCommand( cmd ):
    print("Running:", cmd)
    result = subprocess.run(cmd, capture_output=True, text=True, check=True)
    output = result.stdout
    return output
  
def enableCP():
    name ="vm.v_cheri_prefetch"
    newValue = sysctlWrite(name, 1)
    assert(newValue == 1)

def disableCP():
    name ="vm.v_cheri_prefetch"
    newValue = sysctlWrite(name, 0)
    assert(newValue == 0)

def resetStats():
    for x in important_stat:
        sysctlWrite(x, 0)

def collectStats():
    stats = {}
    for x in important_stat:
        stats[ x ] = sysctlRead(x)
    return stats

def main(argv):
    parser = argparse.ArgumentParser()
    parser.add_argument('-d','--debug', help='Debug Enabled')

    # create sub-parser
    sub_parsers = parser.add_subparsers(help='sub-command help', dest='subcommand')

    # create the parser for the "ll" sub-command
    parser_ll = sub_parsers.add_parser('ll', help='Linked list pointer benchmark')
    parser_ll.add_argument('--size', type=int, help='Number of node of the LL')
    parser_ll.add_argument('--start-delay', type=int, help='Delay for each node visited as loop count')
    parser_ll.add_argument('--step-delay', type=int, help='Delay Increments')
    parser_ll.add_argument('--end-delay', type=int, help='End Delay')
    parser_ll.set_defaults(func=handle_ll)

    # create the parser for the "tree" sub-command
    parser_tree = sub_parsers.add_parser('tree', help='Tree pointer benchmark')
    parser_tree.add_argument('--start-delay', type=int, help='Delay for each node visited as loop count')
    parser_tree.add_argument('--step-delay', type=int, help='Delay Increments')
    parser_tree.add_argument('--end-delay', type=int, help='End Delay')
    parser_tree.add_argument('--depth', type=int, help='Depth of Tree')
    parser_tree.set_defaults(func=handle_tree)
    
    args = parser.parse_args()
    args.func(args)

def recompileLL():
    runCommand(["rm", "-f", "./ll"])
    runCommand(["gmake", "ll"])


def handle_ll(subparser):

    recompileLL()

    # The format of allStat is as follows:
    # subcommand, CP-Enabled, 
    allStats = {}
    expt_idx = 1
    pp = pprint.PrettyPrinter(indent=4)
    print("====================Enabling CP====================================")
    enableCP()

    for x in range(args.start_delay, args.end_delay, args.step_delay):
        # Beign extra cautious
        assert(sysctlRead("vm.v_cheri_prefetch") == 1)


        #output = runCommand([ "./ll", str(args.depth), str(x)])
        output = runCommand([ "./ll" ])
        outputDict = json.loads(output)
        pprint.pprint(outputDict)
        print("===============")

        key = str(expt_idx) + "-" + "CP_ON-" +  \
            args.subcommand + "-" + str(args.depth) + "-" + str(x)
        allStats[key] = outputDict
        expt_idx += 1


    print("====================Disabling CP====================================")
    disableCP()
        
    for x in range(args.start_delay, args.end_delay, args.step_delay):
        # Beign extra cautious
        assert(sysctlRead("vm.v_cheri_prefetch") == 0)

        # Run the command
        #output = runCommand([ "./ll", str(args.depth), str(x)])
        output = runCommand([ "./ll" ])
        outputDict = json.loads(output)
        pprint.pprint(outputDict)
        print("===============")

        key = str(expt_idx) + "-" + "CP_OFF-" +  \
            args.subcommand + "-" + str(args.depth) + "-" + str(x)
        allStats[key] = outputDict
        expt_idx += 1
        
    pprint.pprint(allStats)
    pre='log_' + args.subcommand + '_' + time.strftime("%Y%m%d-%H%M%S")+ '_'
    with tempfile.NamedTemporaryFile(mode='w',dir='.', 
            delete=False, prefix=pre) as fp:
        print("All Stats in:", fp.name) 
        fp.write(json.dumps(allStats, sort_keys=True, indent=4))


def recompileTree():
    runCommand(["rm", "-f", "./tree"])
    runCommand(["gmake", "tree"])

def handle_tree(args):

    recompileTree()

    # The format of allStat is as follows:
    # subcommand, CP-Enabled, 
    allStats = {}
    expt_idx = 1
    pp = pprint.PrettyPrinter(indent=4)
    print("====================Enabling CP====================================")
    enableCP()

    for x in range(args.start_delay, args.end_delay, args.step_delay):
        # Beign extra cautious
        assert(sysctlRead("vm.v_cheri_prefetch") == 1)


        output = runCommand([ "./tree", str(args.depth), str(x)])
        outputDict = json.loads(output)
        pprint.pprint(outputDict)
        print("===============")

        key = str(expt_idx) + "-" + "CP_ON-" +  \
            args.subcommand + "-" + str(args.depth) + "-" + str(x)
        allStats[key] = outputDict
        expt_idx += 1


    print("====================Disabling CP====================================")
    disableCP()
        
    for x in range(args.start_delay, args.end_delay, args.step_delay):
        # Beign extra cautious
        assert(sysctlRead("vm.v_cheri_prefetch") == 0)

        # Run the command
        output = runCommand([ "./tree", str(args.depth), str(x)])
        outputDict = json.loads(output)
        pprint.pprint(outputDict)
        print("===============")

        key = str(expt_idx) + "-" + "CP_OFF-" +  \
            args.subcommand + "-" + str(args.depth) + "-" + str(x)
        allStats[key] = outputDict
        expt_idx += 1
        
    pprint.pprint(allStats)
    pre='log_' + args.subcommand + '_' + time.strftime("%Y%m%d-%H%M%S")+ '_'
    with tempfile.NamedTemporaryFile(mode='w',dir='.', 
            delete=False, prefix=pre) as fp:
        print("All Stats in:", fp.name) 
        fp.write(json.dumps(allStats, sort_keys=True, indent=4))

if __name__ == "__main__":
    main(sys.argv[1:])
