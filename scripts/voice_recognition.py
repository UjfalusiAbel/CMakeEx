import socket
import threading
import sounddevice as sd
import time
import json
from vosk import Model, KaldiRecognizer

MODEL_PATH = "vosk-model-small-en-us-0.15"
PORT = 5000

print("Loading model...")
model = Model(MODEL_PATH)
rec = KaldiRecognizer(model, 16000)
rec.SetWords(False)
print("Finished loading model!")

# Networking with C++
clients = []

#Handling connecting to new client
def handle_client(address, connection):
    clients.append(connection)
    print(f"Client connected from address {address}")
    
#Broadcasting to already connected clients
def broadcast_command(cmd):
    for conn in clients:
        try:
            conn.sendall((cmd + " \n").encode())
        except:
            clients.remove(conn)

#Starting server for C++ to connect to
def start_server():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(("0.0.0.0", PORT))
    sock.listen()
    print(f"Server listening on port {PORT}")
    while True:
        conn, addr = sock.accept()
        threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()
    
threading.Thread(target=start_server, daemon=True).start()

#Recognizing audio
def register_audio(input_data, frames, time, status):
    COMMANDS=["checkin", "restroom","gate","restaurant","toilet","shops","duty-free"]
    isAccepted = rec.AcceptWaveform(bytes(input_data))
    if isAccepted:
        result = rec.Result()
        decoded = json.loads(result)
        text = decoded['text']
        print(text)
        for word in text.split():
            if word in COMMANDS:
                print(f"Command detected {word}")
                broadcast_command(word)
            
#Listening for audio from microphone        
with sd.RawInputStream(samplerate=16000, blocksize=8000, dtype="int16", channels=1, callback=register_audio):
    try: 
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Shutting down")
