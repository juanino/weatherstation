FROM python:3.9 
# Or any preferred Python version.
ADD config_sample.py .
ADD publish_openweather.py .
# run something
RUN pip install requests
RUN pip install Adafruit_IO
CMD python publish_openweather.py
