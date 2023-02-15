#!/usr/local64/bin/python

import argparse
import sys
import subprocess
from python_sysctl import sysctlRead, sysctlWrite


important_stat = ( 
        b"vm.v_prefetches", 
        b"vm.v_majorfault",
        b"vm.v_majorfault_latency",
        b"vm.v_async_io_latency",
        b"vm.v_prefetches",
        b"vm.v_prefetch_latency",
        b"vm.v_blocked_softfault",
        b"vm.v_prefetches"
        )

def debugPrint(args):
    print()

def runCommand( cmd ):
    result = subprocess.run(cmd, capture_output=True)
    print(repr(result.stdout))
  
def enableCP():
    newValue = sysctlWrite(b"vm.v_cheri_prefetch", 1)
    assert(newValue == 1)

def disableCP():
    newValue = sysctlWrite(b"vm.v_cheri_prefetch", 0)
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


def handle_ll(subparser):
    print("In ll", subparser)

def handle_tree(args):
    print("Enabling CP")
    enableCP()

    for x in range(args.start_delay, args.end_delay, args.step_delay):
        runCommand([ "./tree", str(args.depth), str(x)])


    print("Disabling CP")
    disableCP()
        
    for x in range(args.start_delay, args.end_delay, args.step_delay):
        runCommand([ "./tree", str(args.depth), str(x)])

if __name__ == "__main__":
    main(sys.argv[1:])
