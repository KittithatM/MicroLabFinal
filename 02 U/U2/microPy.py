from machine import Pin
import socket
import network
import time

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
if not wlan.isconnected():
    print('Connecting to Wi-Fi...')
    wlan.connect('test', '11111111')
    while not wlan.isconnected():
        time.sleep(1)
print('Connected! IP', wlan.ifconfig()[0])

led0 = Pin(2, Pin.OUT)
led1 = Pin(4, Pin.OUT)

def web_page():
    html = """<!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>U2</title>
    </head>
    <body>
        <h1>MicroPython U2</h1> 
        <p>LED0 State : <strong>""" + ("ON" if led0.value() else "OFF") + """</strong></p>
        <a href="/?led0=on"><button>ON</button></a>
        <a href="/?led0=off"><button>OFF</button></a>
        <p>LED1 State : <strong>""" + ("ON" if led1.value() else "OFF") + """</strong></p>
        <a href="/?led1=on"><button>ON</button></a>
        <a href="/?led1=off"><button>OFF</button></a>
    </body>
    </html> """

    return html

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('',80))
s.listen(5)

while(True):
    conn, addr = s.accept()
    request = conn.recv(1024).decode('utf-8')
    
    if '/?led0=on' in request:
        led.value(1)
    if '/?led0=off' in request:
        led.value(0)
    if '/?led1=on' in request:
        led.value(1)
    if '/?led1=off' in request:
        led.value(0)
    
    response = web_page()
    conn.send('HTTP/1.1 200 OK\nContent-Type: text/html\n\n' + response)
    conn.close()