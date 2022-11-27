###########################################################################
# Author: Mathieu Beaudoin
# Date : 2022-11-24
# Description : Server running the web GUI of the SSRS prototype
###########################################################################

### Imports
from flask import Flask, render_template, request
import serial

### global variable
esp32 = serial.Serial()

### Flask things
app = Flask(__name__)

### Main page url
@app.route("/", methods=['GET', 'POST'])
def index():

    # Buttons implementation
    if request.method == 'POST':

        # When Manual button is pressed
        if request.form.get('manuel') :
            # Try to connect to ESP32 at COM4
            try:
                esp32.port = "COM4"
                esp32.baudrate = 9600
                if(esp32.isOpen() == False):
                    esp32.open()
                else:
                    esp32.close()
                    esp32.open()
                print("--------------------")        
                print("CONNECTED TO")
                print(esp32)
                print("--------------------")
            except:
                print("--------------------")
                print("NOT CONNECTED")
                print("--------------------")

        # When the button is pressed
        if request.form.get('open1') :
            # If the port is connected
            if(esp32.isOpen()):
                # Send a number through serial
                esp32.write('1'.encode('utf-8'))
                print('open')

        elif  request.form.get('open2') :
            if(esp32.isOpen()):
                esp32.write('2'.encode('utf-8'))

        elif  request.form.get('open3') :
            if(esp32.isOpen()):
                esp32.write('3'.encode('utf-8'))

        elif  request.form.get('open4') :
            if(esp32.isOpen()):
                esp32.write('4'.encode('utf-8'))

        elif  request.form.get('close') :
            if(esp32.isOpen()):
                esp32.write('0'.encode('utf-8')) 
                print('close')      

    return render_template("index.html")

if __name__=="__main__":
    app.run(debug=True)