# Import standard python modules
import time
import requests
import json
import pprint
from time import sleep
from config import *
from datetime import datetime




# setup pprint
pp = pprint.PrettyPrinter(indent=4)

# Import Adafruit IO REST client.
from Adafruit_IO import Client, Feed

# holds the count for the feed
run_count = 1234

# Create an instance of the REST client.
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)


while True:
    # get weather from api
    url = requests.get(APIURL)
    text = url.text
    data = json.loads(text)

    #print(type(text))

    #pp.pprint(data)

    name = "TEMP"
    temp = (data['main']['temp'])
    temp = round(temp)
    temp = str(temp)
    temp = temp + "F"
    length = len(temp)

    while (length < 4):
        temp = " " + temp
        print("padding")
        length = len(temp)

    print("length is ", length)

    wind = (data['wind']['speed'])
    try:
        wind = str(round(wind))
        wind = wind + "MH"
    except:
        wind = "--"
    w_length = len(wind)

    while (w_length < 4):
        print("padding")
        wind = wind + " "
        w_length = len(wind)

    feel = (data['main']['feels_like'])
    try:
        feel = str(round(feel))
        feel = feel + "F"
    except:
        feel = "--"
    w_length = len(feel)

    while (w_length < 4):
        print("padding")
        feel = feel + " "
        w_length = len(feel)


    # rotate around items for a while
    x = 0
    while x < 20 :
        print('countdown', 20 - x)
        print("rotating display")
        x = x + 1
        sleep(4)
        print('sending data:XXX' + wind + "XXXX")
        aio.send_data('blue1','WIND')
        aio.send_data('red1', wind)
        sleep(4)
        print('sending data:XXX' + temp + "XXXX")
        aio.send_data('blue1', 'TEMP')
        aio.send_data('red1', temp)
        sleep(4)
        # date
        now = datetime.now()
        month = (now.strftime('%b'))
        month = month.upper()
        print("month is" + month)
        month_len = len(month)
        while (month_len < 4):
            print("padding")
            month = month + " "
            month_len = len(month)
        day = (now.strftime('%d'))
        day_len = len(day)
        while (day_len < 4):
            print("padding")
            day = day + " "
            day_len = len(day)
        aio.send_data('red1',month)
        aio.send_data('blue1',day)
        sleep(4)
        # back to weather
        print('sending data:XXX' + feel + "XXXX")
        aio.send_data('blue1', 'FEEL')
        aio.send_data('red1', feel)
        sleep(6)


    # Adafruit IO is rate-limited for publishing
    # so we'll need a delay for calls to aio.send_data()
    print("sleeping 5")
    time.sleep(5)
