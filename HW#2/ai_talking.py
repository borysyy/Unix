import requests
import json
from pprint import pprint
from ollama import Client


def receive_message(chat_history):

    client = Client(
        host=f"http://localhost:11435",
        headers={
            'Content-Type': 'application/json'
        }   
    )

    response = client.chat(
        model="llama3.1",
        messages=chat_history
    )

    response['message']['role'] = 'user'

    chat_history.append(response['message'])

    local_response = response['message']['content']
 
    print(f"\nLOCAL RESPONSE - {local_response}")
    
    send_message(chat_history)
    
    
    
def send_message(chat_history):
 
    client = Client(
        host=f"http://172.17.0.2:11434",
        headers={
            'Content-Type': 'application/json'
        }   
    )

    response = client.chat(
        model="llama3.1",
        messages=chat_history
    )

    response['message']['role'] = 'user'

    chat_history.append(response['message'])

    remote_response = response['message']['content']

    # pprint(f"CHAT HISTORY {chat_history}")
    
    print(f"\nREMOTE RESPONSE - {remote_response}")
    
    receive_message(chat_history)
  


if __name__ == "__main__":
    chat_history = []
    initial_message = str(input("Initial message: "))
    chat_history.append({"role": "user", "content": initial_message})
    
    # ip = str(input("IP of other computer: "))
    ip = '172.17.0.2'
    send_message(chat_history)
    
    
    # Keep getting a message from the user
    # while(1):
        
        # message = str(input("Message: "))
        
        # if message == "exit":
        #     break
        
                
       
 
        
        
                
