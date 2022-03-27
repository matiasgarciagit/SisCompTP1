# client.py
import decimal
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

keyAPICambio = "2bb67f7ba83044608c6e1b3d220e013aefcf2ab0"

parametersAPICambio = {"api_key": keyAPICambio,"from": "USD", "format": "json"}

url = 'https://pro-api.coinmarketcap.com/v1/cryptocurrency/listings/latest'

urlCambio = 'https://api.getgeoapi.com/v2/currency/convert'

json = requests.get(url, params=params, headers=headers).json()
jsonCambio = requests.get(urlCambio, parametersAPICambio).json()

coins = json['data']
coinsCambio = jsonCambio['rates']

ars = ""
eur = ""

while True:
    for cambio in coinsCambio:
        if cambio == 'ARS':
            ars = str(round(decimal.Decimal(coinsCambio['ARS']['rate']), 2))
        if cambio == 'EUR':
            eur = str(round(decimal.Decimal(coinsCambio['EUR']['rate']), 2))
    break

print(ars)
print(eur)

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