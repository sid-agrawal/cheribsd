#!/bin/python

import pandas as pd
import sys
import json
import pprint
import argparse
pp = pprint.PrettyPrinter(indent=4)




def handleLL(data):
    return



def printSortedTable(test, file):
 
    # Opening JSON file
    with open(file) as json_file:
        data = json.load(json_file)
    

    # Row ID
    index =[]
    for key in data:
        index.append(key)
        # Sanity Check that we have the right data
        assert(data[key]['Test'] == test)

    # Column Names
    columns = []
    for key in data:
        for key2 in data[key]:
            columns.append(key2)
        # Insert only for dict, hence the break.
        break

    # Make the DF and populate it.
    df = pd.DataFrame(columns=columns, index=index)
    for key in data:
         df.loc[key] = pd.Series( data[key] )


    # Sort it by CP and Delay
    df.sort_values(by=['CPEnabled', 'Delay'], inplace=True, ascending=[True,True])
    pp.pprint(df)


def main(argv):
    parser = argparse.ArgumentParser()
    parser.add_argument('-t','--test', help='Which test does the log belong too', required=True)
    parser.add_argument('-f','--file', help='log file', required=True)

    args = parser.parse_args()
    printSortedTable(args.test, args.file)

if __name__ == "__main__":
    main(sys.argv[1:])
