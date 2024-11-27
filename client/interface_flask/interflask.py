import flask
from flask import render_template, request, redirect, url_for
from threading import Thread
import os

app = flask.Flask(__name__)
app.config["DEBUG"] = True

DATA_OUT = (0, 0, 0, 0, 0, 0)

@app.route('/', methods=['GET'])
def main():
  # return "yeepeee"
  return render_template('index.html')

@app.route('/action/<speed>/<info>', methods=['GET'])
def data(speed, info): 
  global DATA_OUT
  k = 1
  element = [0,0,0,0,0,0]
  print(info)
  if ("left" in info):
    element[0] = -k*int(speed)
  if ("right" in info):
    element[0] = k*int(speed)
  if ("forward" in info):
    element[1] = k*int(speed)
  if ("backward" in info):
    element[1] = -k*int(speed)
  if ("rtate" in info):
    element[4] = k*int(speed)
  if ("ltate" in info):
    element[4] = -k*int(speed)  
  
  if ("stop" in info):
    element[0] = 0; element[1] = 0
  DATA_OUT = tuple(element)
  
  # match info:
  #   case "left-forward":
  #     return "left-forward"
  #   case "left":
  #     return "left"
  #   case "left-backward":
  #     return "left-backward"
  #   case "forward":
  #     return "forward"
  #   case "rotate-left":
  #     return "rotate-left"
  #   case "rotate-right":
  #     return "rotate-right"
  #   case "right-forward":
  #     return "right-forward"
  #   case "right":
  #     return "right"
  #   case "right-backward":
  #     return "right-backward"
  #   case "backward":
  #     return "backward"
  #   case "stop": 
  #     return "stop"
  return str(DATA_OUT)
  return "None"

def get_data():
  global DATA_OUT
  print(DATA_OUT)
  return DATA_OUT

def run(): 
  app.run(host='0.0.0.0', port=8080, debug=False)


if __name__ == "__main__":
  t = Thread(target=run)
  t.start()
