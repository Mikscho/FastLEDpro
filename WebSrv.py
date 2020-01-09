from flask import Flask, render_template
import socket
import datetime
app = Flask(__name__)
IP1 = '192.168.10.20'
IP2 = '192.168.10.30'
PORTNUM = 8888
PACKETDATA = b''

@app.route("/")
def index():
    templateData = {}
    return render_template('index.html', **templateData)
@app.route("/<animation>")
def action(animation):
    if animation != "favicon.ico":
        PACKETDATA = str.encode(animation)
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        t = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
        s.connect((IP1, PORTNUM))
        t.connect((IP2, PORTNUM))
        s.sendall(PACKETDATA)
        t.sendall(PACKETDATA)
        s.close
        t.close
        templateData = {}
    return render_template('index.html', **templateData)
if __name__ == "__main__":
   app.run(host='0.0.0.0', debug=True)