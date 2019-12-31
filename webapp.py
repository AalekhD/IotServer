from flask import Flask, render_template
import datetime
import os
import dateutil.parser
import pytz
import requests
app = Flask(__name__)



#not a good idea to keep API_KEY and white-listed Mac address here
API_KEY = "GTW89NF3"
MAC_ADDRESS = "6c:rf:7f:2b:0e:g8"

@app.route("/")
def index():
    #r = requests.get('https://api.thingspeak.com/channels/254616/fields/1/last.txt')
    #temp_c_in = r.text
    temp_c_in = 32 
    temp_f = str(round(((9.0 / 5.0) * float(temp_c_in) + 32), 1)) + ' F'
    return render_template("index.html", temp=temp_f)
'''
@app.route("/update/API_key=<api_key>/mac=<mac>/field=<int:field>/data=<data>", methods=['GET'])
def update(api_key, mac, field, data):
    return render_template("update.html", data=data)
'''
@app.route("/update/API_key=<api_key>/mac=<mac>/field=<int:field>/data=<data>", methods=['GET'])
def write_data_point(api_key, mac, field, data):
    if (api_key == API_KEY and mac == MAC_ADDRESS):
        t = datetime.datetime.now(tz=pytz.timezone('Indian/Maldives'))
        date_time_str = t.isoformat()

        return render_template("showrecent.html", data=data, time_stamp=date_time_str)

    else:
        return render_template("403.html")

if __name__ == "__main__":
    app.run(host='0.0.0.0')