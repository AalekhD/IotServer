import sqlite3

def Initialize_Database(databasename):
    databasename =str(databasename)+".db"
    db = sqlite3.connect(databasename)
    conn = sqlite3.connect(databasename)
    cur = conn.cursor()
    cur.execute("""CREATE TABLE IF NOT EXISTS names_table (
        Id INTEGER PRIMARY KEY AUTOINCREMENT,
        first_name text,
        last_name text,
        age integer
        )""")
    conn.commit()

    cur.close()
    conn.close()
    db.close()

def add_data(data,database_name):
    database_name = database_name+'.db'
    conn = sqlite3.connect(database_name)
    cur = conn.cursor()
    cur.execute("INSERT INTO names_table VALUES(:Id, :first_name, :last_name, :age)",
                  {'Id':None,
                   'first_name': data[1],
                   'last_name': data[2],
                   'age': (data[3])
                  })
    conn.commit()
    cur.close()
    conn.close()

def retrieve_data(database_name):
    database_name = database_name+'.db'
    conn = sqlite3.connect(database_name)
    cur = conn.cursor()
    cur.execute("SELECT first_name, last_name, age, MAX(rowid) FROM names_table")
    record = cur.fetchone()
    print(record)
    cur.close()
    conn.close()
    
if __name__ == '__main__':
    databasename = 'data'
    Initialize_Database(databasename)
    data = [1,'Aalakh','Devari',25]
    add_data(data,databasename)
    retrieve_data(databasename)
