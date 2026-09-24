import socket

HOST = "0.0.0.0"  # listen on all interfaces
PORT = 5000

class DataReceiver:
    def __init__(self, dataConsumer, host=HOST, port=PORT):
        self.dataConsumer = dataConsumer
        self.host = host
        self.port = port

    def start(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
            server.bind((self.host, self.port))
            server.listen(1)

            print(f"Waiting for connection on port {self.port}...")

            conn, addr = server.accept()

            with conn:
                print(f"Connected to {addr}")

                while True:
                    data = conn.recv(2)

                    if not data:
                        print("Connection closed.")
                        break

                    while len(data) < 2:
                        more = conn.recv(2 - len(data))
                        if not more:
                            break
                        data += more

                    if len(data) != 2:
                        print("Incomplete data.")
                        break

                    self.dataConsumer.operate(data)
