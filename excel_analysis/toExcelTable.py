import pandas as pd
writer = pd.ExcelWriter('output2.xlsx')   
workbook=writer.book
worksheet=workbook.add_worksheet('Result')
writer.sheets['Result'] = worksheet
df=pd.read_csv("200Nodes.csv")
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
print(len(nameDf))
print(len(timeDf.columns))
for h in range(0,len(nameDf)):
    if (nameDf[h]=="rateStatistic:vector"):
        finalDf.loc[index]=[runDf[h],nameDf[h],timeDf[0][h],valueDf[0][h]]
        finalDf['vectime']=pd.to_numeric(finalDf['vectime'])
        finalDf['vecvalue']=pd.to_numeric(finalDf['vecvalue'])
        finalDf.to_excel(writer,sheet_name='Result',startcol=6*run , index=False)
        run+=1
        index=0
    else:
        for i in range(0,len(timeDf.columns)):
            finalDf.loc[index]=[runDf[h],nameDf[h],timeDf[i][h],valueDf[i][h]]
            index+=1                  
writer.save()