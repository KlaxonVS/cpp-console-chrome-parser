import os
import sys
import time
import subprocess
import requests
import websockets
import asyncio

dir = os.path.dirname(os.path.abspath(__file__))

async def get_html(debug_url: str):
    while True:
        try:
            async with websockets.connect(debug_url, max_size=2**30) as websocket:  # Set max_size to a larger value
                await websocket.send('{"id":1, "method":"DOM.getDocument", "params":{"depth":-1}}')
                response = await websocket.recv()
                return response  # Print the response obtained from executing DOM.getDocument
                break  # Break the loop after receiving the response
        except websockets.exceptions.ConnectionClosedError:
            print("Connection to the websocket was closed unexpectedly. Reconnecting...")
            continue  # Continue to the next iteration of the loop to reconnect


if __name__ == '__main__':
  FNULL = open(os.devnull, 'w')    #use this if you want to suppress output to stdout from the subprocess
  filename = "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe --remote-debugging-port=9222 --headless"
  subprocess.Popen(filename, stdout=FNULL, stderr=FNULL, shell=False)
  response = requests.put('http://127.0.0.1:9222/json/new?https://petrovich.ru/product/1033555/', timeout=3)
  print(response.status_code)
  time.sleep(10)
  ws_url = response.json().get('webSocketDebuggerUrl')
  res = asyncio.run(get_html(ws_url))
  decoded_string = bytes(res, 'utf-8').decode()
  file = open(dir + "\\ws_chrome.txt", "w", encoding="utf-8")
  file.write(decoded_string)
  file.close()

