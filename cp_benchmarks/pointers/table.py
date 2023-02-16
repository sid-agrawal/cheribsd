#!/bin/python

from prettytable import PrettyTable
import pandas as pd
import pprint
pp = pprint.PrettyPrinter(indent=4)



data = {
        "d1": {
                "a" : 1,
                "b" : 2
            },
        "d2": {
                "a" : 11,
                "b" : 22
            },
        }
pp.pprint(data)


# Specify the Column Names while initializing the Table
myTable = PrettyTable(["no", "a", "b"])
 
# Add rows
for key, value in data.items():
    myTable.add_row([key, value["a"], value["b"] ])
 
print(myTable)


df = pd.DataFrame(columns=['a','b'], index=['d1','d2'])

df.loc['d1'] = pd.Series (data['d1']) 
df.loc['d2'] = pd.Series (data['d2']) 

pp.pprint(df)

df.sort_values(by=['a'], inplace=True, ascending=False)
pp.pprint(df)


def printTableFromJSON(jsonFile):

