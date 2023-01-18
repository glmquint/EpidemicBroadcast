import pandas as pd
writer = pd.ExcelWriter('output.xlsx')   
workbook=writer.book
worksheet=workbook.add_worksheet('Validation')
writer.sheets['Validation'] = worksheet
df=pd.read_csv("Untitled.csv")
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
for h in range(0,len(nameDf)):
    for i in range(0,len(timeDf.columns)):
        finalDf.loc[index]=[runDf[h],nameDf[h],timeDf[i][h],valueDf[i][h]]
        index+=1
        if(index==225):
            finalDf['vectime']=pd.to_numeric(finalDf['vectime'])
            finalDf['vecvalue']=pd.to_numeric(finalDf['vecvalue'])
            finalDf.to_excel(writer,sheet_name='Validation',startcol=6*run , index=False)
            run+=1
            index=0      
writer.save()