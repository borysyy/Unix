import requests
import json
from pprint import pprint

def receive_message(chat_history, ip):
    data = {
        "model": "llama3.1",
        "messages": chat_history,
        "stream": False
    }
    
    url = "http://localhost:11434/api/chat"
    response = requests.post(url, json=data)
    
    json_data = response.json()
    local_response = json_data['message']
    
    local_response["role"] = "user"
        
    chat_history.append(local_response)
    
    print(f"\nLOCAL RESPONSE - {local_response['content']}")
    
    send_message(chat_history, ip)
    
    
    
def send_message(chat_history, ip):
    data = {
            "model": "llama3.1",
            "messages": chat_history,
            "stream": False
        }
    url = f"http://{ip}:11434/api/chat"
    response = requests.post(url, json=data)
    
    if response.status_code == 200:
        # pprint(response.json())
        json_data = response.json()
        remote_response = json_data['message']
        
        
        remote_response["role"] = "user"
        
        chat_history.append(remote_response)
        
        pprint(f"CHAT HISTORY {chat_history}")
        
        print(f"\nREMOTE RESPONSE - {remote_response['content']}")
        
        receive_message(chat_history, ip)
        
    else:
        print(f"Error: {response.status_code}")
    


if __name__ == "__main__":
    chat_history = []
    initial_message = str(input("Initial message: "))
    chat_history.append({"role": "user", "content": initial_message})
    
    # ip = str(input("IP of other computer: "))
    ip = '150.156.81.71'
    send_message(chat_history, ip)
    
    
    # Keep getting a message from the user
    # while(1):
        
        # message = str(input("Message: "))
        
        # if message == "exit":
        #     break
        
                
       
 
        
        
                
