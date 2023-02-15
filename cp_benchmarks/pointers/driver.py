#!/usr/local64/bin/python

import argparse
import sys
import subprocess


def runCommand( cmd ):
    result = subprocess.run(cmd, capture_output=True)
    print(repr(result.stdout))
  


def main(argv):


    argParser = argparse.ArgumentParser()
    argParser.add_argument("-t", "--test", help="Test name: tree, ll")

    argParser.add_argument("--start-delay", help="Delay to start", type=int)
    argParser.add_argument("--step-delay", help="Delay to step", type=int)
    argParser.add_argument("--end-delay", help="Delay to end", type=int)

    args = argParser.parse_args()
    print("args=%s" % args)

    print("args.name=%s" % args.test)
    cmd = [ args.test, "-la" ]
    #runCommand(cmd)

    for x in range(args.start_delay, args.end_delay, args.step_delay):
        print(x)

if __name__ == "__main__":
    main(sys.argv[1:])
