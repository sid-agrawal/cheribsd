#!/usr/local64/bin/python

import argparse
import sys
import subprocess


def runCommand( cmd ):
    result = subprocess.run(cmd, capture_output=True)
    print(repr(result.stdout))
  
def enableCP():
    print("enabling the magical CHERI picking")

def disableCP():
    print("enabling the magical CHERI picking")

def resetStats():
    print("Resetting Stats")

def collectStats():
    print("Collecting Stats")

def main(argv):


    parser = argparse.ArgumentParser()

    # create sub-parser
    sub_parsers = parser.add_subparsers(help='sub-command help')

    # create the parser for the "ll" sub-command
    parser_ll = sub_parsers.add_parser('ll', help='Linked list pointer benchmark')
    parser_ll.add_argument('--size', type=int, help='Number of node of the LL')
    parser_ll.add_argument('--start-delay', type=int, help='Delay for each node visited as loop count')
    parser_ll.add_argument('--step-delay', type=int, help='Delay Increments')
    parser_ll.add_argument('--end-delay', type=int, help='End Delay')

    # create the parser for the "tree" sub-command
    parser_tree = sub_parsers.add_parser('tree', help='Tree pointer benchmark')
    parser_tree.add_argument('--start-delay', type=int, help='Delay for each node visited as loop count')
    parser_tree.add_argument('--step-delay', type=int, help='Delay Increments')
    parser_tree.add_argument('--end-delay', type=int, help='End Delay')
    parser_tree.add_argument('--depth', type=int, help='Depth of Tree')
    
    args = parser.parse_args()
    print("args=%s" % args)

    print("args.name=%s" % args.test)
    cmd = [ args.test, "-la" ]
    #runCommand(cmd)

    for x in range(args.start_delay, args.end_delay, args.step_delay):
        print(x)

if __name__ == "__main__":
    main(sys.argv[1:])
