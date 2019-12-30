from flask import Flask, render_template
import requests
app = Flask(__name__)

@app.route("/")
def index():
    #r = requests.get('https://api.thingspeak.com/channels/254616/fields/1/last.txt')
    #temp_c_in = r.text
    temp_c_in = 32 
    temp_f = str(round(((9.0 / 5.0) * float(temp_c_in) + 32), 1)) + ' F'
    return render_template("index.html", temp=temp_f)

@app.route("/update/API_key=<api_key>/mac=<mac>/field=<int:field>/data=<data>", methods=['GET'])
def update(api_key, mac, field, data):
    return render_template("update.html", data=data)

if __name__ == "__main__":
    app.run(host='0.0.0.0')