import requests
import json
from pprint import pprint
    
def first_ai(chat_history):
    global first_ai_model, first_ai_ip
    
    data = {
            "model": first_ai_model,
            "messages": chat_history,
            "stream": False
    }
    
    url = f"http://{first_ai_ip}:11434/api/chat"
    response = requests.post(url, json=data)
    
    if response.status_code == 200:
        
        json_data = response.json()
        
        first_ai_response = json_data['message']
        
        first_ai_response["role"] = "user"
        
        chat_history.append(first_ai_response)
                
        print(f"\nREMOTE RESPONSE - {first_ai_response['content']}")
        
        second_ai(chat_history)
        
    else:
        print(f"Error: {response.status_code}")

def second_ai(chat_history):
    global second_ai_ip, second_ai_ip
    
    data = {
        "model": second_ai_ip,
        "messages": chat_history,
        "stream": False
    }
    
    url = f"http://{second_ai_ip}:11434/api/chat"
    response = requests.post(url, json=data)
    
    if response.status_code == 200:
        
        json_data = response.json()
        
        second_ai_response = json_data['message']

        second_ai_response["role"] = "user"
            
        chat_history.append(second_ai_response)

        print(f"\nLOCAL RESPONSE - {second_ai_response['content']}")

        first_ai(chat_history)
    
    else:
        print(f"Error: {response.status_code}")
    
    

if __name__ == "__main__":
    chat_history = []
    
    first_ai_ip = str(input("First IP: "))
    first_ai_model = str(input("First Model: "))
    
    second_ai_ip = str(input("Second IP: "))
    second_ai_ip = str(input("Second Model: "))
    
    initial_message = str(input("Initial message: "))
    chat_history.append({"role": "user", "content": initial_message})
    
    first_ai(chat_history)
    
