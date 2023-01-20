import pandas as pd
from sys import argv
import openpyxl

if (len(argv) < 2):
    print(f"USAGE: {argv[0]} file1.csv [fileN.csv]\n    Example:\n{argv[0]} *.csv")
    quit()

for in_file in argv[1:]:
    print(in_file)
    df=pd.read_csv(in_file)
    writer = pd.ExcelWriter(f'output_{in_file}.xlsx', engine='xlsxwriter')   
    workbook=writer.book
    worksheet=workbook.add_worksheet('Result')
    writer.sheets['Result'] = worksheet
    df=df[['run','name','vectime','vecvalue']]
    df=df.dropna(subset=['vectime'])
    df=df.reset_index()
    runDf=df['run']
    nameDf=df['name']
    timeDf=df['vectime'].str.split(' ', expand=True)
    valueDf=df['vecvalue'].str.split(' ', expand=True)
    finalDf = pd.DataFrame(columns=('run', 'name', 'vectime','vecvalue'))
    index=0
    run=0
    #print(len(nameDf))
    #print(len(timeDf.columns))
    for row in range(0,len(nameDf)):
        if (nameDf[row]=="rateStatistic:vector"):
            finalDf.loc[index]=[runDf[row],nameDf[row],timeDf[0][row],valueDf[0][row]]
            finalDf['vectime']=pd.to_numeric(finalDf['vectime'])
            finalDf['vecvalue']=pd.to_numeric(finalDf['vecvalue'])
            finalDf.to_excel(writer,sheet_name='Result',startcol=6*run , index=False)
            run+=1
            index=0
        else:
            for col in range(0,len(timeDf.columns)):
                finalDf.loc[index]=[runDf[row],nameDf[row],timeDf[col][row],valueDf[col][row]]
                index+=1                  
    writer.save()
