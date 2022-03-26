# client.py
import socket
import sys
import os, os.path
import time
import requests
from sys import stdout
import socket
import sys
import os

headers = {
    'X-CMC_PRO_API_KEY': 'e86fa506-463e-496c-8107-6a32bec0ea5f',
    'Accepts': 'application/json'
}

params = {
    'start': '1',
    'limit': '2',
    'convert': 'USD'
}

url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/listings/latest'

json = requests.get(url, params=params, headers=headers).json()

coins = json['data']

btc = ""
eth = ""


ssock_file = "./socket"
csock_file = "./client_sock_py"

if os.path.exists(csock_file):
    os.remove(csock_file)

csock = socket.socket(socket.AF_UNIX, socket.SOCK_DGRAM)
csock.bind(csock_file)


while True:
    for coin in coins:
        if coin['symbol'] == 'BTC':
            btc = str(round(coin['quote']['USD']['price'], 3))
        if coin['symbol'] == 'ETH':
            eth = str(round(coin['quote']['USD']['price'], 3))
    break



csock.sendto(str.encode(btc), ssock_file)
csock.sendto(str.encode(eth), ssock_file)
(bytes, address) = csock.recvfrom(100)
msg = bytes.decode('utf-8')
#print('address:',address,'received:',msg)
(bytes, address) = csock.recvfrom(100)
msg = bytes.decode('utf-8')
#print('address:',address,'received:',msg)

csock.close()

if os.path.exists(csock_file):
    os.remove(csock_file)