import pandas as pd
from sys import argv

print(argv)
if len(argv) < 2:
    print(f"Usage: {argv[0]} file.csv")
    quit()

num_rep = 33
num_scenarios = 24
header_block_size = 27
data_block_size = 24
offset = header_block_size * num_rep * num_scenarios
prob_offset = 23
radius_offset = 24

d = {}

def f(k, v):
    d[k] = v

in_file = argv[1]
writer = pd.ExcelWriter(f"output_{in_file.split('.csv')[0]}.xlsx", engine='xlsxwriter')
workbook = writer.book
df = pd.read_csv(in_file)
df.groupby(["run"]).apply(lambda x:f(int(x.iat[0,0].split('-')[1]), x.copy()))


for i in range(0, len(d), 33): #foreach scenario
    p = d[i].iat[23, 5] #p
    r = d[i].iat[24, 5] #r
    scenario = f"{p}p-{r}r"
    print(scenario)
    worksheet = workbook.add_worksheet(scenario)
    writer.sheets[scenario] = worksheet
    arr = []
    indx = []
    for j in range(0, 33): #foreach repetition
        k = i+j
        #print(f"{k = }")
        vectime = d[k].iat[27, 6].split(' ') #vectime
        col = vectime
        for w in range(27, 27 + 5*4, 4): #foreach stat
            #print(f"{w = }")
            name = d[k].iat[w+3, 5] #name
            indx.append(name)
            vecvalue = d[k].iat[w, 7].split(' ') #:vecvalue
            arr.append(vecvalue)
        endtime = d[k].iat[47, 6].split(' ') #endtime
        arr.append(endtime)
        indx.append('z_endtime')
        endrate = d[k].iat[47, 7].split(' ') # endrate
        arr.append(endrate)
        indx.append('z_endrate')
        #for stat in ['waiting', 'oneMsg', 'collision', 'ready', 'done']:
    target = pd.DataFrame(arr, columns=col, index=indx).sort_index()
    target[target.columns] = target[target.columns].apply(pd.to_numeric, errors='coerce')
    #print(target)
    target.to_excel(writer, sheet_name=scenario)

writer.save()

